/*
 *  Copyright 2010-2014 Fabric Software Inc. All rights reserved.
 */

#include "conversion.h"
#include <FabricEDK.h>
#include "ovrDevice_functions.h"
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>

using namespace Fabric::EDK;

typedef boost::shared_mutex Lock;
typedef boost::unique_lock< Lock >  WriteLock;
typedef boost::shared_lock< Lock >  ReadLock;
Lock gLock;

void ContextCallback(bool opening, void const *contextPtr)
{
  if ( opening )
  {
    WriteLock w_lock(gLock);
    printf("Oculus Rift starting...\n");
    OVR::System::Init();
    ovr_Initialize(NULL);
    printf("Oculus Rift initialized.\n");
  }
  else
  {
    WriteLock w_lock(gLock);
    ovr_Shutdown();
    OVR::System::Destroy();
    printf("Oculus Rift finalized.\n");
  }
}
IMPLEMENT_FABRIC_EDK_ENTRIES_WITH_CONTEXT_CALLBACK( Oculus, &ContextCallback )

// Defined at src\ovrDevice.kl:21:1
FABRIC_EXT_EXPORT void ovrDevice_Construct(
  KL::Traits< KL::ovrDevice >::IOParam this_,
  KL::Traits< KL::UInt32 >::INParam index
) {

  WriteLock w_lock(gLock);

  ovrHmd HMD;
  ovrGraphicsLuid luid;
  ovrResult result = ovr_Create(&HMD, &luid);
  if(!OVR_SUCCESS(result))
    return;

  this_->handle = HMD;
  if(!this_->handle)
    return;

  ovrHmdDesc hmdDesc = ovr_GetHmdDesc(HMD);
  ovrSizei windowSize = { hmdDesc.Resolution.w / 2, hmdDesc.Resolution.h / 2 };

  this_->configured = false;
  this_->stereoEnabled = true;
}

// Defined at src\ovrDevice.kl:28:1
FABRIC_EXT_EXPORT void ovrDevice_Destruct(
  KL::Traits< KL::ovrDevice >::IOParam this_
) {

  WriteLock w_lock(gLock);

  if(this_->handle)
  {
    ovr_Destroy((ovrHmd)this_->handle);
  }
}

// Defined at src\ovrDevice.kl:32:1
FABRIC_EXT_EXPORT void ovrDevice_GetDescription(
  KL::Traits< KL::ovrDescription >::Result _result,
  KL::Traits< KL::ovrDevice >::INParam this_
) {

  if(!this_->handle)
    return;
  ovrHmd hmd = (ovrHmd)this_->handle;
  ovrHmdDesc desc = ovr_GetHmdDesc(hmd);
  convert(desc, _result);
}

// Defined at src\\ovrDevice.kl:41:1
FABRIC_EXT_EXPORT void ovrDevice_GetLastError(
  KL::Traits< KL::String >::Result _result,
  KL::Traits< KL::ovrDevice >::INParam this_
) {
  ovrErrorInfo errorInfo;
  ovr_GetLastErrorInfo(&errorInfo);
  _result = errorInfo.ErrorString;
}

// Defined at src\\ovrDevice.kl:44:1
FABRIC_EXT_EXPORT void ovrDevice_GetVersionString(
  KL::Traits< KL::String >::Result _result,
  KL::Traits< KL::ovrDevice >::INParam this_
) {
  _result = ovr_GetVersionString();
}

// Defined at src\\ovrDevice.kl:47:1
FABRIC_EXT_EXPORT void ovrDevice_TraceMessage(
  KL::Traits< KL::ovrDevice >::INParam this_,
  KL::Traits< KL::SInt32 >::INParam level,
  KL::Traits< KL::String >::INParam message
) {
  ovr_TraceMessage(level, message.getCString());
}

// Defined at src\\ovrDevice.kl:50:1
FABRIC_EXT_EXPORT KL::SInt32 ovrDevice_GetEnabledCaps(
  KL::Traits< KL::ovrDevice >::INParam this_
) {
  if(!this_->handle)
    return 0;
  ovrHmd hmd = (ovrHmd)this_->handle;
  return ovr_GetEnabledCaps(hmd);  
}

// Defined at src\\ovrDevice.kl:54:1
FABRIC_EXT_EXPORT void ovrDevice_SetEnabledCaps(
  KL::Traits< KL::ovrDevice >::INParam this_,
  KL::Traits< KL::SInt32 >::INParam caps
) {
  if(!this_->handle)
    return;
  ovrHmd hmd = (ovrHmd)this_->handle;
  ovr_SetEnabledCaps(hmd, caps);
}
