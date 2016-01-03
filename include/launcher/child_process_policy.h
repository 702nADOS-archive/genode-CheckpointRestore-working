#ifndef _INCLUDE__CHILD_PROCESS_POLICY_H_
#define _INCLUDE__CHILD_PROCESS_POLICY_H_

#include <base/allocator.h>
#include <base/service.h>
#include <base/printf.h>
#include <base/lock.h>
#include <cap_session/connection.h>
#include <init/child.h>

namespace Launcher
{
  class ChildProcessPolicy :  public Genode::Child_policy,
                              public Genode::Client
  {
    typedef Genode::String<64> Name;
    Name const _name;

    Genode::Server                      *_server;
    Genode::Service_registry            *_parent_services;
    Genode::Service_registry            *_child_services;
    Genode::Dataspace_capability         _config_ds;
    Genode::Rpc_entrypoint              *_parent_entrypoint;
    Init::Child_policy_enforce_labeling  _labeling_policy;
    Init::Child_policy_provide_rom_file  _config_policy;
    Init::Child_policy_provide_rom_file  _binary_policy;

  public:
    ChildProcessPolicy(const char                  *name,
                           Genode::Server              *server,
                           Genode::Service_registry    *parent_services,
                           Genode::Service_registry    *child_services,
                           Genode::Dataspace_capability config_ds,
                           Genode::Dataspace_capability binary_ds,
                           Genode::Rpc_entrypoint      *parent_entrypoint);

    const char* name() const;
    void unregister_services();
    bool announce_service(const char              *service_name,
                          Genode::Root_capability  root,
                          Genode::Allocator       *alloc,
                          Genode::Server          * /*server*/);
    void filter_session_args(const char *service, char *args,
                             Genode::size_t args_len);
    Genode::Service *resolve_session_request(const char *service_name,
                                            const char *args);
  };
}

#endif
