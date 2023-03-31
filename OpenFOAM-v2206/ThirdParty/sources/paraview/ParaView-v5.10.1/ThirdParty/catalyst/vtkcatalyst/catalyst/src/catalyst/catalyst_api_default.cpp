/*
 * Distributed under OSI-approved BSD 3-Clause License. See
 * accompanying License.txt
 */

#include "catalyst_impl.h"
#include "catalyst_stub.h"

extern "C" struct catalyst_impl const default_impl = { 1, catalyst_stub_initialize,
  catalyst_stub_execute, catalyst_stub_finalize, catalyst_stub_about, catalyst_stub_results };
