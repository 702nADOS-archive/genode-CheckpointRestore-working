#ifndef _INCLUDE__LAUNCHER_MANAGER_CLIENT_H_
#define _INCLUDE__LAUNCHER_MANAGER_CLIENT_H_

#include <launcher_manager/session.h>
#include <base/rpc_client.h>
#include <base/printf.h>

namespace LauncherManager {
  class Session_client : public Genode::Rpc_client<Session>
  {
    public:
    Session_client(Genode::Capability<Session> cap)
    : Genode::Rpc_client<Session>(cap)
    {
    }

    int create(String const & filename, unsigned int ram_quota)
    {
      return call<Rpc_create>(filename, ram_quota);
    }

    void kill(int process)
    {
      call<Rpc_kill>(process);
    }

    void pause(int process)
    {
      call<Rpc_pause>(process);
    }

    void resume(int process)
    {
      call<Rpc_resume>(process);
    }

    void say_hello()
    {
      PDBG("Saying Hello.");
      call<Rpc_say_hello>();
    }
  };
}
#endif
