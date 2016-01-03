#include <timer_session/connection.h>
namespace Launcher
{
  class ChildProcess;

  void destruct_child(Genode::Allocator *alloc, ChildProcess *child,
                             Timer::Session *timer, int timeout);

}
