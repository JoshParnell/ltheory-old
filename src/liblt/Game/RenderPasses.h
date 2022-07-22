#ifndef RenderPasses_h__
#define RenderPasses_h__

#include "Game/Common.h"
#include "LTE/DeclareFunction.h"
#include "LTE/RenderPass.h"
#include "LTE/V3.h"
#include "LTE/V4.h"

DeclareFunctionNoParams(RenderPass_Blended, RenderPass)

DeclareFunction(RenderPass_Camera, RenderPass,
  Camera, camera)

DeclareFunction(RenderPass_Clear, RenderPass,
  V4, value)

DeclareFunctionNoParams(RenderPass_ClearDepth, RenderPass)

DeclareFunctionNoParams(RenderPass_DepthPrepass, RenderPass)

DeclareFunctionNoParams(RenderPass_DustClouds, RenderPass)

DeclareFunctionNoParams(RenderPass_GBuffer, RenderPass)

DeclareFunctionNoParams(RenderPass_GlobalLighting, RenderPass)

DeclareFunctionNoParams(RenderPass_LocalLighting, RenderPass)

DeclareFunctionNoParams(RenderPass_LensFlares, RenderPass)

DeclareFunctionNoParams(RenderPass_Particles, RenderPass)

DeclareFunctionNoParams(RenderPass_SMAA, RenderPass)

DeclareFunctionNoParams(RenderPass_SSAO, RenderPass)

DeclareFunctionNoParams(RenderPass_Visibility, RenderPass)

#endif
