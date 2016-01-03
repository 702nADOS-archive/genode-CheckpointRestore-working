#ifndef _INCLUDE__LAUNCHER_MANAGER_RM_SESSION_COMPONENT_H_
#define _INCLUDE__LAUNCHER_MANAGER_RM_SESSION_COMPONENT_H_

#include <rm_session/connection.h>
#include <base/rpc_server.h>

namespace LauncherManager
{
  class Rm_session_component : public Genode::Rpc_object<Genode::Rm_session>
  {
  private:
    Genode::Rm_connection _rm;
  public:
    Rm_session_component(//Dataspace_registry & ds_registry,
		                     Genode::addr_t start = ~0UL, Genode::size_t size = 0);

    ~Rm_session_component();

    /**
     * Map dataspace into local address space
     *
     * \param ds               capability of dataspace to map
     * \param size             size of the locally mapped region
     *                         default (0) is the whole dataspace
     * \param offset           start at offset in dataspace (page-aligned)
     * \param use_local_addr   if set to true, attach the dataspace at
     *                         the specified 'local_addr'
     * \param local_addr       local destination address
     * \param executable       if the mapping should be executable
     *
     * \throw Attach_failed    if dataspace or offset is invalid,
     *                         or on region conflict
     * \throw Out_of_metadata  if meta-data backing store is exhausted
     *
     * \return                 local address of mapped dataspace
     *
     */
    Local_addr attach(Genode::Dataspace_capability ds,
                              Genode::size_t size = 0, Genode::off_t offset = 0,
                              bool use_local_addr = false,
                              Local_addr local_addr = (void *)0,
                              bool executable = false);

    /**
     * Shortcut for attaching a dataspace at a predefined local address
     */
    Local_addr attach_at(Genode::Dataspace_capability ds, Genode::addr_t local_addr,
                         Genode::size_t size = 0, Genode::off_t offset = 0);

    /**
     * Shortcut for attaching a dataspace executable at a predefined local address
     */
    Local_addr attach_executable(Genode::Dataspace_capability ds, Genode::addr_t local_addr,
                                 Genode::size_t size = 0, Genode::off_t offset = 0);

    /**
     * Remove region from local address space
     */
    void detach(Local_addr local_addr);

    /**
     * Add client to pager
     *
     * \param thread  thread that will be paged
     * \throw         Invalid_thread
     * \throw         Out_of_metadata
     * \throw         Unbound_thread
     * \return        capability to be used for handling page faults
     *
     * This method must be called at least once to establish a valid
     * communication channel between the pager part of the region manager
     * and the client thread.
     */
    Genode::Pager_capability add_client(Genode::Thread_capability thread);

    /**
     * Remove client from pager
     *
     * \param pager  pager capability of client to be removed
     */
    void remove_client(Genode::Pager_capability p_cap);

    /**
     * Register signal handler for region-manager faults
     *
     * On Linux, this signal is never delivered because page-fault handling
     * is performed by the Linux kernel. On microkernel platforms,
     * unresolvable page faults (traditionally called segmentation fault)
     * will result in the delivery of the signal.
     */
    void fault_handler(Genode::Signal_context_capability handler);

    /**
     * Request current state of RM session
     */
    State state();

    /**
     * Return dataspace representation of region-manager session
     */
    Genode::Dataspace_capability dataspace();

  };
}

#endif
