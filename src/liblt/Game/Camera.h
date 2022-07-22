#ifndef Camera_h__
#define Camera_h__

#include "ObjectWrapper.h"

#include "Component/Motion.h"
#include "Component/Orientation.h"
#include "LTE/DeclareFunction.h"
#include "Module/Common.h"

typedef ObjectWrapper
  < Component_Motion
  < Component_Orientation
  < ObjectWrapperTail<ObjectType_Camera>
  > > >
  CameraBaseT;

struct CameraT : public CameraBaseT {
  BASE_TYPE_EX(CameraT)

  virtual Object const& GetTarget() const = 0;
  virtual View GetView(float aspect) const = 0;

  virtual void SetFov(float fov) = 0;
  virtual void SetRelativePos(V3 const& pos) = 0;
  virtual void SetRelativeLookAt(V3 const& lookAt) = 0;
  virtual void SetRelativeUp(V3 const& lookAt) = 0;
  virtual void SetRigidity(float rigidity) = 0;
  virtual void SetTarget(Object const&) = 0;
};

DeclareFunction(Camera_CanSee, bool,
  Object, object,
  float, maxDistance)

DeclareFunctionNoParams(Camera_Create, Camera)
DeclareFunctionNoParams(Camera_Get, Camera)

LT_API void Camera_Pop();
LT_API void Camera_Push(Camera const&);

#endif
