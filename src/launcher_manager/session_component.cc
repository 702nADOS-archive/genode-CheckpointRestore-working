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

  int ip = 0, sp = 0, r0 = 0, r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, lr = 0, cpsr = 0;

  void Session_component::pushThreadState(int process)
  {
    ChildProcess* child = _children.first();
    while(child != NULL)
    {
      if (child->getId() == process) {
        pushedThreadState = launcher.thread_state(child);
        ip = pushedThreadState.ip;
        Genode::printf("Register ip (push): %i\n", pushedThreadState.ip);
        sp = pushedThreadState.sp;
        Genode::printf("Register sp (push): %i\n", pushedThreadState.sp);
        r0 = pushedThreadState.r0;
        Genode::printf("Register r0 (push): %i\n", pushedThreadState.r0);
        r1 = pushedThreadState.r1;
        Genode::printf("Register r1 (push): %i\n", pushedThreadState.r1);
        r2 = pushedThreadState.r2;
        Genode::printf("Register r2 (push): %i\n", pushedThreadState.r2);
        r3 = pushedThreadState.r3;
        Genode::printf("Register r3 (push): %i\n", pushedThreadState.r3);
        r4 = pushedThreadState.r4;
        Genode::printf("Register r4 (push): %i\n", pushedThreadState.r4);
        r5 = pushedThreadState.r5;
        Genode::printf("Register r5 (push): %i\n", pushedThreadState.r5);
        r6 = pushedThreadState.r6;
        Genode::printf("Register r6 (push): %i\n", pushedThreadState.r6);
        r7 = pushedThreadState.r7;
        Genode::printf("Register r7 (push): %i\n", pushedThreadState.r7);
        r8 = pushedThreadState.r8;
        Genode::printf("Register r8 (push): %i\n", pushedThreadState.r8);
        r9 = pushedThreadState.r9;
        Genode::printf("Register r9 (push): %i\n", pushedThreadState.r9);
        r10 = pushedThreadState.r10;
        Genode::printf("Register r10 (push): %i\n", pushedThreadState.r10);
        r11 = pushedThreadState.r11;
        Genode::printf("Register r11 (push): %i\n", pushedThreadState.r11);
        r12 = pushedThreadState.r12;
        Genode::printf("Register r12 (push): %i\n", pushedThreadState.r12);
        lr = pushedThreadState.lr;
        Genode::printf("Register lr (push): %i\n", pushedThreadState.lr);
        cpsr = pushedThreadState.cpsr;
        Genode::printf("Register cpsr (push): %i\n", pushedThreadState.cpsr);

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
        pushedThreadState.ip = ip;
        Genode::printf("Register ip (pop): %i\n", pushedThreadState.ip);
        pushedThreadState.ip = sp;
        Genode::printf("Register sp (pop): %i\n", pushedThreadState.sp);
        pushedThreadState.ip = r0;
        Genode::printf("Register r0 (pop): %i\n", pushedThreadState.r0);
        pushedThreadState.ip = r1;
        Genode::printf("Register r1 (pop): %i\n", pushedThreadState.r1);
        pushedThreadState.ip = r2;
        Genode::printf("Register r2 (pop): %i\n", pushedThreadState.r2);
        pushedThreadState.ip = r3;
        Genode::printf("Register r3 (pop): %i\n", pushedThreadState.r3);
        pushedThreadState.ip = r4;
        Genode::printf("Register r4 (pop): %i\n", pushedThreadState.r4);
        pushedThreadState.ip = r5;
        Genode::printf("Register r5 (pop): %i\n", pushedThreadState.r5);
        pushedThreadState.ip = r6;
        Genode::printf("Register r6 (pop): %i\n", pushedThreadState.r6);
        pushedThreadState.ip = r7;
        Genode::printf("Register r7 (pop): %i\n", pushedThreadState.r7);
        pushedThreadState.ip = r8;
        Genode::printf("Register r8 (pop): %i\n", pushedThreadState.r8);
        pushedThreadState.ip = r9;
        Genode::printf("Register r9 (pop): %i\n", pushedThreadState.r9);
        pushedThreadState.ip = r10;
        Genode::printf("Register r10 (pop): %i\n", pushedThreadState.r10);
        pushedThreadState.ip = r11;
        Genode::printf("Register r11 (pop): %i\n", pushedThreadState.r11);
        pushedThreadState.ip = r12;
        Genode::printf("Register r12 (pop): %i\n", pushedThreadState.r12);
        pushedThreadState.ip = lr;
        Genode::printf("Register lr (pop): %i\n", pushedThreadState.lr);
        pushedThreadState.ip = cpsr;
        Genode::printf("Register cpsr (pop): %i\n", pushedThreadState.cpsr);


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
