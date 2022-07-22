#include "../Objects.h"

#include "Component/BoundingBox.h"
#include "Component/Drawable.h"
#include "Component/Orientation.h"

#include "LTE/DrawState.h"
#include "LTE/Meshes.h"
#include "LTE/Model.h"
#include "LTE/SDFMesh.h"
#include "LTE/ShaderInstance.h"

namespace {
  Renderable GetModel() {
    static Renderable model;
    if (!model) {
      ShaderInstance shader = ShaderInstance_Create("npm.jsl", "warprail.jsl");
      (*shader)
        (RenderStateSwitch_BlendModeAdditive)
        (RenderStateSwitch_CullModeDisabled)
        (RenderStateSwitch_ZWritableOff);
      DrawState_Link(shader);
      model = (Renderable)Model_Create()->Add(Mesh_Cylinder(50)
        ->Scale(V3(1, 0.5f, 1))
        ->Translate(V3(0, 0.5f, 0))
        ->Rotate(V3(0, kPi2, 0)), shader);
    }
    return model;
  }
}

typedef ObjectWrapper
  < Component_BoundingBox
  < Component_Drawable
  < Component_Orientation
  < ObjectWrapperTail<ObjectType_WarpRail>
  > > > >
  WarpRailBaseT;

AutoClassDerived(WarpRail, WarpRailBaseT,
  Object, node1,
  Object, node2)
  DERIVED_TYPE_EX(WarpRail)

  WarpRail() {
    // Drawable.renderable = GetModel;
  }
};

DERIVED_IMPLEMENT(WarpRail)

DefineFunction(Object_WarpRail) {
  Reference<WarpRail> self = new WarpRail;
  self->node1 = args.node1;
  self->node2 = args.node2;
  return self;
}
