#include "session_component.h"
#include <launcher/child_process.h>
#include <base/printf.h>

namespace LauncherManager {
  Session_component::Session_component()
  {
    _nextId = 0;
    launcher.init();
  }

  Session_component::~Session_component()
  {
  }

  int Session_component::createChild(String const & filename, unsigned int ram_quota)
  {
    Genode::printf("Ram usage: %i/%i\n", Genode::env()->ram_session()->used(), Genode::env()->ram_session()->avail());
    ChildProcess* child = launcher.start_child(filename.string(), ram_quota);
    _children.insert(child);
    child->setId(_nextId++);
    Genode::printf("Ram usage: %i/%i\n", Genode::env()->ram_session()->used(), Genode::env()->ram_session()->avail());
    return child->getId();
  }

  void Session_component::kill(int process)
  {
    Genode::printf("Ram usage: %i/%i\n", Genode::env()->ram_session()->used(), Genode::env()->ram_session()->avail());
    ChildProcess* child = _children.first();
    while(child != NULL)
    {
      if (child->getId() == process) {
        _children.remove(child);
        launcher.kill(child);
        child = NULL;
      } else {
        child = child->next();
      }
    }
    Genode::printf("Ram usage: %i/%i\n", Genode::env()->ram_session()->used(), Genode::env()->ram_session()->avail());
  }

  void Session_component::pause(int process)
  {
    Genode::printf("Ram usage: %i/%i\n", Genode::env()->ram_session()->used(), Genode::env()->ram_session()->avail());
    ChildProcess* child = _children.first();
    while(child != NULL)
    {
      if (child->getId() == process) {
        launcher.pause(child);
        child = NULL;
      } else {
        child = child->next();
      }
    }
    Genode::printf("Ram usage: %i/%i\n", Genode::env()->ram_session()->used(), Genode::env()->ram_session()->avail());
  }

  void Session_component::resume(int process)
  {
    Genode::printf("Ram usage: %i/%i\n", Genode::env()->ram_session()->used(), Genode::env()->ram_session()->avail());
    ChildProcess* child = _children.first();
    while(child != NULL)
    {
      if (child->getId() == process) {
        launcher.resume(child);
        child = NULL;
      } else {
        child = child->next();
      }
    }
    Genode::printf("Ram usage: %i/%i\n", Genode::env()->ram_session()->used(), Genode::env()->ram_session()->avail());
  }

  void Session_component::say_hello()
  {
    PDBG("I am here... Hello.");
  }
}
