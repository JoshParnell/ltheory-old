#ifndef Graphics_Renderables_h__
#define Graphics_Renderables_h__

#include "LTE/DeclareFunction.h"
#include "LTE/Generic.h"

DeclareFunctionArgBind(Renderable_Asteroid, Renderable,
  uint, seed)

DeclareFunctionArgBind(Renderable_Ice, Renderable,
  uint, seed)

DeclareFunctionArgBind(Renderable_Imposter, Renderable,
  Renderable, source)

DeclareFunctionArgBind(Renderable_Starfield, Renderable,
  uint, seed,
  uint, starCount)

#endif
