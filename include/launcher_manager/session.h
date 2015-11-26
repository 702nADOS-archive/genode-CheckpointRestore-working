#include <session/session.h>
#include <base/rpc.h>

namespace LauncherManager {
  class Session : public Genode::Session
  {
  public:
    // typedef Genode::String<64> String;
    typedef Genode::Rpc_in_buffer<24> String;

    static const char *service_name() { return "LauncherManager"; }
    virtual void say_hello() = 0;

    virtual int create(String& filename, unsigned int ram_quota) = 0;
    virtual void kill(int process) = 0;
    virtual void pause(int process) = 0;
    virtual void resume(int process) = 0;

    // Register functions
    GENODE_RPC(Rpc_create, int, create, String&, unsigned int);
    GENODE_RPC(Rpc_kill, void, kill, int);
    GENODE_RPC(Rpc_pause, void, pause, int);
    GENODE_RPC(Rpc_resume, void, resume, int);

    GENODE_RPC(Rpc_say_hello, void, say_hello);

    // adds all functions to the interface
    GENODE_RPC_INTERFACE(Rpc_say_hello, Rpc_kill, Rpc_pause, Rpc_resume);
  };
}
