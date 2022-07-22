#include "../Objects.h"

#include "Component/BoundingBox.h"
#include "Component/Detectable.h"
#include "Component/Drawable.h"
#include "Component/Nameable.h"
#include "Component/Navigable.h"
#include "Component/Orientation.h"
#include "Component/Zoned.h"

#include "LTE/DrawState.h"
#include "LTE/Meshes.h"
#include "LTE/Model.h"
#include "LTE/Pool.h"
#include "LTE/Script.h"
#include "LTE/SDFMesh.h"
#include "LTE/SDFs.h"
#include "LTE/ShaderInstance.h"

#include "UI/Icon.h"

namespace {
  Renderable GetWormholeModel() {
    static Renderable model;
    if (!model) {
      ShaderInstance ss = ShaderInstance_Create("npm.jsl", "wormhole.jsl");
      (*ss)(RenderStateSwitch_BlendModeAdditive);
      DrawState_Link(ss);
      model = (Renderable)Model_Create()
        ->Add(Mesh_BoxSphere(5, true)->ReverseWinding(), ss);
    }
    return model;
  }
}

typedef ObjectWrapper
  < Component_BoundingBox
  < Component_Detectable
  < Component_Drawable
  < Component_Nameable
  < Component_Navigable
  < Component_Orientation
  < Component_Zoned
  < ObjectWrapperTail<ObjectType_Wormhole>
  > > > > > > > >
  WormholeBaseT;

AutoClassDerived(Wormhole, WormholeBaseT,
  Object, tunnel)

  DERIVED_TYPE_EX(Wormhole)
  POOLED_TYPE

  Wormhole() {
    Drawable.renderable = GetWormholeModel;
  }

  void Dock(Object const& docker) {
    Object const& dest = Navigable.nodes[0].dest;
    dest->GetContainer()->AddInterior(docker);
    docker->SetPos(
      dest->GetPos() - 1500.0f * Normalize(dest->GetPos()));
  }

  Icon GetIcon() const {
    Icon icon;
    ScriptFunction_Load("Icons:Wormhole")->Call(icon);
    return icon;
  }

  Signature GetSignature() const {
    return Signature(10.0f, 18.0f, 0.125f, 0.5f);
  }
};

DERIVED_IMPLEMENT(Wormhole)

DefineFunction(Object_Wormhole) {
  Reference<Wormhole> self = new Wormhole;
  self->Zoned.region = SDF_Sphere(0, 10);
  return self;
}

void Object_Wormholes(ObjectT* o1, ObjectT* o2) {
  Object a = Object_Wormhole();
  Object b = Object_Wormhole();

  a->GetNavigable()->nodes.push(NavigableNode(b, 0));
  b->GetNavigable()->nodes.push(NavigableNode(a, 0));

  a->SetName(o2->GetName() + " Wormhole");
  b->SetName(o1->GetName() + " Wormhole");

  V3 direction = Normalize(o2->GetPos() - o1->GetPos());
  a->SetPos( 400000.0f * direction);
  b->SetPos(-400000.0f * direction);
  a->SetScale(1000);
  b->SetScale(1000);

  o1->AddInterior(a);
  o2->AddInterior(b);
}
