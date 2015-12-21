#ifndef _INCLUDE__LAUNCHER_MANAGER_CONNECTION_H_
#define _INCLUDE__LAUNCHER_MANAGER_CONNECTION_H_

#include <launcher_manager/session/connection.h>

static jmp_buf fork_jmp_buf;
static Genode::Capability<Genode::Parent>::Raw new_parent;

extern "C" void stdout_reconnect(); /* provided by 'log_console.cc' */

namespace LauncherManager {
  void checkpoint(void)
  {

  }

  void restore(void)
  {

  }

  void fork_trampoline()
  {

  }

  int fork(void)
  {

  }
}

#endif
