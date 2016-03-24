/*
 *  Copyright 2010-2014 Fabric Software Inc. All rights reserved.
 */

#include "ovrGLTextureBuffer_impl.h"
#include "ovrGLTextureBuffer_functions.h"

using namespace Fabric::EDK;

// Defined at src\\ovrGLTextureBuffer.kl:10:1
FABRIC_EXT_EXPORT void ovrGLTextureBuffer_Construct(
  KL::Traits< KL::ovrGLTextureBuffer >::IOParam this_,
  KL::Traits< KL::ovrDevice >::INParam device,
  KL::Traits< KL::Boolean >::INParam rendertarget,
  KL::Traits< KL::Boolean >::INParam displayableOnHmd,
  KL::Traits< KL::ovrSize >::INParam size,
  KL::Traits< KL::SInt32 >::INParam mipLevels,
  KL::Traits< KL::SInt32 >::INParam sampleCount
) {
  ensureGlewIsInitialized();

  this_->handle = NULL;

  ovrSession * session_ = (ovrSession *)device->session->handle;
  if(!session_)
    return;

  ovrSizei size_;
  convert(size, size_);

  this_->handle = new TextureBuffer(*session_, rendertarget, displayableOnHmd, size_, mipLevels, NULL, sampleCount);
}

// Defined at src\\ovrGLTextureBuffer.kl:12:1
FABRIC_EXT_EXPORT void ovrGLTextureBuffer_Destruct(
  KL::Traits< KL::ovrGLTextureBuffer >::IOParam this_
) {
  if(!this_->handle)
    return;

  TextureBuffer * buffer = (TextureBuffer *)this_->handle;
  delete(buffer);
  this_->handle = NULL;
}

// Defined at src\\ovrGLTextureBuffer.kl:14:1
FABRIC_EXT_EXPORT void ovrGLTextureBuffer_GetSize(
  KL::Traits< KL::ovrSize >::Result _result,
  KL::Traits< KL::ovrGLTextureBuffer >::INParam this_
) {
  if(!this_->handle)
    return;
  TextureBuffer * buffer = (TextureBuffer *)this_->handle;
  ovrSizei result = buffer->GetSize();
  convert(result, _result);  
}

// Defined at src\\ovrGLTextureBuffer.kl:16:1
FABRIC_EXT_EXPORT void ovrGLTextureBuffer_SetAndClearRenderSurface(
  KL::Traits< KL::ovrGLTextureBuffer >::IOParam this_,
  KL::Traits< KL::ovrGLDepthBuffer >::INParam dbuffer
) {
  if(!this_->handle)
    return;
  if(!dbuffer->handle)
    return;
  TextureBuffer * buffer = (TextureBuffer *)this_->handle;
  DepthBuffer * dbuffer_ = (DepthBuffer *)dbuffer->handle;

  // Increment to use next texture, just before writing
  buffer->TextureSet->CurrentIndex = (buffer->TextureSet->CurrentIndex + 1) % buffer->TextureSet->TextureCount;
  buffer->SetAndClearRenderSurface(dbuffer_);
}

// Defined at src\\ovrGLTextureBuffer.kl:18:1
FABRIC_EXT_EXPORT void ovrGLTextureBuffer_UnsetRenderSurface(
  KL::Traits< KL::ovrGLTextureBuffer >::IOParam this_
) {
  if(!this_->handle)
    return;
  TextureBuffer * buffer = (TextureBuffer *)this_->handle;
  buffer->UnsetRenderSurface();  
}

// Defined at src\\ovrGLTextureBuffer.kl:20:1
FABRIC_EXT_EXPORT KL::SInt32 ovrGLTextureBuffer_GetTexId(
  KL::Traits< KL::ovrGLTextureBuffer >::INParam this_
) {
  if(!this_->handle)
    return 0;
  TextureBuffer * buffer = (TextureBuffer *)this_->handle;
  return buffer->texId;
}

// Defined at src\\ovrGLTextureBuffer.kl:22:1
FABRIC_EXT_EXPORT KL::SInt32 ovrGLTextureBuffer_GetFboId(
  KL::Traits< KL::ovrGLTextureBuffer >::INParam this_
) {
  if(!this_->handle)
    return 0;
  TextureBuffer * buffer = (TextureBuffer *)this_->handle;
  return buffer->fboId;
}
