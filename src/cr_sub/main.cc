#include <base/printf.h>
#include <timer_session/connection.h>
#include <launcher_manager/launcher_manager.h>

int main(int argc, char const *argv[]) {
  Genode::printf("Hello world from cr_sub\n");
  Genode::printf("it actually works\n");

  Timer::Connection timer;
  int counter = 0;



  Genode::printf("trampoline starting\n");
  LauncherManager::fork_trampoline();
  Genode::printf("trampoline ending\n");

  while(1)
  {
    Genode::printf("Hey i am a child at: %i\n", counter);
    counter++;
    timer.msleep(200);
  }
  return 0;
}
