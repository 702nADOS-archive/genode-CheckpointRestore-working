#include <dataspace/capability.h>
#include <base/env.h>

#include <rm_registry.h>

Rm_Registry::~Rm_Registry()
{
  /*
   * At the time the destructor is called, most 'Dataspace_info'
   * objects are expected to be gone already because
   * 'Child::_resources' and 'Child::_child' are destructed
   * before the 'Child::_ds_registry'. However, RM dataspaces
   * created via 'Rm_dataspace_info::fork', are not handled by
   * those destructors. So we have to clean them up here.
   */
  while(Rm_Info *info = _pool.first())
  {
    _pool.remove_locked(info);
    destroy(Genode::env()->heap(), info);
  }
}

void Rm_Registry::insert(Rm_Info *info)
{
    _pool.insert(info);
}

void Rm_Registry::remove(Rm_Info *info)
{
  _pool.remove_locked(info);
}

Rm_Info *Rm_Registry::lookup_info(Genode::Dataspace_capability ds_cap)
{
  return _pool.lookup_and_lock(ds_cap);
}
