/*
 * Distributed under OSI-approved BSD 3-Clause License. See
 * accompanying License.txt
 */

#ifndef catalyst_stub_h
#define catalyst_stub_h

#include "catalyst_api.h"

#ifdef __cplusplus
extern "C"
{
#endif

  // This file provides functions to use when implementing custom Catalyst
  // implementation if you want to forward calls to the stub
  // implementation.

  CATALYST_EXPORT enum catalyst_status catalyst_stub_initialize(const conduit_node* params);
  CATALYST_EXPORT enum catalyst_status catalyst_stub_finalize(const conduit_node* params);
  CATALYST_EXPORT enum catalyst_status catalyst_stub_execute(const conduit_node* params);
  CATALYST_EXPORT enum catalyst_status catalyst_stub_about(conduit_node* params);
  CATALYST_EXPORT enum catalyst_status catalyst_stub_results(conduit_node* params);

#ifdef __cplusplus
}
#endif

#endif
