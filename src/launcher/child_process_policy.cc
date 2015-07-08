#include <child_process_policy.h>

ChildProcessPolicy::ChildProcessPolicy(const char                  *name,
                       Genode::Server              *server,
                       Genode::Service_registry    *parent_services,
                       Genode::Service_registry    *child_services,
                       Genode::Dataspace_capability config_ds,
                       Genode::Dataspace_capability binary_ds,
                       Genode::Rpc_entrypoint      *parent_entrypoint)
:
  _name(name),
  _server(server),
  _parent_services(parent_services),
  _child_services(child_services),
  _config_ds(config_ds),
  _parent_entrypoint(parent_entrypoint),
  _labeling_policy(_name.string()),
  _config_policy("config", config_ds, _parent_entrypoint),
  _binary_policy("binary", binary_ds, _parent_entrypoint)
{

}

const char* ChildProcessPolicy::name(void) const
{
  return _name.string();
}

Genode::Service *ChildProcessPolicy::resolve_session_request(const char *service_name,
                                         const char *args)
{
  Genode::printf("Start resolving stuff: %s %s\n", service_name, args);
  Genode::Service *service;

  /* check for config file request */
  if ((service = _config_policy.resolve_session_request(service_name, args)))
    return service;

  /* check for binary file request */
  if ((service = _binary_policy.resolve_session_request(service_name, args)))
    return service;

  /* if service is provided by one of our children, use it */
  if ((service = _child_services->find(service_name)))
    return service;

  // /*
  //  * Handle special case of the demo scenario when the user uses
  //  * a nested Launchad for starting another Nitpicker instance
  //  * before starting Liquid_fb. In this case, we do not want to
  //  * delegate Nitpicker's session requests to the parent. The
  //  * parent may be a launchpad again trying to apply the same
  //  * policy. This instance will recognise that the session is not
  //  * available at init (because the actual input and framebuffer
  //  * drivers are singletons, and would therefore block for
  //  * Framebuffer or Input to become available at one of its own
  //  * children. The launchpad with which the user originally
  //  * interacted, however, would block at the parent interface
  //  * until this condition gets satisfied.
  //  */
  // if (Genode::strcmp(service_name, "Input") != 0
  //  && Genode::strcmp(service_name, "Framebuffer") != 0
  //  && (service = _parent_services->find(service_name)))
  //   return service;



  if ((service = _parent_services->find(service_name)))
    return service;

  // Genode::Client client;
  // service = _parent_services->wait_for_service(service_name, &client, name());
  // if (service == 0)
  //   Genode::printf("Waiting was canceled: %s %s\n", service_name, args);
  // return service;


  // /* wait for the service to become available */
  Genode::Client client;
  return _child_services->wait_for_service(service_name,
                                           &client, name());
}

void ChildProcessPolicy::filter_session_args(const char *service, char *args,
                         Genode::size_t args_len)
{
  _labeling_policy.filter_session_args(service, args, args_len);
}

bool ChildProcessPolicy::announce_service(const char              *service_name,
                      Genode::Root_capability  root,
                      Genode::Allocator       *alloc,
                      Genode::Server          * /*server*/)
{
  if (_child_services->find(service_name)) {
    PWRN("%s: service %s is already registered",
         name(), service_name);
    return false;
  }

  /* XXX remove potential race between checking for and inserting service */

  _child_services->insert(new (alloc)
    Genode::Child_service(service_name, root, _server));
  Genode::printf("%s registered service %s\n", name(), service_name);
  return true;
}

void ChildProcessPolicy::unregister_services()
{
  Genode::Service *rs;
  while ((rs = _child_services->find_by_server(_server)))
    _child_services->remove(rs);
}
