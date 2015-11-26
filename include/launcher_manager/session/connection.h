#ifndef _INCLUDE__LAUNCHER_MANAGER_CONNECTION_H_
#define _INCLUDE__LAUNCHER_MANAGER_CONNECTION_H_

#include <launcher_manager/session/client.h>
#include <base/connection.h>

namespace LauncherManager {

  struct Connection : Genode::Connection<Session>, Session_client
  {
    Connection()
    :
      /* create session */
      Genode::Connection<LauncherManager::Session>(session("foo, ram_quota=4K")),

      /* initialize RPC interface */
      Session_client(cap()) { }
  };
}
#endif
