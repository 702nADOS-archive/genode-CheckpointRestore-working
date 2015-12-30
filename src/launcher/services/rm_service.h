/* Genode includes */
#include <base/service.h>

#include <rm_registry.h>

//Local Services
#include <services/rm_session_component.h>


#ifndef _INCLUDE__LAUNCHER_MANAGER_RM_SERVICE_H_
#define _INCLUDE__LAUNCHER_MANAGER_RM_SERVICE_H_

namespace LauncherManager
{

  class Rm_Service : public Genode::Service
  {
  private:
    Genode::Rpc_entrypoint _ep;

  public:
    Rm_Service(Genode::Rpc_entrypoint &ep)//, Dataspace_registry &ds_registry)
    :
      Service(Genode::Rm_session::service_name()), _ep(ep)
      //,_ds_registry(ds_registry)
    { }

    Genode::Session_capability session(const char *args, Genode::Affinity const &)
    {
      Genode::addr_t start = Genode::Arg_string::find_arg(args, "start").ulong_value(~0UL);
      Genode::size_t size  = Genode::Arg_string::find_arg(args, "size").ulong_value(0);

      // Rm_session_component *rm = new Rm_session_component(start, size);

      // Rm_session_component *rm =
          // new Rm_session_component(_ds_registry, start, size);
      //
      // Rm_dataspace_info *info = new Rm_dataspace_info(rm, _ep);
      // _ds_registry.insert(info);
      // return info->rm_cap();
      PERR("Should return RM_CAP: %i - %i", start, size);
      // return Genode::Session_capability();

      return _ep.manage(new (Genode::env()->heap()) Rm_session_component(start, size));
    }

    void upgrade(Genode::Session_capability, const char *args) { }

    void close(Genode::Session_capability session)
    {
      Rm_session_component * rm_session =
        dynamic_cast<Rm_session_component *>(_ep.lookup_and_lock(session));
      if (!rm_session) {
        PERR("Unexpected call of close with non-RM-session argument");
        return;
      }
      //
      // /* use RM dataspace as key to obtain the dataspace info object */
      // Dataspace_capability ds_cap = rm_session->dataspace();
      //
      // /* release dataspace info */
      // Dataspace_info *info = _ds_registry.lookup_info(ds_cap);
      // if (!info) {
      //   PWRN("Could not lookup dataspace info for local RM session");
      //   return;
      // }
      //
      // _ds_registry.remove(info);
      //
      // info->dissolve_users();
      //
      // /* 'rm_session' is deleted by deleting Rm_dataspace_info 'info' */
      // destroy(env()->heap(), info);
      _ep.dissolve(rm_session);
      destroy(Genode::env()->heap(), rm_session);
      PERR("SHOULD RELEASE RM_CAP");
    }
  };
}
#endif
