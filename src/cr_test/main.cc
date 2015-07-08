#include <base/printf.h>
// #include <parent/parent.h>
// #include <parent/client.h>
// #include <parent/capability.h>
#include <base/process.h>
#include <base/env.h>
// #include <rom_session/capability.h>
#include <rom_session/client.h>

#include <ram_session/capability.h>

#include <cpu_session/capability.h>
#include <cpu_session/client.h>

#include <pd_session/capability.h>
#include <pd_session/client.h>

// #include <thread_session/capability.h>
// #include <thread_session/client.h>

#include <base/connection.h>
#include <dataspace/client.h>
#include <base/thread.h>
#include <thread/capability.h>
#include <pager/capability.h>

// using namespace Genode;

#define PRINT_STUFF Genode::printf("Hello world from sudi_test: %i/%i\n", Genode::env()->ram_session()->used(), Genode::env()->ram_session()->quota());

// int start_child(char* filename, unsigned int ram_quota, Genode::Dataspace_capability config_ds)
// {
//   printf("starting %s with quota %lu\n", filename, ram_quota);
//
// 	/* find unique name for new child */
// 	char unique_name[64];
// 	_get_unique_child_name(filename, unique_name, sizeof(unique_name));
// 	printf("using unique child name \"%s\"\n", unique_name);
//
// 	if (ram_quota > env()->ram_session()->avail()) {
// 		PERR("Child's ram quota is higher than our available quota, using available quota");
// 		ram_quota = env()->ram_session()->avail() - 256*1000;
// 	}
//
// 	size_t metadata_size = 4096*16 + sizeof(Launchpad_child);
//
// 	if (metadata_size > ram_quota) {
// 		PERR("Too low ram_quota to hold child metadata");
// 		return 0;
// 	}
//
// 	ram_quota -= metadata_size;
//
// 	/* lookup executable elf binary */
// 	Dataspace_capability  file_cap;
// 	Rom_session_capability rom_cap;
// 	try {
// 		/*
// 		 * When creating a ROM connection for a non-existing file, the
// 		 * constructor of 'Rom_connection' throws a 'Parent::Service_denied'
// 		 * exception.
// 		 */
// 		Rom_connection rom(filename, unique_name);
// 		rom.on_destruction(Rom_connection::KEEP_OPEN);
// 		rom_cap  = rom.cap();
// 		file_cap = rom.dataspace();
// 	} catch (...) {
// 		printf("Error: Could not access file \"%s\" from ROM service.\n", filename);
// 		return 0;
// 	}
//
// 	/* create ram session for child with some of our own quota */
// 	Ram_connection ram;
// 	ram.on_destruction(Ram_connection::KEEP_OPEN);
// 	ram.ref_account(env()->ram_session_cap());
// 	env()->ram_session()->transfer_quota(ram.cap(), ram_quota);
//
// 	/* create cpu session for child */
// 	Cpu_connection cpu(unique_name);
// 	cpu.on_destruction(Cpu_connection::KEEP_OPEN);
//
// 	if (!ram.cap().valid() || !cpu.cap().valid()) {
// 		if (ram.cap().valid()) {
// 			PWRN("Failed to create CPU session");
// 			env()->parent()->close(ram.cap());
// 		}
// 		if (cpu.cap().valid()) {
// 			PWRN("Failed to create RAM session");
// 			env()->parent()->close(cpu.cap());
// 		}
// 		env()->parent()->close(rom_cap);
// 		PERR("Our quota is %zd", env()->ram_session()->quota());
// 		return 0;
// 	}
//
// 	Rm_connection rm;
// 	rm.on_destruction(Rm_connection::KEEP_OPEN);
// 	if (!rm.cap().valid()) {
// 		PWRN("Failed to create RM session");
// 		env()->parent()->close(ram.cap());
// 		env()->parent()->close(cpu.cap());
// 		env()->parent()->close(rom_cap);
// 		return 0;
// 	}
//
// 	Pd_connection pd;
// 	pd.on_destruction(Pd_connection::KEEP_OPEN);
// 	if (!pd.cap().valid()) {
// 		PWRN("Failed to create PD session");
// 		env()->parent()->close(ram.cap());
// 		env()->parent()->close(cpu.cap());
// 		env()->parent()->close(rom_cap);
// 		env()->parent()->close(rm.cap());
// 		return 0;
// 	}
//
// 	try {
// 		Launchpad_child *c = new (&_sliced_heap)
// 			Launchpad_child(unique_name, file_cap, pd.cap(), ram.cap(),
// 			                cpu.cap(), rm.cap(), rom_cap,
// 			                &_cap_session, &_parent_services, &_child_services,
// 			                config_ds, this);
//
// 		Lock::Guard lock_guard(_children_lock);
// 		_children.insert(c);
//
// 		add_child(unique_name, ram_quota, c, c->heap());
//
// 		return c;
// 	} catch (Cpu_session::Thread_creation_failed) {
// 		PWRN("Failed to create child - Cpu_session::Thread_creation_failed");
// 	} catch (...) {
// 		PWRN("Failed to create child - unknown reason");
// 	}
//
// 	env()->parent()->close(rm.cap());
// 	env()->parent()->close(ram.cap());
// 	env()->parent()->close(cpu.cap());
// 	env()->parent()->close(rom_cap);
//
// 	return 0;
// }
#include <launcher/launcher.h>
#include <base/sleep.h>

int main(int argc, char const *argv[])
{
  PRINT_STUFF
  Launcher launcher;
  launcher.init();
  launcher.start_child("cr_sub", 1024*1024*5);
  Genode::sleep_forever();
  // Genode::Service_registry _parent_services;
  // if (_parent_services.find("RM"))
  //   Genode::printf("Found service\n");
  // else
  //   Genode::printf("Mega fail\n");
  return 0;
}

// int main1(int argc, char const *argv[]) {
//   PRINT_STUFF
//
//   Genode::Rom_session_capability file_cap;
//   file_cap = Genode::env()->parent()->session("ROM", "filename=sudi_sub, ram_quota=4M");
//   Genode::Rom_dataspace_capability ds_cap;
//   ds_cap = Genode::Rom_session_client(file_cap).dataspace();
//
//   PRINT_STUFF
//
//   void *elf_addr = Genode::env()->rm_session()->attach(ds_cap);
//
//   Genode::Rm_session_capability rm_cap;
//   rm_cap = Genode::env()->parent()->session("RM", "ram_quota=4M");
//   Genode::Rm_session_client rsc(rm_cap);
//
//   PRINT_STUFF
//
//   rsc.attach(ds_cap, Genode::Dataspace_client(ds_cap).size(), 0, true, elf_addr, true);
//
//   Genode::Ram_dataspace_capability rw_cap = Genode::env()->ram_session()->alloc(1024*1024*4);
//   //
//   void *sec_addr = Genode::env()->rm_session()->attach(rw_cap);
//   /* write to buffer at sec_addr */
//   Genode::env()->rm_session()->detach(sec_addr);
//
//   PRINT_STUFF
//
//
//   Genode::Cpu_session_capability cpu_cap = Genode::env()->parent()->session("CPU", "ram_quota=4M");
//   Genode::Cpu_session_client csc(cpu_cap);
//   Genode::Thread_capability thread_cap = csc.create_thread(2, "lol");
//
//   PRINT_STUFF
//
//   Genode::Pager_capability pager_cap = rsc.add_client(thread_cap);
//   csc.set_pager(thread_cap, pager_cap);
//
//   // PRINT_STUFF
//   //
//   // Genode::Pd_session_capability pd_cap = Genode::env()->parent()->session("PD", "ram_quota=1M");
//   // Genode::Pd_session_client pdsc(pd_cap);
//   //
//   // PRINT_STUFF
//   //
//   // pdsc.bind_thread(thread_cap);
//   //
//   // PRINT_STUFF
//   //
//   // csc.start(thread_cap, 0, 0);
//   return 0;
// }
