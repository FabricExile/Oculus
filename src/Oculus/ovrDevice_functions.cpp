/*
 *  Copyright 2010-2014 Fabric Software Inc. All rights reserved.
 */

#include "conversion.h"
#include <FabricEDK.h>
#include "ovrDevice_functions.h"
#include "ovrGLTextureBuffer_functions.h"
#include "ovrGLDepthBuffer_functions.h"
#include "ovrGLMirrorTexture_functions.h"
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>

#include <vector>

using namespace Fabric::EDK;

typedef boost::shared_mutex Lock;
typedef boost::unique_lock< Lock >  WriteLock;
typedef boost::shared_lock< Lock >  ReadLock;
Lock gDeviceLock;


void SetupCallback()
{
  WriteLock w_lock(gDeviceLock);
  OVR::System::Init();
  ovr_Initialize(NULL);
}

// void ShutdownCallback()
// {
//   WriteLock w_lock(gDeviceLock);
//   ovr_Shutdown();
//   OVR::System::Destroy();
// }

IMPLEMENT_FABRIC_EDK_ENTRIES_WITH_SETUP_CALLBACK( Oculus, &SetupCallback )

// Defined at src\ovrDevice.kl:21:1
FABRIC_EXT_EXPORT void ovrDevice_Construct(
  KL::Traits< KL::ovrDevice >::IOParam this_,
  KL::Traits< KL::UInt32 >::INParam index
) {

  WriteLock w_lock(gDeviceLock);

  this_->session = KL::ovrSession::Create();
  ovrSession * session_ = (ovrSession *)this_->session->handle;

  ovrHmdDesc hmdDesc = ovr_GetHmdDesc(*session_);
  ovrSizei windowSize = { hmdDesc.Resolution.w / 2, hmdDesc.Resolution.h / 2 };

  this_->configured = false;
  this_->stereoEnabled = true;
}

// Defined at src\ovrDevice.kl:28:1
FABRIC_EXT_EXPORT void ovrDevice_Destruct(
  KL::Traits< KL::ovrDevice >::IOParam this_
) {
  WriteLock w_lock(gDeviceLock);

  for(unsigned int i=0;i<this_->textureBuffers.size();i++)
    ovrGLTextureBuffer_Destruct(this_->textureBuffers[i]);
  for(unsigned int i=0;i<this_->depthBuffers.size();i++)
    ovrGLDepthBuffer_Destruct(this_->depthBuffers[i]);
  for(unsigned int i=0;i<this_->mirrorTextures.size();i++)
    ovrGLMirrorTexture_Destruct(this_->mirrorTextures[i]);
}

// Defined at src\ovrDevice.kl:32:1
FABRIC_EXT_EXPORT void ovrDevice_GetDescription(
  KL::Traits< KL::ovrDescription >::Result _result,
  KL::Traits< KL::ovrDevice >::INParam this_
) {

  ovrSession * session_ = (ovrSession *)this_->session->handle;
  if(!session_)
    return;
  ovrHmdDesc desc = ovr_GetHmdDesc(*session_);
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
}

// Defined at src\\ovrDevice.kl:68:1
FABRIC_EXT_EXPORT void ovrDevice_RecenterPose(
  KL::Traits< KL::ovrDevice >::IOParam this_
) {
  ovrSession * session_ = (ovrSession *)this_->session->handle;
  if(!session_)
    return;
  ovr_RecenterPose(*session_);
}

// Defined at src\\ovrDevice.kl:76:1
FABRIC_EXT_EXPORT void ovrDevice_GetTrackingState(
  KL::Traits< KL::ovrTrackingState >::Result _result,
  KL::Traits< KL::ovrDevice >::INParam this_,
  KL::Traits< KL::Float64 >::INParam absTime,
  KL::Traits< KL::Boolean >::INParam latencyMarker
) {
  ovrSession * session_ = (ovrSession *)this_->session->handle;
  if(!session_)
    return;

  double absTime_ = absTime;
  if(absTime_ < 0.0)
    absTime_ = ovr_GetTimeInSeconds();
  ovrTrackingState state = ovr_GetTrackingState(*session_, absTime_, latencyMarker);
  convert(state, _result);
}

// Defined at src\\ovrDevice.kl:80:1
FABRIC_EXT_EXPORT KL::Boolean ovrDevice_GetInputState(
  KL::Traits< KL::ovrDevice >::INParam this_,
  KL::Traits< KL::SInt32 >::INParam controllerTypeMask,
  KL::Traits< KL::ovrInputState >::IOParam state
) {
  ovrSession * session_ = (ovrSession *)this_->session->handle;
  if(!session_)
    return false;
  ovrInputState _state;
  bool result = OVR_SUCCESS(ovr_GetInputState(*session_, controllerTypeMask, &_state));
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
  ovrSession * session_ = (ovrSession *)this_->session->handle;
  if(!session_)
    return false;
  return OVR_SUCCESS(ovr_SetControllerVibration(*session_, controllerTypeMask, frequency, amplitude));
}

// Defined at src\\ovrDevice.kl:96:1
FABRIC_EXT_EXPORT void ovrDevice_GetFovTextureSize(
  KL::Traits< KL::ovrSize >::Result _result,
  KL::Traits< KL::ovrDevice >::INParam this_,
  KL::Traits< KL::SInt32 >::INParam eye,
  KL::Traits< KL::ovrFovPort >::INParam fov,
  KL::Traits< KL::Float32 >::INParam pixelsPerDisplayPixel
) {
  ovrSession * session_ = (ovrSession *)this_->session->handle;
  if(!session_)
    return;

  ovrFovPort fov_;
  convert(fov, fov_);
  ovrSizei result = ovr_GetFovTextureSize(*session_, (ovrEyeType)eye, fov_, pixelsPerDisplayPixel);
  convert(result, _result);
}

// Defined at src\\ovrDevice.kl:101:1
FABRIC_EXT_EXPORT void ovrDevice_GetRenderDesc(
  KL::Traits< KL::ovrEyeRenderDesc >::Result _result,
  KL::Traits< KL::ovrDevice >::INParam this_,
  KL::Traits< KL::SInt32 >::INParam eye,
  KL::Traits< KL::ovrFovPort >::INParam fov
) {
  ovrSession * session_ = (ovrSession *)this_->session->handle;
  if(!session_)
    return;

  ovrFovPort fov_;
  convert(fov, fov_);
  ovrEyeRenderDesc result = ovr_GetRenderDesc(*session_, (ovrEyeType)eye, fov_);
  convert(result, _result);
}

// Defined at src\\ovrDevice.kl:130:1
FABRIC_EXT_EXPORT KL::Boolean ovrDevice_SubmitFrame_ovrLayerEyeFov(
  KL::Traits< KL::ovrDevice >::IOParam this_,
  KL::Traits< KL::UInt32 >::INParam frameIndex,
  KL::Traits< KL::ovrViewScaleDesc >::INParam scale,
  KL::Traits< KL::ovrLayerEyeFov >::INParam layer
) {

  ovrSession * session_ = (ovrSession *)this_->session->handle;
  if(!session_)
    return false;

  ovrViewScaleDesc scale_;
  convert(scale, scale_);

  ovrLayerEyeFov layer_;
  convert(layer, layer_);

  ovrLayerHeader * lh = &layer_.Header;
  return OVR_SUCCESS(ovr_SubmitFrame(*session_, 0, &scale_, &lh, 1));
}

// Defined at src\\ovrDevice.kl:162:1
FABRIC_EXT_EXPORT KL::Float64 ovrDevice_GetPredictedDisplayTime(
  KL::Traits< KL::ovrDevice >::INParam this_,
  KL::Traits< KL::SInt64 >::INParam frameIndex
) {
  ovrSession * session_ = (ovrSession *)this_->session->handle;
  if(!session_)
    return 0.0;

  return ovr_GetPredictedDisplayTime(*session_, frameIndex);
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
  ovrSession * session_ = (ovrSession *)this_->session->handle;
  if(!session_)
    return;
  ovr_ResetBackOfHeadTracking(*session_);  
}

// Defined at src\\ovrDevice.kl:135:1
FABRIC_EXT_EXPORT void ovrDevice_ResetMulticameraTracking(
  KL::Traits< KL::ovrDevice >::IOParam this_
) {
  ovrSession * session_ = (ovrSession *)this_->session->handle;
  if(!session_)
    return;
  ovr_ResetMulticameraTracking(*session_);
}

// Defined at src\\ovrDevice.kl:138:1
FABRIC_EXT_EXPORT KL::Boolean ovrDevice_GetBool(
  KL::Traits< KL::ovrDevice >::INParam this_,
  KL::Traits< KL::String >::INParam propertyName,
  KL::Traits< KL::Boolean >::INParam defaultValue
) {
  ovrSession * session_ = (ovrSession *)this_->session->handle;
  if(!session_)
    return false;
  return ovr_GetBool(*session_, propertyName.getCString(), defaultValue);
}

// Defined at src\\ovrDevice.kl:142:1
FABRIC_EXT_EXPORT void ovrDevice_SetBool(
  KL::Traits< KL::ovrDevice >::IOParam this_,
  KL::Traits< KL::String >::INParam propertyName,
  KL::Traits< KL::Boolean >::INParam value
) {
  ovrSession * session_ = (ovrSession *)this_->session->handle;
  if(!session_)
    return;
  ovr_SetBool(*session_, propertyName.getCString(), value);
}

// Defined at src\\ovrDevice.kl:145:1
FABRIC_EXT_EXPORT KL::SInt32 ovrDevice_GetInt(
  KL::Traits< KL::ovrDevice >::INParam this_,
  KL::Traits< KL::String >::INParam propertyName,
  KL::Traits< KL::SInt32 >::INParam defaultValue
) {
  ovrSession * session_ = (ovrSession *)this_->session->handle;
  if(!session_)
    return 0;
  return ovr_GetInt(*session_, propertyName.getCString(), defaultValue);
}

// Defined at src\\ovrDevice.kl:149:1
FABRIC_EXT_EXPORT void ovrDevice_SetInt(
  KL::Traits< KL::ovrDevice >::IOParam this_,
  KL::Traits< KL::String >::INParam propertyName,
  KL::Traits< KL::SInt32 >::INParam value
) {
  ovrSession * session_ = (ovrSession *)this_->session->handle;
  if(!session_)
    return;
  ovr_SetInt(*session_, propertyName.getCString(), value);
}

// Defined at src\\ovrDevice.kl:152:1
FABRIC_EXT_EXPORT KL::Float32 ovrDevice_GetFloat(
  KL::Traits< KL::ovrDevice >::INParam this_,
  KL::Traits< KL::String >::INParam propertyName,
  KL::Traits< KL::Float32 >::INParam defaultValue
) {
  ovrSession * session_ = (ovrSession *)this_->session->handle;
  if(!session_)
    return 0.0f;
  return ovr_GetFloat(*session_, propertyName.getCString(), defaultValue);
}

// Defined at src\\ovrDevice.kl:156:1
FABRIC_EXT_EXPORT void ovrDevice_SetFloat(
  KL::Traits< KL::ovrDevice >::IOParam this_,
  KL::Traits< KL::String >::INParam propertyName,
  KL::Traits< KL::Float32 >::INParam value
) {
  ovrSession * session_ = (ovrSession *)this_->session->handle;
  if(!session_)
    return;
  ovr_SetFloat(*session_, propertyName.getCString(), value);
}

// Defined at src\\ovrDevice.kl:160:1
FABRIC_EXT_EXPORT void ovrDevice_GetFloatArray(
  KL::Traits< KL::VariableArray< KL::Float32 > >::Result _result,
  KL::Traits< KL::ovrDevice >::INParam this_,
  KL::Traits< KL::String >::INParam propertyName,
  KL::Traits< KL::UInt32 >::INParam valuesCapacity
) {
  ovrSession * session_ = (ovrSession *)this_->session->handle;
  if(!session_)
    return;
  _result.resize(valuesCapacity);
  ovr_GetFloatArray(*session_, propertyName.getCString(), &_result[0], valuesCapacity);
}


// Defined at src\\ovrDevice.kl:163:1
FABRIC_EXT_EXPORT void ovrDevice_SetFloatArray(
  KL::Traits< KL::ovrDevice >::IOParam this_,
  KL::Traits< KL::String >::INParam propertyName,
  KL::Traits< KL::VariableArray< KL::Float32 > >::INParam values
) {
  ovrSession * session_ = (ovrSession *)this_->session->handle;
  if(!session_)
    return;
  ovr_SetFloatArray(*session_, propertyName.getCString(), &values[0], values.size());
}

// Defined at src\\ovrDevice.kl:167:1
FABRIC_EXT_EXPORT void ovrDevice_GetString(
  KL::Traits< KL::String >::Result _result,
  KL::Traits< KL::ovrDevice >::INParam this_,
  KL::Traits< KL::String >::INParam propertyName,
  KL::Traits< KL::String >::INParam defaultValue
) {
  ovrSession * session_ = (ovrSession *)this_->session->handle;
  if(!session_)
    return;
  _result = ovr_GetString(*session_, propertyName.getCString(), defaultValue.getCString());
}

// Defined at src\\ovrDevice.kl:171:1
FABRIC_EXT_EXPORT void ovrDevice_SetString(
  KL::Traits< KL::ovrDevice >::IOParam this_,
  KL::Traits< KL::String >::INParam propertyName,
  KL::Traits< KL::String >::INParam value
) {
  ovrSession * session_ = (ovrSession *)this_->session->handle;
  if(!session_)
    return;
  ovr_SetString(*session_, propertyName.getCString(), value.getCString());
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
  KL::Traits< KL::Boolean >::INParam latencyMarker,
  KL::Traits< KL::FixedArray< KL::Vec3, 2 > >::INParam hmdToEyeViewOffset,
  KL::Traits< KL::FixedArray< KL::ovrPose, 2 > >::IOParam outEyePoses,
  KL::Traits< KL::ovrTrackingState >::IOParam outHmdTrackingState
) {
  ovrSession * session_ = (ovrSession *)this_->session->handle;
  if(!session_)
    return;

  ovrVector3f hmdToEyeViewOffset_[2];
  convert(hmdToEyeViewOffset[0], hmdToEyeViewOffset_[0]);
  convert(hmdToEyeViewOffset[1], hmdToEyeViewOffset_[1]);
  ovrPosef outEyePoses_[2];
  ovrTrackingState outHmdTrackingState_;

  ovr_GetEyePoses(*session_, frameIndex, latencyMarker, hmdToEyeViewOffset_, outEyePoses_, &outHmdTrackingState_);
  convert(outEyePoses_[0], outEyePoses[0]);
  convert(outEyePoses_[1], outEyePoses[1]);
  convert(outHmdTrackingState_, outHmdTrackingState);
}
