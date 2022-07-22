#include "SDFMesh.h"
#include "Array.h"
#include "Bound.h"
#include "Geometry.h"
#include "Job.h"
#include "Math.h"
#include "Matrix.h"
#include "Mesh.h"
#include "Ray.h"
#include "Renderer.h"
#include "SDF.h"
#include "Shader.h"
#include "Texture2D.h"
#include "Texture3D.h"
#include "Thread.h"
#include "Vector.h"

#include "Module/Scheduler.h"

#include "Volume/Array3D.h"

#include "LTE/Debug.h"

/* Field Texture -> (LOD Level -> MC) -> Ambient Occlusion */

const uint kMaxQuality = 128;
const uint kMaxResolution = 256;
const uint kMinResolution = 4;

/* TODO - Remove this number : just like mips, should create every level until
 *        resolution drops below some point. */
const uint kLodLevels = 8;
const float kLodDownsampleFactor = 0.666f;
const float kLodFactor = 1.0f;

const uint kCollisionMeshResolution = 32;
const uint kOcclusionSamples = 2048;
const float kBaseOcclusionRadius = 0.3f;
const bool kPreload = false;

typedef V3T<uint> SV3;

namespace {
  Shader gCopyShader;
  Shader gGradientShader;
  Shader gOcclusionShader;

  void StaticInitialize() {
    if (!gCopyShader) {
      gCopyShader = Shader_Create("identity.jsl", "gen/fieldcopy.jsl");
      gGradientShader = Shader_Create("identity.jsl", "gen/fieldgradient.jsl");
      gOcclusionShader = Shader_Create("identity.jsl", "gen/fieldocclusion.jsl");
    }
  }

  struct SDFMesh;
  struct SDFMeshLevel {
    Mesh mesh;
    SV3 res;
    SDFMeshLevel() {}
    SDFMeshLevel(SV3 const& res) :
      res(res)
      {}
  };
    
  Job Job_GenerateFieldTexture(SDFMesh* parent, SV3 const& res);
  Job Job_GenerateLodLevel(SDFMesh* p, SDFMeshLevel& level);
  Job Job_GenerateOcclusion(SDFMesh* p, SDFMeshLevel& level);

  AutoClassDerived(SDFMesh, GeometryT,
    SDF, function,
    V3, resolutionMult,
    float, occlusionRadiusMult,
    float, tileMult,
    bool, eliminateFloaters)
    Vector<SDFMeshLevel> levels;
    SDFMeshLevel collisionLevel;
    Shader fieldShader;
    Texture3D fieldTexture;
    Bound3 bound;
    bool isLoaded;

    DERIVED_TYPE_EX(SDFMesh)

    SDFMesh() {}
 
    DefineInitializer {
      StaticInitialize();
      this->collisionLevel = SDFMeshLevel(kCollisionMeshResolution);
      this->bound = function->GetBound().GetExpanded(1.05f);
      this->isLoaded = false;

      if (kPreload)
        Load();
    }

    Mesh GetMesh(size_t lodLevel) const {
      if (levels.empty())
        return nullptr;
      size_t index = Min(levels.size() - 1, lodLevel);
      while (!levels[index].mesh && index + 1 < levels.size())
        index++;
      return levels[index].mesh;
    }

    void Draw() const {
      Mutable(this)->Load();

      Matrix const& world = Renderer_GetWorldMatrix();
      Bound3 worldBound = bound.GetTransformed(world);
      float radius = worldBound.GetRadius();
      float distance = Length(worldBound.GetCenter());

      float lod = kLodFactor * Max(0.0f, distance / radius - 1.0f);
      int lodLevel = (int)(Log(1.0 + lod) / Log(1.0f / kLodDownsampleFactor));

      Mesh currentMesh;
      while (!currentMesh) {
        currentMesh = GetMesh(lodLevel);
        if (!currentMesh)
          Scheduler_Get()->Flush();
      }

      currentMesh->Draw();
    }

    Bound3 GetBound() const {
      return bound;
    }

    Mesh GetCollisionMesh() const {
      Mesh const& m = collisionLevel.mesh;
      return m ? m->Clone() : nullptr;
    }

    short GetVersion() const {
      short versionSum = 0;
      for (size_t i = 0; i < levels.size(); ++i)
        versionSum = (short)(versionSum + (levels[i].mesh ? levels[i].mesh->GetVersion() : 0));
      return versionSum;
    }

    bool Intersects(
      Ray const& ray,
      float* tOut,
      V3* normalOut,
      V2* uvOut) const
    {
      float near, far;
      if (!bound.Intersects(ray.origin, V3(1) / ray.direction, near, far))
        return false;

      const size_t kSubdivisions = 64;
      const size_t kRefinements = 8;
      float rayStep = (far - near) / kSubdivisions;
      float t = near;
      for (size_t i = 0; i < kSubdivisions; ++i) {
        t += rayStep;
        float thisField = function->Evaluate(ray(t));
        if (thisField <= 0) {
          for (size_t j = 0; j < kRefinements; ++j) {
            rayStep /= 2.0f;
            t += thisField <= 0 ? -rayStep : rayStep;
          }

          if (tOut)
            *tOut = t;
          if (normalOut)
            *normalOut = Normalize(function->Gradient(ray(t)));
          return true;
        }
      }
      return false;
    }

    bool IsFullyLoaded() const {
      if (!collisionLevel.mesh)
        return false;
      for (size_t i = 0; i < levels.size(); ++i)
        if (!levels[i].mesh)
          return false;
      return true;
    }

    void Load() {
      if (!isLoaded) {
        V3 extent = bound.GetSideLengths();
        V3 vertexDensity = resolutionMult * extent / extent.GetGeometricAverage();
        SV3 fullRes = (float)kMaxQuality * vertexDensity;
        if (((V3)fullRes).GetGeometricAverage() > kMaxResolution)
          fullRes = (SV3)((V3)fullRes * (kMaxResolution / ((V3)fullRes).GetGeometricAverage()));

        SV3 levelResolution = fullRes;
        for (uint i = 0; i < kLodLevels; ++i) {
          levels.push(SDFMeshLevel(Max(levelResolution, SV3(kMinResolution))));
          levelResolution = V3(levelResolution) * kLodDownsampleFactor;
        }

        fieldShader = Shader_Create("identity.jsl", "gen/field.jsl",
                                    "", "#define FIELDFN " + function->GetCode());
        fieldShader->BindOutput(0, "field_value");
        
        Scheduler_Add(Job_GenerateFieldTexture(this, fullRes), false);
        Scheduler_Add(Job_GenerateLodLevel(this, collisionLevel), false);

        for (size_t i = 0; i < levels.size(); ++i) {
          Scheduler_Add(Job_GenerateLodLevel(this, levels[i]), false);
          Scheduler_Add(Job_GenerateOcclusion(this, levels[i]), false);
        }

        isLoaded = true;
      }
    }

    void Unload() {
      fieldShader = nullptr;
      fieldTexture = nullptr;
    }

    V3 Sample() const {
      Mesh m = GetMesh(0);
      return m ? m->Sample() : 0;
    }
  };

  struct GenerateFieldTexture : public JobT {
    SDFMesh* parent;
    Texture3D output;
    SV3 res;
    uint z;
    Texture2D fieldSlice;
    Array<float> buffer;

    GenerateFieldTexture(SDFMesh* parent, SV3 const& res) :
      parent(parent),
      res(res),
      z(0),
      buffer(res.x * res.y)
      {}

    char const* GetName() const {
      return "SDFMesh::Field Texture";
    }

    uint GetMemoryUsage() const {
      return 16 * res.GetProduct();
    }

    float GetPriority() const {
      return 1;
    }

    bool IsFinished() const {
      return z >= res.z;
    }

    void OnBegin() {
      output = Texture3D_Create(res.x, res.y, res.z, GL_TextureFormat::R32F);
      output->AddressBorder(1, 0, 0, 0);
      fieldSlice = Texture_Create(res.x, res.y, GL_TextureFormat::R32F);
    }

    void OnEnd() {
      parent->fieldTexture = output;
    }

    void OnRun(uint jobSize) {
      RendererBlendMode blendMode(BlendMode::Disabled);
      RendererZBuffer zBuffer(false);
      V3 range = parent->bound.GetSideLengths();
      (*parent->fieldShader)
        ("du", range.GetX())("dv", range.GetY())
        ("halfTexel", V3(0.5f / (float)res.x, 0.5f / (float)res.y, 0))
        ("texelScale", V3((float)res.x / (float)(res.x - 1),
                          (float)res.y / (float)(res.y - 1), 1));

      fieldSlice->Bind(0);
      for (uint i = 0; i < jobSize && z < res.z; ++i) {
        (*parent->fieldShader)
          ("origin", parent->bound.lower + (float)z / (float)(res.z - 1) * range.GetZ());
        Renderer_DrawQuad();
        fieldSlice->GetData(buffer.data());
        output->SetData(
          0, 0, z, res.x, res.y, 1,
          GL_PixelFormat::Red, GL_DataFormat::Float, buffer.data());
        z++;
      }
      fieldSlice->Unbind();
    }
  };

  struct GenerateLodLevel : public JobT {
    SDFMesh* p;
    SDFMeshLevel& level;
    Mesh output;
    Texture2D slice;
    Array3DFloat grid;
    Array<float> buffer;
    uint z;

    struct Polygonize : public JobT {
      Reference<GenerateLodLevel> parent;
      Mesh output;

      Polygonize(Reference<GenerateLodLevel> const& parent) :
        parent(parent)
        {}

      char const* GetName() const {
        return "SDFMesh::Polygonize";
      }

      uint GetMemoryUsage() const {
        return 2 * parent->level.res.GetProduct() * sizeof(Vertex);
      }

      bool IsFinished() const {
        return output;
      }

      void OnEnd() {
        parent->output = output;
      }

      void OnRun(uint size) {
        output = Mesh_Volume(parent->grid, parent->p->bound);

        if (parent->p->eliminateFloaters) {
          Mesh m = output;
          output = m->ComputePrincipleComponent();
        }

        if (!output)
          output = Mesh_Create();
      }
    };

    GenerateLodLevel(SDFMesh* p, SDFMeshLevel& level) :
      p(p),
      level(level),
      grid(level.res.x, level.res.y, level.res.z),
      buffer(level.res.x * level.res.y),
      z(0)
      {}

    bool CanRun() const {
      return p->fieldTexture != nullptr && z < level.res.z;
    }

    char const* GetName() const {
      return "SDFMesh::LOD Level";
    }

    uint GetMemoryUsage() const {
      return 16 * level.res.GetProduct();
    }

    float GetPriority() const {
      return Exp(-Pow((float)level.res.GetProduct(), 1.0f / 3.0f));
    }

    bool IsFinished() const {
      return output;
    }

    void OnBegin() {
      slice = Texture_Create(level.res.x, level.res.y, GL_TextureFormat::R32F);
    }

    void OnEnd() {
      level.mesh = output;
    }

    void OnRun(uint jobSize) {
      if (z >= level.res.z)
        return;

      RendererBlendMode blendMode(BlendMode::Disabled);
      RendererZBuffer zBuffer(false);

      V3 range = p->bound.GetSideLengths();
      SV3 res = level.res;
      (*gCopyShader)
        ("du", range.GetX())
        ("dv", range.GetY())
        ("fieldSampler", p->fieldTexture)
        ("fieldOrigin", p->bound.lower)
        ("fieldExtent", range)
        ("halfTexel", V3(0.5f / (float)res.x, 0.5f / (float)res.y, 0))
        ("texelScale", V3((float)res.x / (float)(res.x - 1),
                          (float)res.y / (float)(res.y - 1),
                          1));

      slice->Bind(0);
      for (uint i = 0; i < jobSize && z < res.z; ++i) {
        (*gCopyShader)
          ("origin", p->bound.lower + (float)z / (float)(res.z - 1) * range.GetZ());
        Renderer_DrawQuad();
        slice->GetData(buffer.data());
        for (uint y = 0; y < res.y; ++y)
        for (uint x = 0; x < res.x; ++x)
          grid.SetData(buffer[x + y * res.x], x, y, z);
        z++;
      }
      slice->Unbind();

      /* If we've finished generating the grid, launch the contouring job. */
      if (z >= res.z)
        Scheduler_Add(new Polygonize(this), true);
    }
  };

  struct GenerateOcclusion : public JobT {
    SDFMesh* p;
    SDFMeshLevel& level;
    uint x;
    uint dim;
    uint samples;
    Texture2D positionTexture;
    Texture2D normalTexture;
    Texture2D noiseTexture;
    Texture2D occlusionTexture;

    GenerateOcclusion(SDFMesh* p, SDFMeshLevel& level) :
      p(p),
      level(level),
      x(0)
      {}
    
    bool CanRun() const {
      return level.mesh != nullptr;
    }

    char const* GetName() const {
      return "SDFMesh::Occlusion";
    }

    uint GetMemoryUsage() const {
      return 16 * level.res.GetProduct();
    }

    float GetPriority() const {
      return 0;
    }

    bool IsFinished() const {
      return x >= dim;
    }

    void OnBegin() {
      Mesh const& input = level.mesh;
      float radius = p->occlusionRadiusMult * kBaseOcclusionRadius;
      samples = (uint)Ceil(Sqrt((float)kOcclusionSamples));
      uint rootSamples = samples;
      samples *= samples;
      dim = (uint)Ceil(Sqrt((float)input->vertices.size()));

      /* Create the vertex position & normal buffers. */
      Array<V4> positionBuffer(dim * dim);
      Array<V4> normalBuffer(dim * dim);
      for (size_t i = 0; i < input->vertices.size(); ++i) {
        positionBuffer[i] = V4(input->vertices[i].p, 0);
        normalBuffer[i] = V4(input->vertices[i].n, 0);
      }

      positionTexture = Texture_Create(dim, dim, GL_TextureFormat::RGBA32F, positionBuffer.data());
      normalTexture = Texture_Create(dim, dim, GL_TextureFormat::RGBA32F, normalBuffer.data());
      positionBuffer.clear();
      normalBuffer.clear();

      /* Create the sampling pattern buffer. */
      Array<V4> noiseBuffer(samples);
      uint bufferIndex = 0;
      for (uint i = 0; i < rootSamples; ++i) {
        float thisRadius = Sqrt((float)i / (float)(rootSamples - 1));
        for (uint j = 0; j < rootSamples; ++j) {
          float angle = kTau * (float)j / (float)(rootSamples - 1);
          float y = thisRadius * Cos(angle);
          float z = thisRadius * Sin(angle);
          float x = Sqrt(1.0f - thisRadius * thisRadius);
          x += 0.04f;
          float factor = -Log(1.0f - Rand());
          noiseBuffer[bufferIndex++] = V4((radius * factor) * V3(x, y, z), 0);
        }
      }
      
      noiseTexture = Texture_Create(samples, 1, GL_TextureFormat::RGBA32F, noiseBuffer.data());
      occlusionTexture = Texture_Create(dim, dim, GL_TextureFormat::R32F);
    }

    void OnEnd() {
      Mesh const& input = level.mesh;
      Array<float> occlusionBuffer(dim * dim);
      occlusionTexture->GetData(occlusionBuffer.data());
      for (size_t i = 0; i < input->vertices.size(); ++i)
        input->vertices[i].u = occlusionBuffer[i];

      input->version++;
    }

    void OnRun(uint jobSize) {
      jobSize = Min(jobSize, dim - x);
      V3 extent = p->bound.GetSideLengths();
      (*gOcclusionShader)
        ("samples", (int)samples)
        ("positionSampler", positionTexture)
        ("normalSampler", normalTexture)
        ("noiseSampler", noiseTexture)
        ("fieldSampler", p->fieldTexture)
        ("fieldOrigin", p->bound.lower)
        ("fieldExtent", extent)
        ("fieldStep", extent / V3(level.res));

      RendererBlendMode blendMode(BlendMode::Disabled);
      RendererZBuffer zBuffer(false);
      occlusionTexture->Bind(0);
      Renderer_DrawFSQPart(x, 0, jobSize, dim);
      occlusionTexture->Unbind();
      x += jobSize;
    }
  };

  Job Job_GenerateFieldTexture(SDFMesh* parent, SV3 const& res) {
    return new GenerateFieldTexture(parent, res);
  }

  Job Job_GenerateLodLevel(SDFMesh* p, SDFMeshLevel& level) {
    return new GenerateLodLevel(p, level);
  }

  Job Job_GenerateOcclusion(SDFMesh* p, SDFMeshLevel& level) {
    return new GenerateOcclusion(p, level);
  }
}

namespace LTE {
  Geometry SDFMesh_Create(
    SDF const& field,
    V3 const& resolutionMult,
    float occlusionRadiusMult,
    float tileMult,
    bool eliminateFloaters)
  {
    return new SDFMesh(
      field,
      resolutionMult,
      occlusionRadiusMult,
      tileMult,
      eliminateFloaters);
  }
}
