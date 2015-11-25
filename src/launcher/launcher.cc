#include <launcher.h>
#include <child_process.h>

#include <base/env.h>
#include <base/child.h>
#include <base/sleep.h>
#include <base/service.h>
#include <base/snprintf.h>
#include <base/blocking.h>
#include <rom_session/connection.h>
#include <ram_session/connection.h>
#include <cpu_session/connection.h>
#include <pd_session/connection.h>
#include <rm_session/connection.h>
#include <os/config.h>
#include <timer_session/connection.h>
#include <base/service.h>

Launcher::Launcher() : _sliced_heap(Genode::env()->ram_session(), Genode::env()->rm_session())
{

}

Launcher::~Launcher()
{

}

void Launcher::init()
{
  /* names of services provided by the parent */
  static const char *names[] = {

    /* core services */
    "CAP", "RAM", "RM", "PD", "CPU", "IO_MEM", "IO_PORT",
    "IRQ", "ROM", "LOG", "SIGNAL",

    /* services expected to got started by init */
    "Timer",
    // "Nitpicker", "Init", "Timer", "PCI", "Block", "Nic", "Rtc",

    0 /* null-termination */
  };
  for (unsigned i = 0; names[i]; i++)
    _parent_services.insert(new (Genode::env()->heap()) Genode::Parent_service(names[i]));
}

ChildProcess* Launcher::start_child(const char* filename, unsigned int ram_quota)
{
  if (ram_quota >  Genode::env()->ram_session()->avail()) {
		PERR("Child's ram quota is higher than our available quota, using available quota");
		ram_quota =  Genode::env()->ram_session()->avail() - 256*1000;
	}
  Genode::printf("Starting: %s with %i/%i\n", filename, ram_quota, Genode::env()->ram_session()->avail());

  //TODO: have to reduce it by the metadata size of a child in this launcher
  //TODO: generate unique name incase we want multiple instances of the same programm
  const char* unique_name = filename;

  /* lookup executable elf binary */
  Genode::Dataspace_capability  file_cap;
  Genode::Rom_session_capability rom_cap;
	try {
		/*
		 * When creating a ROM connection for a non-existing file, the
		 * constructor of 'Rom_connection' throws a 'Parent::Service_denied'
		 * exception.
		 */
    Genode::Rom_connection rom(filename, unique_name);
		rom.on_destruction(Genode::Rom_connection::KEEP_OPEN);
		rom_cap  = rom.cap();
		file_cap = rom.dataspace();
	} catch (...) {
    Genode::printf("Error: Could not access file \"%s\" from ROM service.\n", filename);
		return 0;
	}

  /* create ram session for child with some of our own quota */
  Genode::Ram_connection ram;
	ram.on_destruction(Genode::Ram_connection::KEEP_OPEN);
	ram.ref_account(Genode::env()->ram_session_cap());
  Genode::env()->ram_session()->transfer_quota(ram.cap(), ram_quota);

	/* create cpu session for child */
  Genode::Cpu_connection cpu(unique_name);
	cpu.on_destruction(Genode::Cpu_connection::KEEP_OPEN);

  if (!ram.cap().valid() || !cpu.cap().valid()) {
		if (ram.cap().valid()) {
			PWRN("Failed to create CPU session");
      Genode::env()->parent()->close(ram.cap());
		}
		if (cpu.cap().valid()) {
			PWRN("Failed to create RAM session");
      Genode::env()->parent()->close(cpu.cap());
		}
    Genode::env()->parent()->close(rom_cap);
		PERR("Our quota is %zd", Genode::env()->ram_session()->quota());
		return 0;
	}

  Genode::Rm_connection rm;
	rm.on_destruction(Genode::Rm_connection::KEEP_OPEN);
	if (!rm.cap().valid()) {
    Genode::printf("FAILED RM SESSION\n");
		PWRN("Failed to create RM session");
    Genode::env()->parent()->close(ram.cap());
    Genode::env()->parent()->close(cpu.cap());
    Genode::env()->parent()->close(rom_cap);
		return 0;
	}

  Genode::Pd_connection pd;
	pd.on_destruction(Genode::Pd_connection::KEEP_OPEN);
	if (!pd.cap().valid()) {
		PWRN("Failed to create PD session");
    Genode::env()->parent()->close(ram.cap());
    Genode::env()->parent()->close(cpu.cap());
    Genode::env()->parent()->close(rom_cap);
    Genode::env()->parent()->close(rm.cap());
		return 0;
	}

  //TODO: start the process here!!!!
  Genode::printf("All seams good\n");

  try {
    // return 0;
    // use Genode::Dataspace_capability()
    // Genode::Dataspace_connection ds_connection;

    return new (&_sliced_heap) ChildProcess(unique_name, file_cap, pd.cap(), ram.cap(),
                    cpu.cap(), rm.cap(), rom_cap,
                    &_cap_session, &_parent_services, &_child_services,
                    Genode::Dataspace_capability());

  } catch (Genode::Cpu_session::Thread_creation_failed) {
    PWRN("Failed to create child - Cpu_session::Thread_creation_failed");
  } catch (...) {
    PWRN("Failed to create child - unknown reason");
  }


  //cleanup
  Genode::env()->parent()->close(rm.cap());
  Genode::env()->parent()->close(ram.cap());
  Genode::env()->parent()->close(cpu.cap());
  Genode::env()->parent()->close(rom_cap);

  return 0;
}

void Launcher::single_step(ChildProcess* child, bool step)
{
  Genode::printf("Step child\n", step);
  Genode::Cpu_session_client client(child->cpu_session_cap());
  client.single_step(child->thread_cap(), step);

}

void Launcher::pause(ChildProcess* child)
{
  Genode::printf("Pause child\n");
  Genode::Cpu_session_client client(child->cpu_session_cap());
  client.pause(child->thread_cap());
}

void Launcher::resume(ChildProcess* child)
{
  Genode::printf("Resume child\n");
  Genode::Cpu_session_client client(child->cpu_session_cap());
  client.resume(child->thread_cap());
}

Genode::Thread_state Launcher::thread_state(ChildProcess* child)
{
  pause(child);
  Genode::Cpu_session_client client(child->cpu_session_cap());
  Genode::Thread_state state = client.state(child->thread_cap());
  // Genode::Ipc_pager native = (Genode::Ipc_pager)client.native_cap(child->thread_cap);
  // native.get_regs(state);
  //
  // Genode::printf("Saved registers\n");
  resume(child);
  return state;
}

void Launcher::thread_state(ChildProcess* child, Genode::Thread_state state)
{
  pause(child);
  Genode::Cpu_session_client client(child->cpu_session_cap());
  client.state(child->thread_cap(), state);
  resume(child);
}

ChildProcess* Launcher::clone(ChildProcess* origin)
{
  // go through threads with first then next on cpu session
  Genode::Cpu_session_client client(origin->cpu_session_cap());
  // Genode::Ram_dataspace_capability ram =  client.utcb(origin->thread_cap());
  pause(origin);
  Genode::Thread_state state = client.state(origin->thread_cap());

  Genode::Ram_session_client ram(origin->ram_session_cap());
  
  Genode::printf("Origin is paused?: %s\n", state.paused ? "true":"false");

  resume(origin);
  return 0;
}

void Launcher::kill(ChildProcess* child)
{
  Genode::Rm_session_capability   rm_session_cap = child->rm_session_cap();
  Genode::Ram_session_capability ram_session_cap = child->ram_session_cap();
  Genode::Cpu_session_capability cpu_session_cap = child->cpu_session_cap();
  Genode::Rom_session_capability rom_session_cap = child->rom_session_cap();
  Genode::Pd_session_capability  pd_session_cap  = child->pd_session_cap();

	const Genode::Server *server = child->server();
	// destroy(&_sliced_heap, child);

	Genode::env()->parent()->close(rm_session_cap);
  Genode::env()->parent()->close(cpu_session_cap);
  Genode::env()->parent()->close(rom_session_cap);
  Genode::env()->parent()->close(ram_session_cap);
  Genode::env()->parent()->close(pd_session_cap);

  // destroy(&_sliced_heap, child);

  //TODO: revoke server from other children so connections to them are closed

  delete child;

}
