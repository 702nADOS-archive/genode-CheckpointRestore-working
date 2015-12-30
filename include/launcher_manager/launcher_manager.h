#ifndef _INCLUDE__LAUNCHER_MANAGER_CONNECTION_H_
#define _INCLUDE__LAUNCHER_MANAGER_CONNECTION_H_

#include <launcher_manager/session/connection.h>
#include <os/config.h>
#include <util/construct_at.h>
// #include <setjmp.h>


// static jmp_buf fork_jmp_buf;
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
    /* reinitialize environment */
  	using namespace Genode;

    printf("FORK ENTRY");
  	// env()->reinit(new_parent.dst, new_parent.local_name);

  	/* reinitialize standard-output connection */
  	// stdout_reconnect();

  	/* reinitialize config */
  	// Genode::Config *config = Genode::config();
  	// construct_at<Genode::Config>(config);

  	/* reinitialize noux connection */
  	// construct_at<Noux_connection>(noux_connection());

  	/* reinitialize main-thread object which implies reinit of context area */
  	// auto context_area_rm = noux_connection()->context_area_rm_session();
  	// env()->reinit_main_thread(context_area_rm);

  	/* apply processor state that the forker had when he did the fork */
  	// longjmp(fork_jmp_buf, 1);
  }

  int fork(void)
  {
    return -1;
  }
}

#endif
