/*
 * Distributed under OSI-approved BSD 3-Clause License. See
 * accompanying License.txt
 */

#ifndef catalyst_impl_h
#define catalyst_impl_h

#include <catalyst_api.h>

// include mangled conduit C API.
#include <conduit.h>

#ifdef __cplusplus
extern "C"
{
#endif

  struct catalyst_impl
  {
    int version;
    enum catalyst_status (*initialize)(const conduit_node*);
    enum catalyst_status (*execute)(const conduit_node*);
    enum catalyst_status (*finalize)(const conduit_node*);
    enum catalyst_status (*about)(conduit_node*);
    enum catalyst_status (*results)(conduit_node*);
  };

#ifdef __cplusplus
} // extern "C"
#endif

#endif // #ifndef catalyst_impl_h
