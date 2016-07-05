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
    Genode::printf("Child ID: %i\n", child->getId());
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

  void Session_component::pushThreadState(int process)
  {
    ChildProcess* child = _children.first();
    while(child != NULL)
    {
      if (child->getId() == process) {
        pushedThreadState = launcher.thread_state(child);

        Genode::printf("Register ip: %i\n", pushedThreadState.ip);
        Genode::printf("Register sp: %i\n", pushedThreadState.sp);
        Genode::printf("Register r0: %i\n", pushedThreadState.r0);
        Genode::printf("Register r1: %i\n", pushedThreadState.r1);
        Genode::printf("Register r2: %i\n", pushedThreadState.r2);
        Genode::printf("Register r3: %i\n", pushedThreadState.r3);
        Genode::printf("Register r4: %i\n", pushedThreadState.r4);
        Genode::printf("Register r5: %i\n", pushedThreadState.r5);
        Genode::printf("Register r6: %i\n", pushedThreadState.r6);
        Genode::printf("Register r7: %i\n", pushedThreadState.r7);
        Genode::printf("Register r8: %i\n", pushedThreadState.r8);
        Genode::printf("Register r9: %i\n", pushedThreadState.r9);
        Genode::printf("Register r10: %i\n", pushedThreadState.r10);
        Genode::printf("Register r11: %i\n", pushedThreadState.r11);
        Genode::printf("Register r12: %i\n", pushedThreadState.r12);
        Genode::printf("Register lr: %i\n", pushedThreadState.lr);
        Genode::printf("Register cpsr: %i\n", pushedThreadState.cpsr);

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
    ChildProcess* child = _children.first();
    while(child != NULL)
    {
      if (child->getId() == process) {

        pushedThreadState = launcher.thread_state(child);

        Genode::printf("Register ip: %i\n", pushedThreadState.ip);
        Genode::printf("Register sp: %i\n", pushedThreadState.sp);
        Genode::printf("Register r0: %i\n", pushedThreadState.r0);
        Genode::printf("Register r1: %i\n", pushedThreadState.r1);
        Genode::printf("Register r2: %i\n", pushedThreadState.r2);
        Genode::printf("Register r3: %i\n", pushedThreadState.r3);
        Genode::printf("Register r4: %i\n", pushedThreadState.r4);
        Genode::printf("Register r5: %i\n", pushedThreadState.r5);
        Genode::printf("Register r6: %i\n", pushedThreadState.r6);
        Genode::printf("Register r7: %i\n", pushedThreadState.r7);
        Genode::printf("Register r8: %i\n", pushedThreadState.r8);
        Genode::printf("Register r9: %i\n", pushedThreadState.r9);
        Genode::printf("Register r10: %i\n", pushedThreadState.r10);
        Genode::printf("Register r11: %i\n", pushedThreadState.r11);
        Genode::printf("Register r12: %i\n", pushedThreadState.r12);
        Genode::printf("Register lr: %i\n", pushedThreadState.lr);
        Genode::printf("Register cpsr: %i\n", pushedThreadState.cpsr);

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
