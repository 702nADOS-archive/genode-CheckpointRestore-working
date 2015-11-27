#ifndef _INCLUDE__LAUNCHER_MANAGER_SESSION_COMPONENT_H_
#define _INCLUDE__LAUNCHER_MANAGER_SESSION_COMPONENT_H_

#include <base/rpc_server.h>
#include <launcher_manager/session.h>
#include <launcher/launcher.h>

class ChildProcess;

namespace LauncherManager {
  class Session_component : public Genode::Rpc_object<LauncherManager::Session>
  {
    public:
      Session_component();
      virtual ~Session_component();
      int createChild(String const & filename, unsigned int ram_quota);
      void kill(int process);
      void pause(int process);
      void resume(int process);

      void pushThreadState(int process);
      void popThreadState(int process);

      void say_hello();
    protected:
      Launcher launcher;

      Genode::List<ChildProcess> _children;
      int _nextId;

      //pushed thread state
      Genode::Thread_state pushedThreadState;
  };
}
#endif
