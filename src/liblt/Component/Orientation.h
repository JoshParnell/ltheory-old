#ifndef Component_Orientation_h__
#define Component_Orientation_h__

#include "Common.h"
#include "Game/Item.h"
#include "LTE/AutoClass.h"
#include "LTE/Transform.h"
#include "LTE/V3.h"

AutoClass(ComponentOrientation,
  Transform, transform)

  short version;
  short myVersion;

  ComponentOrientation() :
    version(1),
    myVersion(0)
    {}

  Transform const& GetTransform() const {
    ((ComponentOrientation*)this)->UpdateTransform();
    return transform;
  }

  Transform& GetTransformW() {
    version++;
    return transform;
  }

  void Run(ObjectT* self, UpdateState& state) {
    UpdateTransform();
  }

  void UpdateTransform() {
    if (myVersion != version) {
      transform.Orthogonalize();
      myVersion = ++version;
    }
  }

  LT_API void Rotate(V3 const& lookUpRight);
};

AutoComponent(Orientation)
  void OnUpdate(UpdateState& s) {
    Orientation.Run(this, s);
    BaseT::OnUpdate(s);
  }

  void SetSupertype(Item const& type) {
    if (type->GetScale() > 0)
      SetScale(V3(type->GetScale()));

    BaseT::SetSupertype(type);
  }

  void SetLook(V3 const& look) {
    Orientation.GetTransformW().look = look;
  }

  void SetUp(V3 const& up) {
    Orientation.GetTransformW().up = up;
  }

  void SetPos(Position const& p) {
    Orientation.GetTransformW().pos = p;
  }

  void SetScale(V3 const& scale) {
    Orientation.GetTransformW().scale = scale;
  }

  Transform const& GetTransform() const {
    return Orientation.GetTransform();
  }
};

#endif
