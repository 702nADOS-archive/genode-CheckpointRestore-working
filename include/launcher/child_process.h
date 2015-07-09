#ifndef _INCLUDE__CHILD_PROCESS_H_
#define _INCLUDE__CHILD_PROCESS_H_

#include <base/allocator.h>
#include <base/service.h>
#include <base/printf.h>
#include <base/lock.h>
#include <cap_session/connection.h>
#include <init/child.h>

#include "child_process_policy.h"

class ChildProcess
{
  Genode::Rom_session_capability _rom;
  Genode::Ram_session_capability _ram;
  Genode::Cpu_session_capability _cpu;
  Genode::Rm_session_capability  _rm;
  Genode::Pd_session_capability  _pd;
  Genode::Server                 _server;

  /*
   * Entry point used for serving the parent interface and the
   * locally provided ROM sessions for the 'config' and 'binary'
   * files.
   */
  enum { ENTRYPOINT_STACK_SIZE = 12*1024 };
  Genode::Rpc_entrypoint _entrypoint;

  ChildProcessPolicy     _policy;
  Genode::Child          _child;
public:
  ChildProcess(const char                       *name,
                  Genode::Dataspace_capability   elf_ds,
                  Genode::Pd_session_capability  pd,
                  Genode::Ram_session_capability ram,
                  Genode::Cpu_session_capability cpu,
                  Genode::Rm_session_capability  rm,
                  Genode::Rom_session_capability rom,
                  Genode::Cap_session           *cap_session,
                  Genode::Service_registry      *parent_services,
                  Genode::Service_registry      *child_services,
                  Genode::Dataspace_capability   config_ds);

  Genode::Rom_session_capability rom_session_cap();
  Genode::Ram_session_capability ram_session_cap();
  Genode::Cpu_session_capability cpu_session_cap();
  Genode::Rm_session_capability  rm_session_cap();
  Genode::Pd_session_capability  pd_session_cap();
  Genode::Thread_capability      thread_cap();

  const char* name() const;

  const Genode::Server* server() const;

  Genode::Allocator* heap();

  void revoke_server(const Genode::Server *server);

  void exit();
};

#endif
