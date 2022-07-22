#include "Game/Camera.h"
#include "LTE/Function.h"
#include "LTE/View.h"
#include "LTE/Viewport.h"
#include "LTE/Window.h"

TypeAlias(Reference<CameraT>, Camera);

DefineConversion(camera_to_object, Camera, Object) {
  dest = (Object)src;
}

FreeFunction(RayD, Camera_GetRay,
  "Return the world-space ray that projects to 'position' under 'camera'",
  Camera, camera,
  V2, position)
{
  return camera->GetView(Viewport_Get()->GetAspect()).GetRay(Viewport_Get()->ToNDC(position));
} FunctionAlias(Camera_GetRay, GetRay);

FreeFunction(Object, Camera_GetTarget,
  "Return the object that 'camera' is tracking",
  Camera, camera)
{
  return camera->GetTarget();
} FunctionAlias(Camera_GetTarget, GetTarget);

FreeFunction(Position, Camera_Project,
  "Return the projected position of 'position' from the viewpoint of 'camera'",
  Camera, camera,
  Position, position)
{
  return camera->GetView(Window_Get()->GetAspect()).Project(position);
} FunctionAlias(Camera_Project, Project);

VoidFreeFunctionNoParams(Camera_Pop,
  "Pop the last camera from the global camera stack")
{
  Camera_Pop();
}

VoidFreeFunction(Camera_Push,
  "Push 'camera' to the global camera stack",
  Camera, camera)
{
  Camera_Push(camera);
} FunctionAlias(Camera_Push, Push);

VoidFreeFunction(Camera_SetFov,
  "Set 'camera's vertical field of view to 'fov' degrees",
  Camera, camera,
  float, fov)
{
  camera->SetFov(Radians(fov));
} FunctionAlias(Camera_SetFov, SetFov);

VoidFreeFunction(Camera_SetRelativePos,
  "Set 'camera's position to 'position' relative to the target",
  Camera, camera,
  Position, position)
{
  camera->SetRelativePos(position);
} FunctionAlias(Camera_SetRelativePos, SetRelativePos);

VoidFreeFunction(Camera_SetRelativeLookAt,
  "Set 'camera' to look at 'position' relative to the target",
  Camera, camera,
  Position, lookAt)
{
  camera->SetRelativeLookAt(lookAt);
} FunctionAlias(Camera_SetRelativeLookAt, SetRelativeLookAt);

VoidFreeFunction(Camera_SetRelativeUp,
  "Set 'camera's up direction 'up' relative to the target",
  Camera, camera,
  V3, up)
{
  camera->SetRelativeUp(up);
} FunctionAlias(Camera_SetRelativeUp, SetRelativeUp);

VoidFreeFunction(Camera_SetRigidity,
  "Set 'camera's to have 'rigidity'",
  Camera, camera,
  float, rigidity)
{
  camera->SetRigidity(rigidity);
} FunctionAlias(Camera_SetRigidity, SetRigidity);

VoidFreeFunction(Camera_SetTarget,
  "Set 'camera' to track 'target'",
  Camera, camera,
  Object, target)
{
  camera->SetTarget(target);
} FunctionAlias(Camera_SetTarget, SetTarget);

FreeFunction(V2, Position_Frame,
  "Return the screen-space coordinate of the projected 'position'",
  Position, position)
{
  return Viewport_Get()->FromNDC(position.GetXY());
}
