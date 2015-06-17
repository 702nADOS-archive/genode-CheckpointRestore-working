#include <base/allocator.h>
#include <base/service.h>
#include <base/printf.h>
#include <base/lock.h>
#include <cap_session/connection.h>
#include <timer_session/timer_session.h>

class Launcher
{
public:
  Launcher();
  ~Launcher();

  void init();
  void start_child(const char* filename, unsigned int ram_quota);
};
