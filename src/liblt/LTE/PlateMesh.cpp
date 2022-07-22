#include "PlateMesh.h"

#include "Matrix.h"
#include "Meshes.h"
#include "StackFrame.h"
#include "Warp.h"

#include "Array.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture2D.h"

namespace {
  AutoClass(Plate,
    V3, center,
    V3, scale,
    V3, rotation,
    float, bevel)
    Plate() {}
  };

  AutoClassDerived(PlateMeshImpl, PlateMeshT,
    Vector<Plate>, plates,
    Vector<PlateMesh>, children,
    Vector<Matrix>, childTransforms,
    Vector<Warp>, warps,
    uint, quality)
    DERIVED_TYPE_EX(PlateMeshImpl)

    PlateMeshImpl() {}

    void Add(
      V3 const& center,
      V3 const& scale,
      V3 const& rotation,
      float bevel)
    {
      plates.push(Plate(center, scale, rotation, bevel));
    }

    void Add(Warp const& warp) {
      warps.push(warp);
    }

    void Add(
      PlateMesh const& pm,
      V3 const& offset = 0,
      V3 const& scale = 1)
    {
      children.push(pm);
      childTransforms.push(
        Matrix::Translation(offset) *
        Matrix::Scale(scale));
    }

    Mesh GetMesh() const {
      Mesh self = Mesh_Create();

      for (size_t i = 0; i < plates.size(); ++i) {
        Plate const& plate = plates[i];
        self->AddMesh(
          Mesh_BoxRounded(quality, plate.bevel),
          Matrix::Translation(plate.center) *
          Matrix::RotationYPR(plate.rotation) *
          Matrix::Scale(plate.scale));
      }

      for (size_t i = 0; i < children.size(); ++i)
        self->AddMesh(children[i]->GetMesh(), childTransforms[i]);

      for (size_t i = 0; i < self->vertices.size(); ++i) {
        V3& p = self->vertices[i].p;
        V3 delta = 0;
        for (size_t j = 0; j < warps.size(); ++j)
          delta += warps[j]->GetDelta(p);
        p += delta;
      }

      self->ComputeNormals();
      return self;
    }

    void ReflectX() {
      for (size_t i = 0; i < plates.size(); ++i) {
        plates[i].center.x *= -1;
        plates[i].rotation.z *= -1;
      }
    }

    void ReflectY() {
      for (size_t i = 0; i < plates.size(); ++i) {
        plates[i].center.y *= -1;
        plates[i].rotation.y *= -1;
      }
    }

    void ReflectZ() {
      for (size_t i = 0; i < plates.size(); ++i) {
        plates[i].center.z *= -1;
        plates[i].rotation.x *= -1;
      }
    }
  };
}

DefineFunction(PlateMesh_Create) {
  Reference<PlateMeshImpl> self = new PlateMeshImpl;
  self->quality = args.quality;
  return self;
}

DefineFunction(Mesh_ComputeOcclusion) {
  AUTO_FRAME;
  Mesh const& m = args.mesh;

  Vector<V4> points;
  Vector<V4> normals;

  /* Fill surfel buffers. */
  for (size_t i = 0; i < m->indices.size(); i += 3) {
    Vertex const& v0 = m->vertices[m->indices[i + 0]];
    Vertex const& v1 = m->vertices[m->indices[i + 1]];
    Vertex const& v2 = m->vertices[m->indices[i + 2]];

    V3 normal = Cross(v2.p - v0.p, v1.p - v0.p);
    float area = 0.5f * Length(normal) / kPi;
    points.push(V4((v0.p + v1.p + v2.p) / 3.0f, area));
    normals.push(V4(Normalize(normal), 0.0f));
  }

  int sDim = (int)Ceil(Sqrt((float)points.size()));

  while ((int)points.size() < sDim * sDim) {
    points.push(V4(0));
    normals.push(V4(0));
  }

  Texture2D sPointBuffer =
    Texture_Create(sDim, sDim, GL_TextureFormat::RGBA32F, points.data());
  Texture2D sNormalBuffer =
    Texture_Create(sDim, sDim, GL_TextureFormat::RGBA32F, normals.data());

  /* Fill vertex buffers. */
  points.clear();
  normals.clear();
  for (size_t i = 0; i < m->vertices.size(); ++i) {
    points.push(V4(m->vertices[i].p, 0));
    normals.push(V4(m->vertices[i].n, 0));
  }
  
  int vDim = (int)Ceil(Sqrt((float)points.size()));

  while ((int)points.size() < vDim * vDim) {
    points.push(V4(0));
    normals.push(V4(0));
  }

  Texture2D vPointBuffer =
    Texture_Create(vDim, vDim, GL_TextureFormat::RGBA32F, points.data());
  Texture2D vNormalBuffer = 
    Texture_Create(vDim, vDim, GL_TextureFormat::RGBA32F, normals.data());

  /* GPU computation. */
  Texture2D occlusionBuffer =
    Texture_Create(vDim, vDim, GL_TextureFormat::R32F);
  static Shader shader = Shader_Create("identity.jsl", "compute/occlusion.jsl");

  occlusionBuffer->Bind(0);
  (*shader)
    ("sDim", sDim)
    ("sPointBuffer", sPointBuffer)
    ("sNormalBuffer", sNormalBuffer)
    ("vPointBuffer", vPointBuffer)
    ("vNormalBuffer", vNormalBuffer);
  Renderer_SetShader(*shader);
  Renderer_DrawFSQ();
  occlusionBuffer->Unbind();

  /* Write result to mesh. */
  Array<float> result(vDim * vDim);
  occlusionBuffer->GetData(result.data());
  for (size_t i = 0; i < m->vertices.size(); ++i)
    m->vertices[i].u = result[i];
  m->version++;
}
