/*
 *  Copyright 2010-2014 Fabric Software Inc. All rights reserved.
 */

#include "conversion.h"
#include <FabricEDK.h>
#include "ovrSession_functions.h"

using namespace Fabric::EDK;

// Defined at src\\ovrSession.kl:9:1
FABRIC_EXT_EXPORT void ovrSession_Construct(
  KL::Traits< KL::ovrSession >::IOParam this_
) {
  ovrSession * session = new ovrSession();
  ovrGraphicsLuid luid;
  ovrResult result = ovr_Create(session, &luid);
  if(!OVR_SUCCESS(result))
    delete(session);
  else
    this_->handle = session;
}

// Defined at src\\ovrSession.kl:10:1
FABRIC_EXT_EXPORT void ovrSession_Destruct(
  KL::Traits< KL::ovrSession >::IOParam this_
) {
  if(!this_->handle)
    return;
  ovrSession * hmd = (ovrSession*)this_->handle;
  ovr_Destroy(*hmd);
  delete(hmd);
}

// Defined at src\\ovrSession.kl:20:1
FABRIC_EXT_EXPORT KL::Boolean ovrSession_getSessionStatus(
  KL::Traits< KL::ovrSession >::INParam this_,
  KL::Traits< KL::ovrSessionStatus >::IOParam sessionStatus
) {
  if(!this_->handle)
    return false;
  ovrSession * hmd = (ovrSession*)this_->handle;
  ovrSessionStatus sessionStatus_;
  ovr_GetSessionStatus(*hmd, &sessionStatus_);
  convert(sessionStatus_, sessionStatus);
  return true;
}

// Defined at src\\ovrSession.kl:31:1
FABRIC_EXT_EXPORT KL::SInt32 ovrSession_getEnabledCaps(
  KL::Traits< KL::ovrSession >::INParam this_
) {
  if(!this_->handle)
    return 0;
  ovrSession * hmd = (ovrSession*)this_->handle;
  return (int)ovr_GetEnabledCaps(*hmd);
}

// Defined at src\\ovrSession.kl:42:1
FABRIC_EXT_EXPORT void ovrSession_setEnabledCaps(
  KL::Traits< KL::ovrSession >::IOParam this_,
  KL::Traits< KL::SInt32 >::INParam hmdCaps
) {
  if(!this_->handle)
    return;
  ovrSession * hmd = (ovrSession*)this_->handle;
  ovr_SetEnabledCaps(*hmd, (unsigned int)hmdCaps);
}
