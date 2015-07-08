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
    "Nitpicker", "Init", "Timer", "PCI", "Block", "Nic", "Rtc",

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
