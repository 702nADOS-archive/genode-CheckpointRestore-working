// #include <base/printf.h>
// // #include <parent/parent.h>
// // #include <parent/client.h>
// // #include <parent/capability.h>
// #include <base/process.h>
// #include <base/env.h>
// // #include <rom_session/capability.h>
// #include <rom_session/client.h>
//
// #include <ram_session/capability.h>
//
// #include <cpu_session/capability.h>
// #include <cpu_session/client.h>
//
// #include <pd_session/capability.h>
// #include <pd_session/client.h>
//
// // #include <thread_session/capability.h>
// // #include <thread_session/client.h>
//
// #include <base/connection.h>
// #include <dataspace/client.h>
// #include <base/thread.h>
// #include <thread/capability.h>
// #include <pager/capability.h>

// using namespace Genode;

#include <base/printf.h>
#include <base/sleep.h>
#include <timer_session/connection.h>
#include <launcher_manager/session/connection.h>

#define PRINT_STUFF Genode::printf("Hello world from sudi_test: %i/%i\n", Genode::env()->ram_session()->used(), Genode::env()->ram_session()->quota());

int main(int argc, char const *argv[])
{
  Timer::Connection timer;

  // {
  //   LauncherManager::Connection launcher_main;
  //   launcher_main.createChild("launcher_manager", 1024*1024*10);
  //   timer.msleep(2000);
  // }

  LauncherManager::Connection launcher;
  launcher.say_hello();

  int child = 0;//launcher.createChild("cr_sub", 1024*1024*5);

  bool paused = false;
  int runs = 0;

  while (1)
  {
    PRINT_STUFF
    // timer.msleep(200);
    // if (runs%2 == 0) {
    //   child = launcher.createChild("cr_sub", 1024*1024);
    // } else if (runs%3 == 0) {
    //   launcher.kill(child);
    // }
    int child1 = launcher.createChild("cr_sub", 1024*1024);
    timer.msleep(1500);
    launcher.pushThreadState(child1);
    launcher.kill(child1);

    // child1 = launcher.createChild("cr_test", 1024*1024);
    // timer.msleep(500);
    // launcher.pushThreadState(child1);
    // launcher.kill(child1);

    // POP thread state
    // child1 = launcher.createChild("cr_sub", 1024*1024);
    // timer.msleep(200);
    // launcher.popThreadState(child1);
    // timer.msleep(2000);
    // launcher.kill(child1);

    // if (runs == 3)
    // {
    //   Genode::printf("Before kill\n");
    //   launcher.kill(child);
    //   Genode::printf("After kill\n");
    // }
    // else
    // {
    //   if (!paused)
    //     launcher.pause(child);
    //   else
    //     launcher.resume(child);
    //   paused = !paused;
    // }
    runs++;
    Genode::printf("This is launcher run: %i\n", runs);
  }
}
