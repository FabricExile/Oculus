/*
 *  Copyright 2010-2014 Fabric Software Inc. All rights reserved.
 */

#include "conversion.h"
#include <FabricEDK.h>
#include "ovrDevice_functions.h"
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>

#include <vector>

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
  KL::Traits< KL::ovrDevice >::IOParam this_,
  KL::Traits< KL::SInt32 >::INParam level,
  KL::Traits< KL::String >::INParam message
) {
  ovr_TraceMessage(level, message.getCString());
  printf("ovrDevice_TraceMessage: '%s'\n", message.getCString());
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
  KL::Traits< KL::ovrDevice >::IOParam this_,
  KL::Traits< KL::SInt32 >::INParam caps
) {
  if(!this_->handle)
    return;
  ovrHmd hmd = (ovrHmd)this_->handle;
  ovr_SetEnabledCaps(hmd, caps);
}

// Defined at src\\ovrDevice.kl:57:1
FABRIC_EXT_EXPORT KL::Boolean ovrDevice_ConfigureTracking(
  KL::Traits< KL::ovrDevice >::IOParam this_,
  KL::Traits< KL::SInt32 >::INParam requestedTrackingCaps,
  KL::Traits< KL::SInt32 >::INParam requiredTrackingCaps
) {
  if(!this_->handle)
    return false;
  ovrHmd hmd = (ovrHmd)this_->handle;
  printf("ovrDevice_ConfigureTracking: '%d' '%d'\n", requestedTrackingCaps, requiredTrackingCaps);
  return OVR_SUCCESS(ovr_ConfigureTracking(hmd, requestedTrackingCaps, requiredTrackingCaps));
}

// Defined at src\\ovrDevice.kl:68:1
FABRIC_EXT_EXPORT void ovrDevice_RecenterPose(
  KL::Traits< KL::ovrDevice >::IOParam this_
) {
  if(!this_->handle)
    return;
  ovrHmd hmd = (ovrHmd)this_->handle;
  ovr_RecenterPose(hmd);
  printf("ovr_RecenterPose\n");
}

// Defined at src\\ovrDevice.kl:76:1
FABRIC_EXT_EXPORT void ovrDevice_GetTrackingState(
  KL::Traits< KL::ovrTrackingState >::Result _result,
  KL::Traits< KL::ovrDevice >::INParam this_,
  KL::Traits< KL::Float64 >::INParam absTime
) {
  if(!this_->handle)
    return;
  ovrHmd hmd = (ovrHmd)this_->handle;
  ovrTrackingState state = ovr_GetTrackingState(hmd, absTime);
  convert(state, _result);
}

// Defined at src\\ovrDevice.kl:80:1
FABRIC_EXT_EXPORT KL::Boolean ovrDevice_GetInputState(
  KL::Traits< KL::ovrDevice >::INParam this_,
  KL::Traits< KL::SInt32 >::INParam controllerTypeMask,
  KL::Traits< KL::ovrInputState >::IOParam state
) {
  if(!this_->handle)
    return false;
  ovrHmd hmd = (ovrHmd)this_->handle;
  ovrInputState _state;
  bool result = OVR_SUCCESS(ovr_GetInputState(hmd, controllerTypeMask, &_state));
  if(result)
    convert(_state, state);
  return result;
}

// Defined at src\\ovrDevice.kl:87:1
FABRIC_EXT_EXPORT KL::Boolean ovrDevice_SetControllerVibration(
  KL::Traits< KL::ovrDevice >::IOParam this_,
  KL::Traits< KL::SInt32 >::INParam controllerTypeMask,
  KL::Traits< KL::Float32 >::INParam frequency,
  KL::Traits< KL::Float32 >::INParam amplitude
) {
  if(!this_->handle)
    return false;
  ovrHmd hmd = (ovrHmd)this_->handle;
  return OVR_SUCCESS(ovr_SetControllerVibration(hmd, controllerTypeMask, frequency, amplitude));
}

// Defined at src\\ovrDevice.kl:96:1
FABRIC_EXT_EXPORT void ovrDevice_GetFovTextureSize(
  KL::Traits< KL::ovrSize >::Result _result,
  KL::Traits< KL::ovrDevice >::INParam this_,
  KL::Traits< KL::SInt32 >::INParam eye,
  KL::Traits< KL::ovrFovPort >::INParam fov,
  KL::Traits< KL::Float32 >::INParam pixelsPerDisplayPixel
) {
  if(!this_->handle)
    return;
  ovrHmd hmd = (ovrHmd)this_->handle;

  ovrFovPort fov_;
  convert(fov, fov_);
  ovrSizei result = ovr_GetFovTextureSize(hmd, (ovrEyeType)eye, fov_, pixelsPerDisplayPixel);
  convert(result, _result);
}

// Defined at src\\ovrDevice.kl:101:1
FABRIC_EXT_EXPORT void ovrDevice_GetRenderDesc(
  KL::Traits< KL::ovrEyeRenderDesc >::Result _result,
  KL::Traits< KL::ovrDevice >::INParam this_,
  KL::Traits< KL::SInt32 >::INParam eye,
  KL::Traits< KL::ovrFovPort >::INParam fov
) {
  if(!this_->handle)
    return;
  ovrHmd hmd = (ovrHmd)this_->handle;

  ovrFovPort fov_;
  convert(fov, fov_);
  ovrEyeRenderDesc result = ovr_GetRenderDesc(hmd, (ovrEyeType)eye, fov_);
  convert(result, _result);
}

// Defined at src\\ovrDevice.kl:108:1
FABRIC_EXT_EXPORT KL::Boolean ovrDevice_SubmitFrame(
  KL::Traits< KL::ovrDevice >::IOParam this_,
  KL::Traits< KL::UInt32 >::INParam frameIndex,
  KL::Traits< KL::ovrViewScaleDesc >::INParam scale,
  KL::Traits< KL::VariableArray< KL::ovrLayerHeader > >::INParam layers
) {
  if(!this_->handle)
    return false;
  ovrHmd hmd = (ovrHmd)this_->handle;

  ovrViewScaleDesc scale_;
  convert(scale, scale_);

  std::vector<ovrLayerHeader> layers_;
  std::vector<ovrLayerHeader*> layerPtrs_;
  layers_.resize(layers.size());
  layerPtrs_.resize(layers.size());
  for(size_t i=0;i<layers_.size();i++)
  {
    convert(layers[i], layers_[i]);
    layerPtrs_[i] = &layers_[i];
  }

  if(layers_.size() == 0)
    return OVR_SUCCESS(ovr_SubmitFrame(hmd, 0, &scale_, 0, 0));
  return OVR_SUCCESS(ovr_SubmitFrame(hmd, 0, &scale_, &layerPtrs_[0], layerPtrs_.size()));
}

// Defined at src\\ovrDevice.kl:118:1
FABRIC_EXT_EXPORT void ovrDevice_GetFrameTiming(
  KL::Traits< KL::ovrFrameTiming >::Result _result,
  KL::Traits< KL::ovrDevice >::INParam this_,
  KL::Traits< KL::UInt32 >::INParam frameIndex
) {
  if(!this_->handle)
    return;
  ovrHmd hmd = (ovrHmd)this_->handle;
  ovrFrameTiming result = ovr_GetFrameTiming(hmd, frameIndex);
  convert(result, _result);
}

// Defined at src\\ovrDevice.kl:124:1
FABRIC_EXT_EXPORT KL::Float64 ovrDevice_GetTimeInSeconds(
  KL::Traits< KL::ovrDevice >::INParam this_
) {
  return ovr_GetTimeInSeconds();
}

// Defined at src\\ovrDevice.kl:130:1
FABRIC_EXT_EXPORT void ovrDevice_ResetBackOfHeadTracking(
  KL::Traits< KL::ovrDevice >::IOParam this_
) {
  if(!this_->handle)
    return;
  ovrHmd hmd = (ovrHmd)this_->handle;
  ovr_ResetBackOfHeadTracking(hmd);  
}

// Defined at src\\ovrDevice.kl:135:1
FABRIC_EXT_EXPORT void ovrDevice_ResetMulticameraTracking(
  KL::Traits< KL::ovrDevice >::IOParam this_
) {
  if(!this_->handle)
    return;
  ovrHmd hmd = (ovrHmd)this_->handle;
  ovr_ResetMulticameraTracking(hmd);
}

// Defined at src\\ovrDevice.kl:138:1
FABRIC_EXT_EXPORT KL::Boolean ovrDevice_GetBool(
  KL::Traits< KL::ovrDevice >::INParam this_,
  KL::Traits< KL::String >::INParam propertyName,
  KL::Traits< KL::Boolean >::INParam defaultValue
) {
  if(!this_->handle)
    return false;
  ovrHmd hmd = (ovrHmd)this_->handle;
  return ovr_GetBool(hmd, propertyName.getCString(), defaultValue);
}

// Defined at src\\ovrDevice.kl:142:1
FABRIC_EXT_EXPORT void ovrDevice_SetBool(
  KL::Traits< KL::ovrDevice >::IOParam this_,
  KL::Traits< KL::String >::INParam propertyName,
  KL::Traits< KL::Boolean >::INParam value
) {
  if(!this_->handle)
    return;
  ovrHmd hmd = (ovrHmd)this_->handle;
  ovr_SetBool(hmd, propertyName.getCString(), value);
}

// Defined at src\\ovrDevice.kl:145:1
FABRIC_EXT_EXPORT KL::SInt32 ovrDevice_GetInt(
  KL::Traits< KL::ovrDevice >::INParam this_,
  KL::Traits< KL::String >::INParam propertyName,
  KL::Traits< KL::SInt32 >::INParam defaultValue
) {
  if(!this_->handle)
    return 0;
  ovrHmd hmd = (ovrHmd)this_->handle;
  return ovr_GetInt(hmd, propertyName.getCString(), defaultValue);
}

// Defined at src\\ovrDevice.kl:149:1
FABRIC_EXT_EXPORT void ovrDevice_SetInt(
  KL::Traits< KL::ovrDevice >::IOParam this_,
  KL::Traits< KL::String >::INParam propertyName,
  KL::Traits< KL::SInt32 >::INParam value
) {
  if(!this_->handle)
    return;
  ovrHmd hmd = (ovrHmd)this_->handle;
  ovr_SetInt(hmd, propertyName.getCString(), value);
}

// Defined at src\\ovrDevice.kl:152:1
FABRIC_EXT_EXPORT KL::Float32 ovrDevice_GetFloat(
  KL::Traits< KL::ovrDevice >::INParam this_,
  KL::Traits< KL::String >::INParam propertyName,
  KL::Traits< KL::Float32 >::INParam defaultValue
) {
  if(!this_->handle)
    return 0.0f;
  ovrHmd hmd = (ovrHmd)this_->handle;
  return ovr_GetFloat(hmd, propertyName.getCString(), defaultValue);
}

// Defined at src\\ovrDevice.kl:156:1
FABRIC_EXT_EXPORT void ovrDevice_SetFloat(
  KL::Traits< KL::ovrDevice >::IOParam this_,
  KL::Traits< KL::String >::INParam propertyName,
  KL::Traits< KL::Float32 >::INParam value
) {
  if(!this_->handle)
    return;
  ovrHmd hmd = (ovrHmd)this_->handle;
  ovr_SetFloat(hmd, propertyName.getCString(), value);
}

// Defined at src\\ovrDevice.kl:160:1
FABRIC_EXT_EXPORT void ovrDevice_GetFloatArray(
  KL::Traits< KL::VariableArray< KL::Float32 > >::Result _result,
  KL::Traits< KL::ovrDevice >::INParam this_,
  KL::Traits< KL::String >::INParam propertyName,
  KL::Traits< KL::UInt32 >::INParam valuesCapacity
) {
  if(!this_->handle)
    return;
  ovrHmd hmd = (ovrHmd)this_->handle;
  _result.resize(valuesCapacity);
  ovr_GetFloatArray(hmd, propertyName.getCString(), &_result[0], valuesCapacity);
}


// Defined at src\\ovrDevice.kl:163:1
FABRIC_EXT_EXPORT void ovrDevice_SetFloatArray(
  KL::Traits< KL::ovrDevice >::IOParam this_,
  KL::Traits< KL::String >::INParam propertyName,
  KL::Traits< KL::VariableArray< KL::Float32 > >::INParam values
) {
  if(!this_->handle)
    return;
  ovrHmd hmd = (ovrHmd)this_->handle;
  ovr_SetFloatArray(hmd, propertyName.getCString(), &values[0], values.size());
}

// Defined at src\\ovrDevice.kl:167:1
FABRIC_EXT_EXPORT void ovrDevice_GetString(
  KL::Traits< KL::String >::Result _result,
  KL::Traits< KL::ovrDevice >::INParam this_,
  KL::Traits< KL::String >::INParam propertyName,
  KL::Traits< KL::String >::INParam defaultValue
) {
  if(!this_->handle)
    return;
  ovrHmd hmd = (ovrHmd)this_->handle;
  _result = ovr_GetString(hmd, propertyName.getCString(), defaultValue.getCString());
}

// Defined at src\\ovrDevice.kl:171:1
FABRIC_EXT_EXPORT void ovrDevice_SetString(
  KL::Traits< KL::ovrDevice >::IOParam this_,
  KL::Traits< KL::String >::INParam propertyName,
  KL::Traits< KL::String >::INParam value
) {
  if(!this_->handle)
    return;
  ovrHmd hmd = (ovrHmd)this_->handle;
  ovr_SetString(hmd, propertyName.getCString(), value.getCString());
}

// Defined at src\\ovrDevice.kl:187:1
FABRIC_EXT_EXPORT void ovrDevice_GetProjectionMat44(
  KL::Traits< KL::Mat44 >::Result _result,
  KL::Traits< KL::ovrDevice >::INParam this_,
  KL::Traits< KL::ovrFovPort >::INParam fov,
  KL::Traits< KL::Float32 >::INParam znear,
  KL::Traits< KL::Float32 >::INParam zfar,
  KL::Traits< KL::SInt32 >::INParam projectionModFlags
) {
  ovrFovPort fov_;
  convert(fov, fov_);
  ovrMatrix4f result = ovrMatrix4f_Projection(fov_, znear, zfar, (unsigned int)projectionModFlags);
  convert(result, _result);
}

// Defined at src\\ovrDevice.kl:190:1
FABRIC_EXT_EXPORT void ovrDevice_GetTimewarpProjectionDescFromProjection(
  KL::Traits< KL::ovrTimewarpProjectionDesc >::Result _result,
  KL::Traits< KL::ovrDevice >::INParam this_,
  KL::Traits< KL::Mat44 >::INParam projection,
  KL::Traits< KL::SInt32 >::INParam projectionModFlags
) {
  ovrMatrix4f projection_;
  convert(projection, projection_);
  ovrTimewarpProjectionDesc result = ovrTimewarpProjectionDesc_FromProjection(projection_, (unsigned int)projectionModFlags);
  convert(result, _result);
}

// Defined at src\\ovrDevice.kl:195:1
FABRIC_EXT_EXPORT void ovrDevice_GetOrthoSubProjectionMat44(
  KL::Traits< KL::Mat44 >::Result _result,
  KL::Traits< KL::ovrDevice >::INParam this_,
  KL::Traits< KL::Mat44 >::INParam projection,
  KL::Traits< KL::Vec2 >::INParam orthoScale,
  KL::Traits< KL::Float32 >::INParam orthoDistance,
  KL::Traits< KL::Float32 >::INParam hmdToEyeViewOffsetX
) {
  ovrMatrix4f projection_;
  convert(projection, projection_);
  ovrVector2f orthoScale_;
  convert(orthoScale, orthoScale_);
  ovrMatrix4f result = ovrMatrix4f_OrthoSubProjection(projection_, orthoScale_, orthoDistance, hmdToEyeViewOffsetX);
  convert(result, _result);
}

// Defined at src\\ovrDevice.kl:198:1
FABRIC_EXT_EXPORT void ovrDevice_CalcEyePoses(
  KL::Traits< KL::FixedArray< KL::ovrPose, 2 > >::Result _result,
  KL::Traits< KL::ovrDevice >::INParam this_,
  KL::Traits< KL::ovrPose >::INParam headPose,
  KL::Traits< KL::FixedArray< KL::Vec3, 2 > >::INParam hmdToEyeViewOffset
) {
  ovrPosef headPose_;
  convert(headPose, headPose_);
  ovrVector3f hmdToEyeViewOffset_[2];
  convert(hmdToEyeViewOffset[0], hmdToEyeViewOffset_[0]);
  convert(hmdToEyeViewOffset[1], hmdToEyeViewOffset_[1]);
  ovrPosef outEyePoses[2];
  ovr_CalcEyePoses(headPose_, hmdToEyeViewOffset_, outEyePoses);
  convert(outEyePoses[0], _result[0]);
  convert(outEyePoses[1], _result[1]);
}

// Defined at src\\ovrDevice.kl:206:1
FABRIC_EXT_EXPORT void ovrDevice_GetEyePoses(
  KL::Traits< KL::ovrDevice >::INParam this_,
  KL::Traits< KL::UInt32 >::INParam frameIndex,
  KL::Traits< KL::FixedArray< KL::Vec3, 2 > >::INParam hmdToEyeViewOffset,
  KL::Traits< KL::FixedArray< KL::ovrPose, 2 > >::IOParam outEyePoses,
  KL::Traits< KL::ovrTrackingState >::IOParam outHmdTrackingState
) {
  if(!this_->handle)
    return;
  ovrHmd hmd = (ovrHmd)this_->handle;
  ovrVector3f hmdToEyeViewOffset_[2];
  convert(hmdToEyeViewOffset[0], hmdToEyeViewOffset_[0]);
  convert(hmdToEyeViewOffset[1], hmdToEyeViewOffset_[1]);
  ovrPosef outEyePoses_[2];
  ovrTrackingState outHmdTrackingState_;
  ovr_GetEyePoses(hmd, frameIndex, hmdToEyeViewOffset_, outEyePoses_, &outHmdTrackingState_);
  convert(outEyePoses_[0], outEyePoses[0]);
  convert(outEyePoses_[1], outEyePoses[1]);
  convert(outHmdTrackingState_, outHmdTrackingState);
}
