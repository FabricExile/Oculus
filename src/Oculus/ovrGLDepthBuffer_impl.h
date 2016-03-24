/*
 *  Copyright 2010-2014 Fabric Software Inc. All rights reserved.
 */

#ifndef __DEPTHBUFFER_H__
#define __DEPTHBUFFER_H__

#include "conversion.h"

struct DepthBuffer
{
    GLuint texId;

    DepthBuffer(ovrSizei size, int sampleCount)
    {
        OVR_ASSERT(sampleCount <= 1); // The code doesn't currently handle MSAA textures.

        glGenTextures(1, &texId);
        glBindTexture(GL_TEXTURE_2D, texId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        GLenum internalFormat = GL_DEPTH_COMPONENT32F;
        GLenum type = GL_FLOAT;

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.w, size.h, 0, GL_DEPTH_COMPONENT, type, NULL);
    }

    ~DepthBuffer()
    {
        if (texId)
        {
            glDeleteTextures(1, &texId);
        }
    }
};

#endif
