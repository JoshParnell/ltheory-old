#ifndef Component_Attachable_h__
#define Component_Attachable_h__

#include "Common.h"
#include "Game/Item.h"
#include "LTE/AutoClass.h"
#include "LTE/Transform.h"

AutoClass(ComponentAttachable,
  Transform, transform,
  short, parentVersion,
  bool, moved)

  ComponentAttachable() :
    parentVersion(0),
    moved(false)
    {}

  void SetPos(Position const& pos) {
    moved = true;
    transform.pos = pos;
  }
  
  void SetLook(V3 const& look) {
    moved = true;
    transform.look = look;
  }
  
  void SetScale(V3 const& scale) {
    moved = true;
    transform.scale = scale;
  }

  void SetTransform(Transform const& transform) {
    moved = true;
    this->transform = transform;
  }
  
  void SetUp(V3 const& up) {
    moved = true;
    transform.up = up;
  }

  void Run(ObjectT* self, UpdateState& state) {
    UpdateTransform(self);
  }

  LT_API void UpdateTransform(ObjectT* self);
};

AutoComponent(Attachable)
  void OnUpdate(UpdateState& s) {
    Attachable.Run(this, s);
    BaseT::OnUpdate(s);
  }

  Transform const& GetLocalTransform() const {
    return Attachable.transform;
  }

  void SetSupertype(Item const& type) {
    if (type->GetScale() > 0)
      Attachable.SetScale(V3(type->GetScale()));

    BaseT::SetSupertype(type);
  }

  void SetLocalTransform(Transform const& transform) {
    Attachable.SetTransform(transform);
  }
};

#endif
