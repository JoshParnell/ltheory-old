#include "ParticleSystem.h"

#include "DrawState.h"
#include "Map.h"
#include "Meshes.h"
#include "Renderer.h"
#include "RenderStyle.h"
#include "ShaderInstance.h"
#include "Texture2D.h"
#include "Vector.h"
#include "View.h"

#include <algorithm>
#include "Debug.h"

const size_t kMaxParticles = 1024 * 1024;
const float kLodFactor = Squared(512);

namespace {
  Vector<ParticleSystem>& GetStack() {
    static Vector<ParticleSystem> stack;
    return stack;
  }

  Mesh gParticleMesh = Mesh_Billboard(-1, 1, -1, 1);

  struct Particle {
    V3D p;
    V3 v;
    V3 attrib;
    float size;
    float maxLife;
    float life;

    Particle(
        V3D const& p,
        V3 const& v,
        float size,
        float maxLife,
        V3 const& attrib) :
      p(p),
      v(v),
      attrib(attrib),
      size(size),
      maxLife(maxLife),
      life(maxLife)
      {}

    friend bool operator<(Particle const& a, Particle const& b) {
      return b.life < a.life;
    }
  };

  AutoClass(ParticleVertex,
    V3, p,
    V3, n,
    V2, uv,
    V3, c)
    ParticleVertex() {}
  };

  AutoClassDerivedEmpty(ParticleSystemImpl, ParticleSystemT)
    typedef Map<ShaderInstance, Vector<Particle> > ParticleMapT;
    ParticleMapT particles;
    DERIVED_TYPE_EX(ParticleSystemImpl)

    ParticleSystemImpl() {}

    void Draw(DrawState* state) const {
      RenderStyle const& style = RenderStyle_Get();

      for (ParticleMapT::const_iterator it = particles.begin();
           it != particles.end();
           ++it)
      {
        ShaderInstance const& shader = it->first;
        Vector<Particle> const& particles = it->second;

        style->SetTransform(Transform_Identity());
        style->SetShader(shader);

        if (!style->WillRender())
          continue;

        size_t vertexCount = gParticleMesh->vertices.size() * particles.size();
        size_t indexCount = gParticleMesh->indices.size() * particles.size();

        static Vector<ParticleVertex> vertices;
        static Vector<uint> indices;
        vertices.clear();
        indices.clear();
        vertices.reserve(vertexCount);
        indices.reserve(indexCount);

        for (size_t i = 0; i < particles.size(); ++i) {
          Particle const& particle = particles[i];
          if (!state->view->CanSee(particle.p))
            continue;

          V3D position = particle.p - state->view->transform.pos;
          float d2 = LengthSquared(position);
          float r2 = Squared(particle.size);
          if (d2 >= kLodFactor * r2)
            continue;

          float age = (particle.maxLife - particle.life) / particle.maxLife;
          for (size_t j = 0; j < gParticleMesh->indices.size(); ++j)
            indices.push((uint)(gParticleMesh->indices[j] + vertices.size()));

          for (size_t j = 0; j < gParticleMesh->vertices.size(); ++j) {
            Vertex& sourceVertex = gParticleMesh->vertices[j];
            vertices.push(ParticleVertex());
            ParticleVertex& v = vertices.back();
            v.p = position;
            v.n.x = particle.size;
            v.n.y = age;
            v.uv.x = sourceVertex.u;
            v.uv.y = sourceVertex.v;
            v.c = particle.attrib;
          }
        }

        DrawState_Link(shader);
        shader->Begin();
        state->primary->Bind(0);

        Renderer_DrawVertices(
          vertices.data(),
          Type_Get<ParticleVertex>(),
          indices.data(),
          indices.size(),
          GL_IndexFormat::Int);

        state->primary->Unbind();
        shader->End();
      }
    }

    void Run(float dt) {
      for (ParticleMapT::iterator it = particles.begin();
           it != particles.end();
           ++it)
      {
        Vector<Particle>& particles = it->second;

        /* Particle update. */
        for (int i = 0; i < (int)particles.size(); ++i) {
          Particle& particle = particles[i];
          particle.p += dt * particle.v;
          particle.life -= dt;
          if (particle.life <= 0.0f) {
            particles.removeIndex(i);
            i--;
            continue;
          }
        }

        /* Enforce max particle count. */
        if (particles.size() > kMaxParticles) {
          // std::sort(particles.v.begin(), particles.v.end());
          while (particles.size() > kMaxParticles)
            particles.pop();
        }
      }
    }
  };
}

DefineFunction(ParticleSystem_Add) {
  ((ParticleSystemImpl*)args.particleSystem.t)
    ->particles[args.particle].push(Particle(
    args.position,
    args.velocity,
    args.scale,
    args.life,
    args.attribute));
} FunctionAlias(ParticleSystem_Add, Add);

DefineFunction(ParticleSystem_Create) {
  return new ParticleSystemImpl;
}

DefineFunction(ParticleSystem_Get) {
  return GetStack().size() ? GetStack().back() : nullptr;
}

DefineFunction(ParticleSystem_Pop) {
  GetStack().pop();
}

DefineFunction(ParticleSystem_Push) {
  GetStack().push(args.ps);
}
