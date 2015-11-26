#include <base/printf.h>
#include <root/component.h>

#include "session_component.h"

namespace LauncherManager {

  class Root_component : public Genode::Root_component<Session_component>
  {
    protected:

      Session_component *_create_session(const char *args);

    public:

      Root_component(Genode::Rpc_entrypoint *ep, Genode::Allocator      *allocator);
  };
}
