/*
 *  Copyright 2010-2014 Fabric Software Inc. All rights reserved.
 */

#include "ovrGLDepthBuffer_impl.h"
#include "ovrGLDepthBuffer_functions.h"

using namespace Fabric::EDK;

// Defined at src\\ovrGLDepthBuffer.kl:10:1
FABRIC_EXT_EXPORT void ovrGLDepthBuffer_Construct(
  KL::Traits< KL::ovrGLDepthBuffer >::IOParam this_,
  KL::Traits< KL::ovrSize >::INParam size,
  KL::Traits< KL::SInt32 >::INParam sampleCount
) {
  ensureGlewIsInitialized();

  this_->handle = NULL;
  ovrSizei size_;
  convert(size, size_);
  this_->handle = new DepthBuffer(size_, sampleCount);
}

// Defined at src\\ovrGLDepthBuffer.kl:12:1
FABRIC_EXT_EXPORT void ovrGLDepthBuffer_Destruct(
  KL::Traits< KL::ovrGLDepthBuffer >::IOParam this_
) {
  if(!this_->handle)
    return;

  DepthBuffer * buffer = (DepthBuffer *)this_->handle;
  delete(buffer);
  this_->handle = NULL;
}

// Defined at src\\ovrGLDepthBuffer.kl:14:1
FABRIC_EXT_EXPORT KL::SInt32 ovrGLDepthBuffer_GetTexId(
  KL::Traits< KL::ovrGLDepthBuffer >::INParam this_
) {
  if(!this_->handle)
    return 0;
  DepthBuffer * buffer = (DepthBuffer *)this_->handle;
  return  buffer->texId;
}
