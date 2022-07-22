#include "../Objects.h"

#include "Component/Attachable.h"
#include "Component/Drawable.h"
#include "Component/Orientation.h"

#include "Game/Light.h"

#include "LTE/DrawState.h"
#include "LTE/Matrix.h"
#include "LTE/Math.h"
#include "LTE/Meshes.h"
#include "LTE/Model.h"
#include "LTE/Pool.h"
#include "LTE/Renderable.h"
#include "LTE/RenderStyle.h"
#include "LTE/ShaderInstance.h"
#include "LTE/View.h"

typedef ObjectWrapper
  < Component_Attachable
  < Component_Drawable
  < Component_Orientation
  < ObjectWrapperTail<ObjectType_Explosion>
  > > > >
  ExplosionBaseT;

AutoClassDerived(Explosion, ExplosionBaseT,
  LightRef, light,
  ExplosionType, type,
  float, age,
  float, duration)

  DERIVED_TYPE_EX(Explosion)
  POOLED_TYPE

  struct RenderComponent : public RenderableT {
    DERIVED_TYPE(RenderComponent, RenderableT)
    Explosion* self;

    RenderComponent(Explosion* self) :
      self(self)
      {}

    Bound3 GetBound() const {
      return Bound3(V3(-1), V3(1));
    }

    void Render(DrawState* state) const {
      if (self->age < 0)
        return;

      /* Distance culling. */
      const float cullDistance = 1000;
      Transform const& transform = self->GetTransform();

      if (Length(state->view->transform.pos - transform.pos) > cullDistance * self->GetRadius())
        return;

      static Model model[ExplosionType_SIZE];
      static ShaderInstance ss[ExplosionType_SIZE];

      if (!model[0]) {
        char const* shaderPath[ExplosionType_SIZE] = {
          "explosion.jsl",
          "shield_explosion.jsl",
        };

        for (int i = 0; i < ExplosionType_SIZE; ++i) {
          Shader shader = Shader_Create("billboard_soft.jsl", shaderPath[i]);
          ss[i] = ShaderInstance_Create(shader);
          (*ss[i])
            ("zOffset", 30.0f)
            (RenderStateSwitch_BlendModeAdditive)
            (RenderStateSwitch_ZWritableOff);
          DrawState_Link(ss[i]);
          model[i] = Model_Create()->Add(Mesh_Billboard(), ss[i]);
        }
      }

      const Color kExplosionColor[ExplosionType_SIZE][2] = {
        { Color(1.0f, 0.2f, 0.1f), Color(1.8f, 0.8f, 0.5f) },
        { Color(0.1f, 0.2f, 1.0f), Color(0.3f, 0.8f, 1.8f) },
      };

      (*ss[self->type])
        ("billboardSize", 30.0f * transform.scale.GetMax())
        ("color1", kExplosionColor[self->type][0])
        ("color2", kExplosionColor[self->type][1])
        ("opacity", self->GetOpacity())
        ("age", self->age);

      RenderStyle_Get()->SetTransform(transform);
      model[self->type]->Render(state);
    }
  };

  Explosion() :
    age(0),
    duration(1)
  {
    Drawable.renderable = (Renderable)(new RenderComponent(this));
  }

  float GetOpacity() const {
    return Sqrt(Saturate(1.f - age / duration));
  }

  void OnUpdate(UpdateState& state) {
    BaseType::OnUpdate(state);

    age += state.dt;
    if (age > duration) {
      Delete();
      return;
    }

    if (!light)
      light = Light_Create(this);
    light->color = GetOpacity() * Color(2.8f, 1.0f, 0.5f);
  }
};

DERIVED_IMPLEMENT(Explosion)

DefineFunction(Object_Explosion) {
  Reference<Explosion> self = new Explosion;
  self->type = args.type;
  self->age = args.age;
  self->duration = args.duration;
  return self;
}
