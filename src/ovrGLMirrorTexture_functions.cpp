/*
 *  Copyright 2010-2014 Fabric Software Inc. All rights reserved.
 */

#include "ovrGLMirrorTexture_impl.h"
#include "ovrGLMirrorTexture_functions.h"

using namespace Fabric::EDK;

// Defined at src\\ovrGLMirrorTexture.kl:10:1
FABRIC_EXT_EXPORT void ovrGLMirrorTexture_Construct(
  KL::Traits< KL::ovrGLMirrorTexture >::IOParam this_,
  KL::Traits< KL::ovrDevice >::INParam device,
  KL::Traits< KL::ovrSize >::INParam size
) {
  ensureGlewIsInitialized();

  this_->handle = NULL;

  ovrSession * session_ = (ovrSession *)device->session->handle;
  if(!session_)
    return;

  ovrSizei size_;
  convert(size, size_);

  this_->handle = new MirrorTexture(*session_, size_);
}

// Defined at src\\ovrGLMirrorTexture.kl:12:1
FABRIC_EXT_EXPORT void ovrGLMirrorTexture_Destruct(
  KL::Traits< KL::ovrGLMirrorTexture >::IOParam this_
) {
  if(!this_->handle)
    return;

  MirrorTexture * texture = (MirrorTexture *)this_->handle;
  delete(texture);
  this_->handle = NULL;
}

// Defined at src\\ovrGLMirrorTexture.kl:14:1
FABRIC_EXT_EXPORT void ovrGLMirrorTexture_GetSize(
  KL::Traits< KL::ovrSize >::Result _result,
  KL::Traits< KL::ovrGLMirrorTexture >::INParam this_
) {
  if(!this_->handle)
    return;
  MirrorTexture * texture = (MirrorTexture *)this_->handle;
  ovrSizei result = texture->GetSize();
  convert(result, _result);  
}

// Defined at src\\ovrGLMirrorTexture.kl:16:1
FABRIC_EXT_EXPORT void ovrGLMirrorTexture_BlitTexture(
  KL::Traits< KL::ovrGLMirrorTexture >::IOParam this_
) {
  if(!this_->handle)
    return;
  MirrorTexture * texture = (MirrorTexture *)this_->handle;
  texture->BlitTexture();
}

// Defined at src\\ovrGLMirrorTexture.kl:20:1
FABRIC_EXT_EXPORT KL::SInt32 ovrGLMirrorTexture_GetTexId(
  KL::Traits< KL::ovrGLMirrorTexture >::INParam this_
) {
  if(!this_->handle)
    return 0;
  MirrorTexture * texture = (MirrorTexture *)this_->handle;
  return texture->texId;
}

// Defined at src\\ovrGLMirrorTexture.kl:22:1
FABRIC_EXT_EXPORT KL::SInt32 ovrGLMirrorTexture_GetFboId(
  KL::Traits< KL::ovrGLMirrorTexture >::INParam this_
) {
  if(!this_->handle)
    return 0;
  MirrorTexture * texture = (MirrorTexture *)this_->handle;
  return texture->fboId;
}
