/*
 *  Copyright 2010-2014 Fabric Software Inc. All rights reserved.
 */

#ifndef __conversion_h__
#define __conversion_h__

#include "Oculus.h"

#include <Kernel/OVR_System.h>

// Include the Oculus SDK
#include "OVR_CAPI_GL.h"

namespace Fabric
{
  namespace EDK
  {
    void convert(KL::Vec2 a, ovrVector2f & b);
    void convert(KL::ovrSize a, ovrSizei & b);
    void convert(KL::ovrVec2 a, ovrVector2i & b);
    void convert(KL::ovrRect a, ovrRecti & b);
    void convert(KL::Vec3 a, ovrVector3f & b);
    void convert(KL::Quat a, ovrQuatf & b);
    void convert(KL::Mat44 a, ovrMatrix4f & b);
    void convert(KL::ovrPose a, ovrPosef & b);
    void convert(KL::ovrFovPort a, ovrFovPort & b);
    void convert(KL::ovrPoseState a, ovrPoseStatef & b);
    void convert(KL::ovrSensorData a, ovrSensorData & b);
    void convert(KL::ovrTrackingState a, ovrTrackingState & b);
    void convert(KL::ovrInputState a, ovrInputState & b);
    void convert(KL::ovrEyeRenderDesc a, ovrEyeRenderDesc & b);
    void convert(KL::ovrTimewarpProjectionDesc a, ovrTimewarpProjectionDesc & b);
    void convert(KL::ovrViewScaleDesc a, ovrViewScaleDesc & b);
    void convert(KL::ovrTextureHeader a, ovrTextureHeader & b);
    void convert(KL::ovrLayerHeader a, ovrLayerHeader & b);
    void convert(KL::ovrFrameTiming a, ovrFrameTiming & b);
    // void convert(KL::ovrGLTexture a, ovrTexture & b);

    //------------------------------------------------------------------------------------------------------

    void convert(ovrVector2f a, KL::Vec2 & b);
    void convert(ovrSizei a, KL::ovrSize & b);
    void convert(ovrVector2i a, KL::ovrVec2 & b);
    void convert(ovrRecti a, KL::ovrRect & b);
    void convert(ovrVector3f a, KL::Vec3 & b);
    void convert(ovrQuatf a, KL::Quat & b);
    void convert(ovrMatrix4f a, KL::Mat44 & b);
    void convert(ovrPosef a, KL::ovrPose & b);
    void convert(ovrFovPort a, KL::ovrFovPort & b);
    void convert(ovrPoseStatef a, KL::ovrPoseState & b);
    void convert(ovrSensorData a, KL::ovrSensorData & b);
    void convert(ovrTrackingState a, KL::ovrTrackingState & b);
    void convert(ovrInputState a, KL::ovrInputState & b);
    void convert(ovrEyeRenderDesc a, KL::ovrEyeRenderDesc & b);
    void convert(ovrEyeRenderDesc a, KL::ovrEyeRenderDesc & b);
    void convert(ovrTimewarpProjectionDesc a, KL::ovrTimewarpProjectionDesc & b);
    void convert(ovrViewScaleDesc a, KL::ovrViewScaleDesc & b);
    void convert(ovrTextureHeader a, KL::ovrTextureHeader & b);
    void convert(ovrLayerHeader a, KL::ovrLayerHeader & b);
    void convert(ovrFrameTiming a, KL::ovrFrameTiming & b);
    void convert(ovrHmdDesc a, KL::ovrDescription & b);
    // void convert(const ovrTexture & a, KL::ovrGLTexture & b);
  }
}

#endif
