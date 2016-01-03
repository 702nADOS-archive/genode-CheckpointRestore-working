#include <services/rm_session_component.h>

namespace Launcher
{

Rm_session_component::Rm_session_component(//Dataspace_registry & ds_registry,
                     Genode::addr_t start, Genode::size_t size)
:
  _rm(start, size)//, _ds_registry(ds_registry)
{ }

Rm_session_component::~Rm_session_component()
{
  // Region *curr;
  // while ((curr = _regions.first()))
  // 	detach(curr->local_addr);
}

Rm_session_component::Local_addr Rm_session_component::attach(Genode::Dataspace_capability ds,
                          Genode::size_t size, Genode::off_t offset,
                          bool use_local_addr,
                          Local_addr local_addr,
                          bool executable)
{
  return _rm.attach(ds, size, offset, use_local_addr, local_addr, executable);
}

Rm_session_component::Local_addr Rm_session_component::attach_at(Genode::Dataspace_capability ds, Genode::addr_t local_addr,
                     Genode::size_t size, Genode::off_t offset)
{
  return attach(ds, size, offset, true, local_addr);
}

Rm_session_component::Local_addr Rm_session_component::attach_executable(Genode::Dataspace_capability ds, Genode::addr_t local_addr,
                             Genode::size_t size, Genode::off_t offset)
{
  return attach(ds, size, offset, true, local_addr, true);
}

void Rm_session_component::detach(Local_addr local_addr)
{
  _rm.detach(local_addr);
}

Genode::Pager_capability Rm_session_component::add_client(Genode::Thread_capability thread)
{
  return _rm.add_client(thread);
}

void Rm_session_component::remove_client(Genode::Pager_capability p_cap)
{
  _rm.remove_client(p_cap);
}

void Rm_session_component::fault_handler(Genode::Signal_context_capability handler)
{
  _rm.fault_handler(handler);
}

Rm_session_component::State Rm_session_component::state()
{
  return _rm.state();
}

/**
 * Return dataspace representation of region-manager session
 */
Genode::Dataspace_capability Rm_session_component::dataspace()
{
  return _rm.dataspace();
}

}
