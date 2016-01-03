#ifndef _INCLUDE__LAUNCHER_MANAGER_RM_REGISTRY_H_
#define _INCLUDE__LAUNCHER_MANAGER_RM_REGISTRY_H_

#include <base/object_pool.h>
#include <rm_info.h>
#include <dataspace/capability.h>

class Rm_Registry
{
private:
  Genode::Object_pool<Rm_Info> _pool;

public:
  ~Rm_Registry();
  void insert(Rm_Info *info);
  void remove(Rm_Info *info);
  Rm_Info *lookup_info(Genode::Dataspace_capability ds_cap);
};

#endif
