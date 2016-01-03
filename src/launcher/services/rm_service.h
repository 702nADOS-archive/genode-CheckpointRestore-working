#ifndef _INCLUDE__LAUNCHER_MANAGER_RM_SERVICE_H_
#define _INCLUDE__LAUNCHER_MANAGER_RM_SERVICE_H_

/* Genode includes */
#include <base/service.h>
#include <base/rpc_server.h>

namespace LauncherManager
{

  class Rm_Service : public Genode::Service
  {
  private:
    Genode::Rpc_entrypoint _ep;

  public:
    Rm_Service(Genode::Rpc_entrypoint &ep);

    Genode::Session_capability session(const char *args, Genode::Affinity const &);

    void upgrade(Genode::Session_capability, const char *args);

    void close(Genode::Session_capability session);
  };
}
#endif
