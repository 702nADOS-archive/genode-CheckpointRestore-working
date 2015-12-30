#ifndef _INCLUDE__LAUNCHER_H_
#define _INCLUDE__LAUNCHER_H_

#include <base/allocator.h>
#include <base/service.h>
#include <base/printf.h>
#include <base/lock.h>
#include <cap_session/connection.h>
#include <timer_session/timer_session.h>

#include <init/child.h>

class ChildProcess;

class Launcher
{
public:
  Launcher();
  ~Launcher();

  void init();
  ChildProcess* start_child(const char* filename, unsigned int ram_quota);
  void kill(ChildProcess* child);
  void single_step(ChildProcess* child, bool step);
  void pause(ChildProcess* child);
  void resume(ChildProcess* child);

  ChildProcess* clone(ChildProcess* origin);
  Genode::Thread_state thread_state(ChildProcess* child);
  void thread_state(ChildProcess* child, Genode::Thread_state state);

private:

  Genode::Sliced_heap _sliced_heap;
  Genode::Cap_connection _cap_session;
  Genode::Service_registry _parent_services;
  Genode::Service_registry _child_services;

  // Entrypoint used to store locally provided services
  enum { ENTRYPOINT_STACK_SIZE = 12*1024 };
  Genode::Rpc_entrypoint _entrypoint;
};

#endif
