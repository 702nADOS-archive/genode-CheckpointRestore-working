#include <timer_session/connection.h>
class ChildProcess;

void destruct_child(Genode::Allocator *alloc, ChildProcess *child,
                           Timer::Session *timer, int timeout);
