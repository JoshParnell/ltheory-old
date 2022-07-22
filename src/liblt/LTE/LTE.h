#ifndef LTE_H
#define LTE_H

#include "Common.h"

/*
 *  LTE ~ Limit Theory Game Engine ---------------------------------------------
 *    Josh Parnell, 2012 - 2014
 *
 */

#if !defined(LTE_CONSOLE) && defined(LIBLT_WINDOWS)
  
  #pragma comment(linker, "/SUBSYSTEM:windows")
  #ifndef NOMINMAX
    #define NOMINMAX
  #endif

  #include <windows.h>
  int main(int argc, char const* argv[]);
  int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrev, PSTR cmd, int show) {
    char const* argv[] = { "program" };
    return main(0, argv);
  }

#endif

#include "Archive.h"
#include "Array.h"
#include "Axis.h"
#include "AutoClass.h"
#include "AutoPtr.h"
#include "BaseType.h"
#include "Button.h"
#include "Call.h"
#include "CollisionMesh.h"
#include "Config.h"
#include "Debug.h"
#include "DeclareFunction.h"
#include "Diff.h"
#include "Enum.h"
#include "Expression.h"
#include "Font.h"
#include "Function.h"
#include "Generic.h"
#include "Geom.h"
#include "Geometry.h"
#include "GL.h"
#include "Grammar.h"
#include "HashSet.h"
#include "InternalList.h"
#include "Job.h"
#include "Joystick.h"
#include "Keyboard.h"
#include "List.h"
#include "ListElement.h"
#include "Location.h"
#include "Map.h"
#include "Math.h"
#include "Matrix.h"
#include "Meshes.h"
#include "Mouse.h"
#include "Model.h"
#include "Module.h"
#include "OS.h"
#include "Patch.h"
#include "Plane.h"
#include "Pointer.h"
#include "Pool.h"
#include "Profiler.h"
#include "Program.h"
#include "ProgramLog.h"
#include "RNG.h"
#include "Ray.h"
#include "Reference.h"
#include "RenderPass.h"
#include "RenderPasses.h"
#include "RenderStyle.h"
#include "Renderer.h"
#include "Script.h"
#include "SDF.h"
#include "SDFMesh.h"
#include "Shader.h"
#include "ShaderInstance.h"
#include "Smooth.h"
#include "SpatialPartition.h"
#include "Sphere.h"
#include "SphereTree.h"
#include "Stack.h"
#include "StackFrame.h"
#include "StringList.h"
#include "StringTree.h"
#include "Text.h"
#include "Texture2D.h"
#include "Texture3D.h"
#include "Thread.h"
#include "Timer.h"
#include "Type.h"
#include "Tuple.h"
#include "Window.h"
#include "Viewport.h"
#include "Vector.h"
#include "VectorNP.h"
#include "VectorMap.h"
#include "V2.h"
#include "V3.h"
#include "V4.h"

#endif
