#ifndef LTE_RenderPasses_h__
#define LTE_RenderPasses_h__

#include "DeclareFunction.h"
#include "RenderPass.h"
#include "String.h"

DeclareFunction(RenderPass_Aberration, RenderPass,
  float, strength)

DeclareFunction(RenderPass_Composite, RenderPass,
  Vector<RenderPass>, passes)

DeclareFunction(RenderPass_Bloom, RenderPass,
  int, radius,
  float, variance)

DeclareFunction(RenderPass_BloomLight, RenderPass,
  int, radius)

DeclareFunction(RenderPass_CustomFilter, RenderPass,
  Data, data)

DeclareFunctionNoParams(RenderPass_MotionBlur, RenderPass)

DeclareFunction(RenderPass_PostFilter, RenderPass,
  String, shaderPath)

DeclareFunction(RenderPass_RadialBlur, RenderPass,
  V2, center,
  float, radius,
  float, strength,
  float, falloff)

DeclareFunctionNoParams(RenderPass_Tonemap, RenderPass)

#endif
