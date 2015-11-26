#include "root.h"

namespace LauncherManager {
  Session_component *Root_component::_create_session(const char *args)
  {
    PDBG("creating LauncherManager session.");
    return new (md_alloc()) Session_component();
  }

  Root_component::Root_component(Genode::Rpc_entrypoint *ep,
                 Genode::Allocator      *allocator)
  : Genode::Root_component<Session_component>(ep, allocator)
  {
    PDBG("Creating LauncherManager root component.");
  }
}
