#ifndef UI_Compositors_h__
#define UI_Compositors_h__

#include "Common.h"
#include "Compositor.h"
#include "LTE/DeclareFunction.h"

DeclareFunctionNoParams(Compositor_None, Compositor)

DeclareFunction(Compositor_Basic, Compositor,
  float, noise,
  float, lines,
  V3, gradeBlue)

DeclareFunction(Compositor_Custom, Compositor,
  Compositor, base,
  Data, data)

#endif
