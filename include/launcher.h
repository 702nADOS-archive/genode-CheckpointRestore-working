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

private:
  Genode::Sliced_heap _sliced_heap;
};

#endif
