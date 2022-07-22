#include "../Objects.h"

#include "Component/Attachable.h"
#include "Component/Drawable.h"
#include "Component/Motion.h"
#include "Component/Orientation.h"

#include "LTE/Color.h"
#include "LTE/DrawState.h"
#include "LTE/Matrix.h"
#include "LTE/Meshes.h"
#include "LTE/ParticleSystem.h"
#include "LTE/Pool.h"
#include "LTE/Renderable.h"
#include "LTE/Renderer.h"
#include "LTE/RenderStyle.h"
#include "LTE/RingBuffer.h"
#include "LTE/ShaderInstance.h"
#include "LTE/View.h"

typedef ObjectWrapper
  < Component_Attachable
  < Component_Drawable
  < Component_Orientation
  < ObjectWrapperTail<ObjectType_Trail>
  > > > >
  TrailBaseT;

namespace {
  struct SegmentData {
    Position p;
    V3 v;
    float opacity;
  };
}

AutoClassDerived(Trail, TrailBaseT,
  Color, color,
  float, size,
  uint, length,
  float, age)

  DERIVED_TYPE_EX(Trail)
  POOLED_TYPE

  RingBuffer<SegmentData> trail;

  struct RenderComponent : public RenderableT {
    DERIVED_TYPE(RenderComponent, RenderableT)
    Trail* self;

    RenderComponent(Trail* self) :
      self(self)
      {}

    void Render(DrawState* state) const {
      if (self->trail.size() < 2)
        return;

      static Shader shader;
      static ShaderInstance shaderState;
      if (!shader) {
        shader = Shader_Create("trail.jsl", "trail.jsl");
        shaderState = ShaderInstance_Create(shader);
        (*shaderState)
          (RenderStateSwitch_BlendModeAdditive)
          (RenderStateSwitch_ZWritableOff)
          ("maxAlpha", 1.0f);
      }

      const Distance cullDistance = 5000;
      Distance dist = Length(self->trail[0].p - state->view->transform.pos);
      if (dist > cullDistance * self->size)
        return;

      RenderStyle const& style = RenderStyle_Get();
      style->SetTransform(Transform_Identity());
      style->SetShader(shaderState);

      if (!style->WillRender())
        return;

      static Vector<Vertex> vertexArray;
      static Vector<ushort> indexArray;
      vertexArray.clear();
      indexArray.clear();

      V3 direction = 0;
      float totalLength = 0;

      for (size_t i = 0; i < self->trail.size(); ++i) {
        SegmentData const& segment = self->trail.GetRelative(i + 1);

        size_t indexOffset = vertexArray.size();
        Position const& p = segment.p;
        float const& opacity = segment.opacity;

        if (i + 1 < self->trail.size()) {
          SegmentData const& lastSegment = self->trail.GetRelative(i + 2);
          Position const& pLast = lastSegment.p;
          direction = pLast - p;
        }

        for (uint j = 0; j < 2; ++j) {
          Vertex v;
          v.p = p - state->view->transform.pos;
          v.n = direction;
          v.u = (j ? 1.0f : -1.0f);
          v.v = opacity;
          vertexArray.push(v);
        }

        if (i + 1 < self->trail.size()) {
          indexArray.push((short)(0 + indexOffset));
          indexArray.push((short)(1 + indexOffset));
          indexArray.push((short)(3 + indexOffset));
          indexArray.push((short)(0 + indexOffset));
          indexArray.push((short)(3 + indexOffset));
          indexArray.push((short)(2 + indexOffset));
        }
        
        totalLength += Length(direction);
      }

      (*shader)
        ("color", self->color)
        ("size", self->size)
        ("totalLength", totalLength);
      DrawState_Link(shader);

      shaderState->Begin();
      Renderer_DrawVertices(vertexArray, indexArray);
      shaderState->End();
    }
  };

  Trail() :
    length(0),
    age(0)
  {
    Drawable.renderable = (Renderable)(new RenderComponent(this));
  }

  void OnUpdate(UpdateState& state) {
    BaseType::OnUpdate(state);
    age += state.dt;

    if (!trail.size()) {
      trail.resize(length);
      Position pos = GetPos();
      for (size_t i = 0; i < length; ++i) {
        SegmentData& segment = trail[i];
        segment.p = pos;
        segment.opacity = 0;
      }
    }

    bool faded = true;
    float delta = 1.0f / (float)(length - 1);
    for (size_t i = 0; i < length; ++i) {
      SegmentData& segment = trail.GetRelative(i);
      segment.opacity += delta;
      if (segment.opacity > 0)
        faded = false;
    }

    if (parent && parent->CanMove()) {
      trail.Advance();
      SegmentData& segment = trail.GetCurrent();
      segment.p = GetPos();
      segment.opacity = 0;
    } else if (faded) {
      Delete();
      return;
    }
  }
};

DERIVED_IMPLEMENT(Trail)

Object Object_Trail(
  Object const& parent,
  int length,
  Color const& color,
  float size)
{
  Reference<Trail> self = new Trail;
  self->length = length;
  self->color = color;
  self->size = size;

  /* TODO : Detach upon parent deletion so trail persists. */
  parent->AddChild(self);
  return self;
}
