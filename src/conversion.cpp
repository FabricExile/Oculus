/*
 *  Copyright 2010-2014 Fabric Software Inc. All rights reserved.
 */

#include "conversion.h"

namespace Fabric
{
  namespace EDK
  {
    void convert(KL::Vec2 a, ovrVector2f & b) {
      b.x = a.x;
      b.y = a.y;
    }

    void convert(KL::ovrSize a, ovrSizei & b) {
      b.w = a.w;
      b.h = a.h;
    }

    void convert(KL::ovrVec2 a, ovrVector2i & b) {
      b.x = a.x;
      b.y = a.y;
    }

    void convert(KL::ovrRect a, ovrRecti & b) {
      convert(a.Pos, b.Pos);
      convert(a.Size, b.Size);
    }

    void convert(KL::Vec3 a, ovrVector3f & b) {
      b.x = a.x;
      b.y = a.y;
      b.z = a.z;
    }

    void convert(KL::Quat a, ovrQuatf & b) {
      b.x = a.v.x;
      b.y = a.v.y;
      b.z = a.v.z;
      b.w = a.w;
    }

    void convert(KL::Mat44 a, ovrMatrix4f & b) {
      b.M[0][0] = a.row0.x;
      b.M[0][1] = a.row0.y;
      b.M[0][2] = a.row0.z;
      b.M[0][3] = a.row0.t;
      b.M[1][0] = a.row1.x;
      b.M[1][1] = a.row1.y;
      b.M[1][2] = a.row1.z;
      b.M[1][3] = a.row1.t;
      b.M[2][0] = a.row2.x;
      b.M[2][1] = a.row2.y;
      b.M[2][2] = a.row2.z;
      b.M[2][3] = a.row2.t;
      b.M[3][0] = a.row3.x;
      b.M[3][1] = a.row3.y;
      b.M[3][2] = a.row3.z;
      b.M[3][3] = a.row3.t;
    }

    void convert(KL::ovrPose a, ovrPosef & b) {
      convert(a.Orientation, b.Orientation);
      convert(a.Position, b.Position);
    }

    void convert(KL::ovrFovPort a, ovrFovPort & b) {
      b.UpTan = a.UpTan;
      b.DownTan = a.DownTan;
      b.LeftTan = a.LeftTan;
      b.RightTan = a.RightTan;
    }

    void convert(KL::ovrPoseState a, ovrPoseStatef & b) {
      convert(a.ThePose, b.ThePose);
      convert(a.AngularVelocity, b.AngularVelocity);
      convert(a.LinearVelocity, b.LinearVelocity);
      convert(a.AngularAcceleration, b.AngularAcceleration);
      convert(a.LinearAcceleration, b.LinearAcceleration);
      b.TimeInSeconds = a.TimeInSeconds;
    }

    void convert(KL::ovrSensorData a, ovrSensorData & b) {
      convert(a.Accelerometer, b.Accelerometer);
      convert(a.Gyro, b.Gyro);
      convert(a.Magnetometer, b.Magnetometer);
      b.Temperature = a.Temperature;
      b.TimeInSeconds = a.TimeInSeconds;
    }

    void convert(KL::ovrTrackingState a, ovrTrackingState & b) {
      convert(a.HeadPose, b.HeadPose);
      convert(a.CameraPose, b.CameraPose);
      convert(a.LeveledCameraPose, b.LeveledCameraPose);
      convert(a.RawSensorData, b.RawSensorData);
      b.StatusFlags = a.StatusFlags;
      b.LastVisionProcessingTime = a.LastVisionProcessingTime;
    }

    void convert(KL::ovrEyeRenderDesc a, ovrEyeRenderDesc & b) {
      b.Eye = (ovrEyeType)a.Eye;
      convert(a.Fov, b.Fov);
      convert(a.DistortedViewport, b.DistortedViewport);
      convert(a.PixelsPerTanAngleAtCenter, b.PixelsPerTanAngleAtCenter);
      convert(a.HmdToEyeViewOffset, b.HmdToEyeViewOffset);
    }

    void convert(KL::ovrGLTexture a, ovrTexture & b) {
      b.Header.API = (ovrRenderAPIType)a.API;
      convert(a.TextureSize, b.Header.TextureSize);
      convert(a.RenderViewport, b.Header.RenderViewport);

      ovrGLTexture * bGL = (ovrGLTexture*)&b;
      bGL->OGL.TexId = a.TexId;
    }

    void convert(KL::ovrGLConfig a, ovrRenderAPIConfig & b) {
      b.Header.API = (ovrRenderAPIType)a.API;
      convert(a.RTSize, b.Header.RTSize);
      b.Header.Multisample = a.Multisample;

      ovrGLConfig * bGL = (ovrGLConfig*)&b;
#if defined(OVR_OS_WIN32)
      bGL->OGL.Window = (HWND)a.Window;
      bGL->OGL.DC = wglGetCurrentDC();
#elif defined(OVR_OS_LINUX)
      bGL->OGL.Disp = (_XDisplay*)a.Disp;
      bGL->OGL.Win = (Window)a.Win;
#endif
    }

    void convert(KL::ovrFrameTiming a, ovrFrameTiming & b) {
      b.DeltaSeconds = a.DeltaSeconds;
      b.ThisFrameSeconds = a.ThisFrameSeconds;
      b.TimewarpPointSeconds = a.TimewarpPointSeconds;
      b.NextFrameSeconds = a.NextFrameSeconds;
      b.ScanoutMidpointSeconds = a.ScanoutMidpointSeconds;
      b.EyeScanoutSeconds[0] = a.EyeScanoutSeconds[0];
      b.EyeScanoutSeconds[1] = a.EyeScanoutSeconds[1];
    }

    //------------------------------------------------------------------------------------------------------

    void convert(ovrVector2f a, KL::Vec2 & b) {
      b.x = a.x;
      b.y = a.y;
    }

    void convert(ovrSizei a, KL::ovrSize & b) {
      b.w = a.w;
      b.h = a.h;
    }

    void convert(ovrVector2i a, KL::ovrVec2 & b) {
      b.x = a.x;
      b.y = a.y;
    }

    void convert(ovrRecti a, KL::ovrRect & b) {
      convert(a.Pos, b.Pos);
      convert(a.Size, b.Size);
    }

    void convert(ovrVector3f a, KL::Vec3 & b) {
      b.x = a.x;
      b.y = a.y;
      b.z = a.z;
    }

    void convert(ovrQuatf a, KL::Quat & b) {
      b.v.x = a.x;
      b.v.y = a.y;
      b.v.z = a.z;
      b.w = a.w;
    }

    void convert(ovrMatrix4f a, KL::Mat44 & b) {
      b.row0.x = a.M[0][0];
      b.row0.y = a.M[0][1];
      b.row0.z = a.M[0][2];
      b.row0.t = a.M[0][3];
      b.row1.x = a.M[1][0];
      b.row1.y = a.M[1][1];
      b.row1.z = a.M[1][2];
      b.row1.t = a.M[1][3];
      b.row2.x = a.M[2][0];
      b.row2.y = a.M[2][1];
      b.row2.z = a.M[2][2];
      b.row2.t = a.M[2][3];
      b.row3.x = a.M[3][0];
      b.row3.y = a.M[3][1];
      b.row3.z = a.M[3][2];
      b.row3.t = a.M[3][3];
    }

    void convert(ovrPosef a, KL::ovrPose & b) {
      convert(a.Orientation, b.Orientation);
      convert(a.Position, b.Position);
    }

    void convert(ovrFovPort a, KL::ovrFovPort & b) {
      b.UpTan = a.UpTan;
      b.DownTan = a.DownTan;
      b.LeftTan = a.LeftTan;
      b.RightTan = a.RightTan;
    }

    void convert(ovrPoseStatef a, KL::ovrPoseState & b) {
      convert(a.ThePose, b.ThePose);
      convert(a.AngularVelocity, b.AngularVelocity);
      convert(a.LinearVelocity, b.LinearVelocity);
      convert(a.AngularAcceleration, b.AngularAcceleration);
      convert(a.LinearAcceleration, b.LinearAcceleration);
      b.TimeInSeconds = a.TimeInSeconds;
    }

    void convert(ovrSensorData a, KL::ovrSensorData & b) {
      convert(a.Accelerometer, b.Accelerometer);
      convert(a.Gyro, b.Gyro);
      convert(a.Magnetometer, b.Magnetometer);
      b.Temperature = a.Temperature;
      b.TimeInSeconds = a.TimeInSeconds;
    }

    void convert(ovrTrackingState a, KL::ovrTrackingState & b) {
      convert(a.HeadPose, b.HeadPose);
      convert(a.CameraPose, b.CameraPose);
      convert(a.LeveledCameraPose, b.LeveledCameraPose);
      convert(a.RawSensorData, b.RawSensorData);
      b.StatusFlags = a.StatusFlags;
      b.LastVisionProcessingTime = a.LastVisionProcessingTime;
    }

    void convert(ovrEyeRenderDesc a, KL::ovrEyeRenderDesc & b) {
      b.Eye = a.Eye;
      convert(a.Fov, b.Fov);
      convert(a.DistortedViewport, b.DistortedViewport);
      convert(a.PixelsPerTanAngleAtCenter, b.PixelsPerTanAngleAtCenter);
      convert(a.HmdToEyeViewOffset, b.HmdToEyeViewOffset);
    }

    void convert(const ovrTexture & a, KL::ovrGLTexture & b) {
      b.API = a.Header.API;
      convert(a.Header.TextureSize, b.TextureSize);
      convert(a.Header.RenderViewport, b.RenderViewport);

      ovrGLTexture * aGL = (ovrGLTexture*)&a;
      b.TexId = aGL->OGL.TexId;
    }

    void convert(ovrRenderAPIConfig a, KL::ovrGLConfig & b) {
      b.API = a.Header.API;
      convert(a.Header.RTSize, b.RTSize);
      b.Multisample = a.Header.Multisample;

      ovrGLConfig * aGL = (ovrGLConfig*)&a;
#if defined(OVR_OS_WIN32)
      b.Window = (KL::UInt64)aGL->OGL.Window;
#elif defined(OVR_OS_LINUX)
      b.Disp = (KL::UInt64)aGL->OGL.Disp;
      b.Win = (KL::UInt64)aGL->OGL.Win;
#endif
    }
    
    void convert(ovrFrameTiming a, KL::ovrFrameTiming & b) {
      b.DeltaSeconds = a.DeltaSeconds;
      b.ThisFrameSeconds = a.ThisFrameSeconds;
      b.TimewarpPointSeconds = a.TimewarpPointSeconds;
      b.NextFrameSeconds = a.NextFrameSeconds;
      b.ScanoutMidpointSeconds = a.ScanoutMidpointSeconds;
      b.EyeScanoutSeconds[0] = a.EyeScanoutSeconds[0];
      b.EyeScanoutSeconds[1] = a.EyeScanoutSeconds[1];
    }
  }
}
