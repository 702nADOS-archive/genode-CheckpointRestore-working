#include <child_process.h>
#include <base/service.h>
#include <base/process.h>
#include <session/capability.h>

ChildProcess::ChildProcess(const char                       *name,
                Genode::Dataspace_capability   elf_ds,
                Genode::Pd_session_capability  pd,
                Genode::Ram_session_capability ram,
                Genode::Cpu_session_capability cpu,
                Genode::Rm_session_capability  rm,
                Genode::Rom_session_capability rom,
                Genode::Cap_session           *cap_session,
                Genode::Service_registry      *parent_services,
                Genode::Service_registry      *child_services,
                Genode::Dataspace_capability   config_ds):
          				_rom(rom),
                  _ram(ram),
                  _cpu(cpu),
                  _rm(rm),
                  _server(_ram),
                  _pd(pd),
          				_entrypoint(cap_session, ENTRYPOINT_STACK_SIZE, name, false),
          				_policy(name, &_server, parent_services, child_services, config_ds, elf_ds, &_entrypoint),
          				_child(elf_ds, pd, ram, cpu, rm, &_entrypoint, &_policy),
                  _id(-1)
{
  Genode::printf("OK\n");
}

void ChildProcess::start()
{
  _entrypoint.activate();
}

void ChildProcess::start_forked_main_thread(Genode::addr_t ip, Genode::addr_t sp, Genode::addr_t parent_cap_addr)
{
  _entrypoint.activate();

  return;
  /* poke parent_cap_addr into child's address space */
  Genode::Capability<Genode::Parent> const &cap = _child.parent_cap();
  Genode::Capability<Genode::Parent>::Raw   raw = { cap.dst(), cap.local_name() };
  // _rm.poke(parent_cap_addr, &raw, sizeof(raw));

  /* start execution of new main thread at supplied trampoline */
  Genode::Cpu_session_client cpu(_cpu);
  // cpu.create_thread(size_t quota, Name const &name, addr_t utcb = 0)
  cpu.start(cpu.create_thread(0, _policy.name()), ip, sp);
}

void ChildProcess::setId(int id)
{
  _id = id;
}

int ChildProcess::getId()
{
  return _id;
}

Genode::Pd_session_capability ChildProcess::pd_session_cap()
{
  return _pd;
}

Genode::Rom_session_capability ChildProcess::rom_session_cap()
{
  return _rom;
}

Genode::Ram_session_capability ChildProcess::ram_session_cap()
{
  return _ram;
}

Genode::Cpu_session_capability ChildProcess::cpu_session_cap()
{
  return _cpu;
}

Genode::Rm_session_capability ChildProcess::rm_session_cap()
{
  return _rm;
}

Genode::Thread_capability ChildProcess::thread_cap()
{
  return _child.main_thread_cap();
}

const char* ChildProcess::name() const
{
  return _policy.name();
}

const Genode::Server* ChildProcess::server() const
{
  return &_server;
}

Genode::Allocator* ChildProcess::heap()
{
  return _child.heap();
}

void ChildProcess::revoke_server(const Genode::Server *server)
{
  _child.revoke_server(server);
}

void ChildProcess::close(Genode::Session_capability cap)
{
  _child.close(cap);
}

void ChildProcess::exit()
{
  _child.exit(0);
}
