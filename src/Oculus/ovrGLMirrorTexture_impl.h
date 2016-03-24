/*
 *  Copyright 2010-2014 Fabric Software Inc. All rights reserved.
 */
#ifndef __MIRRORTEXTUREBUFFER_H__
#define __MIRRORTEXTUREBUFFER_H__

#include "conversion.h"

struct MirrorTexture
{
    ovrHmd device;
    ovrGLTexture* texture;
    GLuint texId;
    GLuint fboId;
    ovrSizei texSize;

    MirrorTexture(ovrHmd hmd, ovrSizei size)
        : device(hmd), texture(nullptr), texId(0), fboId(0)
    {
        texSize = size;

        ovrResult result = ovr_CreateMirrorTextureGL(hmd, GL_SRGB8_ALPHA8, size.w, size.h, reinterpret_cast<ovrTexture**>(&texture));
        if (OVR_SUCCESS(result))
        {
          texId = texture->OGL.TexId;

          // Configure the mirror read buffer
          glGenFramebuffers(1, &fboId);
          glBindFramebuffer(GL_READ_FRAMEBUFFER, fboId);
          glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texId, 0);
          glFramebufferRenderbuffer(GL_READ_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
          glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
        }
    }

    ~MirrorTexture()
    {
      if(fboId)
        glDeleteFramebuffers(1, &fboId);
      if(texture)
        ovr_DestroyMirrorTexture(device, reinterpret_cast<ovrTexture*>(texture));
    }

    ovrSizei GetSize() const
    {
      return texSize;
    }

    void BlitTexture()
    {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, fboId);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        GLint w = texSize.w;
        GLint h = texSize.h;
        glBlitFramebuffer(0, h, w, 0,
                          0, 0, w, h,
                          GL_COLOR_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    }
};

#endif
