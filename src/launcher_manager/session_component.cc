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
    Launcher::ChildProcess* child = launcher.start_child(filename.string(), ram_quota);
    _children.insert(child);
    child->setId(_nextId++);
    Genode::printf("Ram usage: %i/%i\n", Genode::env()->ram_session()->used(), Genode::env()->ram_session()->avail());
    return child->getId();
  }

  void Session_component::kill(int process)
  {
    Genode::printf("Ram usage: %i/%i\n", Genode::env()->ram_session()->used(), Genode::env()->ram_session()->avail());
    Launcher::ChildProcess* child = _children.first();
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
    Launcher::ChildProcess* child = _children.first();
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
    Launcher::ChildProcess* child = _children.first();
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

  void Session_component::pushThreadState(int process)
  {
    Launcher::ChildProcess* child = _children.first();
    while(child != NULL)
    {
      if (child->getId() == process) {
        pushedThreadState = launcher.thread_state(child);
        Genode::printf("ThreadState: %i\n", pushedThreadState.ip);
        // addr_t ip     = 0;   /* instruction pointer */16820189
      	// addr_t sp     = 0;   /* stack pointer       */1074789704
      	// addr_t edi    = 0;
      	// addr_t esi    = 0;
      	// addr_t ebp    = 0;
      	// addr_t ebx    = 0;
      	// addr_t edx    = 0;
      	// addr_t ecx    = 0;
      	// addr_t eax    = 0;
      	// addr_t gs     = 0;
      	// addr_t fs     = 0;
      	// addr_t eflags = 0;
      	// addr_t trapno = 0;
        child = NULL;
      } else {
        child = child->next();
      }
    }
  }

  void Session_component::popThreadState(int process)
  {
    Launcher::ChildProcess* child = _children.first();
    while(child != NULL)
    {
      if (child->getId() == process) {
        launcher.thread_state(child, pushedThreadState);
        child = NULL;
      } else {
        child = child->next();
      }
    }
  }

  void Session_component::say_hello()
  {
    PDBG("I am here... Hello.");
  }
}
