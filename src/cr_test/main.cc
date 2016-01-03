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

#include <rm_session/connection.h>
#include <ram_session/connection.h>


#include <base/printf.h>
#include <base/sleep.h>
#include <timer_session/connection.h>
#include <launcher_manager/session/connection.h>

#define PRINT_STUFF Genode::printf("Hello world from sudi_test: %i/%i\n", Genode::env()->ram_session()->used(), Genode::env()->ram_session()->quota());


//simple test case to attach 2 dataspace to the same address
int testShadowCopy()
{
  try
  {
    Genode::Dataspace_capability dataspace1 = Genode::env()->ram_session()->alloc(4);
    Genode::Dataspace_capability dataspace2 = Genode::env()->ram_session()->alloc(4);

    int *src1 = Genode::env()->rm_session()->attach(dataspace1);
    int *src2 = 0;
    try
    {
      src2 = Genode::env()->rm_session()->attach(dataspace2, 0, 0, true, src1);
    } catch(Genode::Rm_session::Invalid_args attach_failed)
    {
      PERR("Attach failed: invalid args\n");
      PERR("ShadowCopy does not work\n");
      return 1;
    } catch(Genode::Rm_session::Invalid_dataspace attach_failed)
    {
      PERR("Attach failed: invalid dataspace\n");
      PERR("ShadowCopy does not work\n");
      return 1;
    } catch(Genode::Rm_session::Region_conflict attach_failed)
    {
      PERR("Attach failed: region conflict\n");
      PERR("ShadowCopy does not work\n");
      return 1;
    } catch(Genode::Rm_session::Out_of_metadata attach_failed)
    {
      PERR("Attach failed: out of metadata\n");
      PERR("ShadowCopy does not work\n");
      return 1;
    }
    *src1 = 4;
    if (*src1 == *src2) Genode::printf("ShadowCopy works\n");
    else PERR("ShadowCopy does not work\n");

    //Cleanup
    Genode::env()->rm_session()->detach(src1);
    Genode::env()->rm_session()->detach(src2);
    Genode::env()->ram_session()->free(dataspace1);
    Genode::env()->ram_session()->free(dataspace2);
  } catch(Genode::Ram_session::Alloc_failed alloc_failed) {
    PERR("Alloc failed\n");
  } catch(Genode::Ram_session::Quota_exceeded alloc_failed) {
    PERR("Quota exceeded\n");
  } catch(Genode::Ram_session::Out_of_metadata alloc_failed) {
    PERR("out of metadata\n");
  }
  return 0;
}

int main(int argc, char const *argv[])
{
  testShadowCopy();

  Timer::Connection timer;

  LauncherManager::Connection launcher;
  launcher.say_hello();

  int child = 0;//launcher.createChild("cr_sub", 1024*1024*5);

  bool paused = false;
  int runs = 0;

  while (1)
  {
    PRINT_STUFF

    int child1 = launcher.createChild("cr_sub", 1024*1024);
    timer.msleep(1500);
    launcher.pushThreadState(child1);
    launcher.kill(child1);

    runs++;
    Genode::printf("This is launcher run: %i\n", runs);
  }
}
