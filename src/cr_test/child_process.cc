#include <child_process.h>

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
          				_entrypoint(cap_session, ENTRYPOINT_STACK_SIZE, name, false),
          				_policy(name, &_server, parent_services, child_services, config_ds, elf_ds, &_entrypoint),
          				_child(elf_ds, pd, ram, cpu, rm, &_entrypoint, &_policy)
{
  _entrypoint.activate();
  Genode::printf("OK\n");
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
