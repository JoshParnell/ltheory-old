#include "Shader.h"

#include "AutoPtr.h"
#include "BuildMode.h"
#include "CubeMap.h"
#include "GL.h"
#include "Location.h"
#include "Map.h"
#include "Matrix.h"
#include "Pointer.h"
#include "Program.h"
#include "ProgramLog.h"
#include "Renderer.h"
#include "StackFrame.h"
#include "Texture2D.h"
#include "Texture3D.h"
#include "V4.h"
#include "VectorMap.h"

#include <sstream>
#include <iostream>

const String kVersionDirective = "#version 120\n";
const uint kTextureUnits = 16;

namespace {
  typedef Reference<struct ShaderObjectT> ShaderObject;
  typedef Reference<struct ProgramObjectT> ProgramObject;
  typedef Map<String, ShaderObject> ShaderMap;
  typedef Map<String, ProgramObject> ProgramMap;
  typedef Map<char const*, int> LocationCache;

  GL_Program gActiveProgram = GL_NullProgram;
  Pointer<ShaderT> gActiveShader;

  ShaderMap& GetShaderCache() {
    static ShaderMap map;
    return map;
  }

  ProgramMap& GetProgramCache() {
    static ProgramMap map;
    return map;
  }

  /* Run a manual preprocessor on the shader code to support #include. */
  String JSLPreprocess(String const& code) {
    std::stringstream parsed;
    std::stringstream codestream(code);
    String buf;

    while (getline(codestream, buf)) {
      if (buf.size() && buf.front() == '#') {
        Vector<String> tokens;
        String_Split(tokens, buf.substr(1), ' ');

        if (!tokens.size()) {
          parsed << buf << '\n';
          continue;
        }

        if (tokens[0] == "include") {
          LTE_ASSERT(tokens.size() == 2);
          parsed << JSLPreprocess(
            Location_Shader("common/" + tokens[1])->ReadAscii());
        }

        else if (tokens[0] == "output") {
          LTE_ASSERT(tokens.size() == 4);
          int index = FromString<int>(tokens[1]);
          String const& varType = tokens[2];
          int components = 1;
          if (varType.contains('2'))
            components = 2;
          else if (varType.contains('3'))
            components = 3;
          else if (varType.contains('4'))
            components = 4;
          
          parsed << "#define " << tokens[3] << " gl_FragData[" << index << "]."; 
          parsed << String("xyzw").substr(0, components);  
          parsed << '\n';
        }

        else
          parsed << buf << '\n';
      } else
        parsed << buf << '\n';
    }
    return parsed.str();
  }

  struct ShaderObjectT : public RefCounted {
    GL_Shader id;
    String path;
    String source;
    int version;

    ShaderObjectT() : version(0) {}

    ~ShaderObjectT() {
      if (!Program_InStaticSection()) {
        GL_DeleteShader(id);
        if (path.size() && GetShaderCache().get(path))
          GetShaderCache().erase(path);
      }
    }

    bool Compile() {
      SFRAME("Compile GPU Shader");
      version++;
      GL_ShaderSource(id, source);
      GL_CompileShader(id);

      /* Check the compilation status to see if there was an error. */
      int compileStatus = GL_GetShaderI(id, GL_ShaderProperty::CompileStatus);

      if (compileStatus == 0) {
        Log_Error("Failed to compile shader.");
        PrintSource();
        Log_Message("Compiler Log:");
        Log_Message(GetLog());
        getchar();
        return false;
      }
      return true;
    }

    String GetLog() const {
      String log;
      GL_GetShaderInfoLog(id, log);
      return log;
    }

    void PrintSource() {
      Vector<String> splitSource;
      String_Split(splitSource, source, '\n');
      for (size_t i = 0; i < splitSource.size(); ++i)
        Log_Message(ToString(i) + '\t' + splitSource[i]);
    }
  };

  ShaderObject ShaderObject_Create(
    String const& code,
    GL_ShaderType::Enum type)
  {
    ShaderObject self = new ShaderObjectT;
    self->id = GL_CreateShader(type);
    self->source = kVersionDirective + JSLPreprocess(code);
    if (!self->Compile())
      return nullptr;
    return self;
  }

  ShaderObject ShaderObject_Load(
    String const& path,
    GL_ShaderType::Enum type)
  {
    if (GetShaderCache().get(path))
      return GetShaderCache()[path];

    String code = Location_Shader(path)->ReadAscii();
    if (code.empty()) {
      Log_Error("Failed to load shader <" + path + ">");
      return nullptr;
    }

    ShaderObject self = ShaderObject_Create(code, type);
    if (self) {
      GetShaderCache()[path] = self;
      self->path = path;
    } else {
      Log_Error("Failed to load shader <" + path + ">");
    }

    return self;
  }

  struct ProgramObjectT : public RefCounted {
    ShaderObject vertShader;
    ShaderObject fragShader;
    GL_Program id;
    String path;
    int textureUnitIndex;
    int version;

    LocationCache uniforms;
    int mWorld;
    int mView;
    int mProj;
    int mWorldIT;
    int mWVP;

    ProgramObjectT() :
      textureUnitIndex(0),
      version(0),
      mWorld(-1),
      mView(-1),
      mProj(-1),
      mWorldIT(-1),
      mWVP(-1)
      {}

    ~ProgramObjectT() {
      if (!Program_InStaticSection()) {
        GL_DeleteProgram(id);
        if (gActiveProgram == id)
          Shader_UseFixedFunction();
        if (path.size() && GetProgramCache().get(path))
          GetProgramCache().erase(path);
      }
    }

    void BindGlobalAttributes() {
      BindInput(0, "vertex_position");
      BindInput(1, "vertex_normal");
      BindInput(2, "vertex_uv");
      BindInput(3, "vertex_color");
      BindOutput(0, "fragment_color0");
      BindOutput(1, "fragment_linearDepth");
    }

    void BindInput(size_t attribIndex, char const* name) {
      LTE_ASSERT(attribIndex < kAttribArrays);
      GL_BindAttribLocation(id, attribIndex, name);
    }

    void BindOutput(size_t bufferIndex, char const* name) {
      LTE_ASSERT(bufferIndex < GL_MAX_DRAW_BUFFERS);
      GL_BindFragDataLocation(id, bufferIndex, name);
    }

    void CacheWVP() {
      /* Cache the locations of WVP matrices. */
      mWorld = GL_GetUniformLocation(id, "WORLD");
      mView = GL_GetUniformLocation(id, "VIEW");
      mProj = GL_GetUniformLocation(id, "PROJ");
      mWorldIT = GL_GetUniformLocation(id, "WORLDIT");
      mWVP = GL_GetUniformLocation(id, "WVP");
    }

    String GetLog() const {
      String log;
      GL_GetProgramInfoLog(id, log);
      return log;
    }

    int GetUniformLocation(char const* name, bool warn) {
      int* it = uniforms.get(name);
      if (it)
        return *it;

      int index = GL_GetUniformLocation(id, name);
      uniforms[name] = index;

      if (warn && index < 0) {
        String warning = Stringize() |
          "Unused variable " | name | " in Shader(" | vertShader->path |
          ", " | fragShader->path | ")";
        Log_Warning(warning);
      }

      return index;
    }

    void Link() {
      SFRAME("Link GPU Program");
      GL_LinkProgram(id);

      if (GL_GetProgramI(id, GL_ProgramProperty::LinkStatus) == 0) {
        Log_Error("Failed to link program.");
        Log_Error(GetLog());
        vertShader->PrintSource();
        fragShader->PrintSource();
      }

      CacheWVP();
      version = vertShader->version ^ fragShader->version;
    }
  };

  ProgramObject ProgramObject_Create(
    ShaderObject const& vertex,
    ShaderObject const& fragment)
  {
    ProgramObject self = new ProgramObjectT;
    self->id = GL_CreateProgram();
    if (self->id == GL_NullProgram)
      return nullptr;

    self->vertShader = vertex;
    self->fragShader = fragment;
    GL_AttachShader(self->id, vertex->id);
    GL_AttachShader(self->id, fragment->id);
    self->BindGlobalAttributes();
    self->Link();
    return self;
  }

  ProgramObject ProgramObject_Load(
    String const& vertPath,
    String const& fragPath)
  {
    String programPath = vertPath + "?" + fragPath;
    if (GetProgramCache().get(programPath))
      return GetProgramCache()[programPath];

    ShaderObject vs = ShaderObject_Load("vertex/" + vertPath, GL_ShaderType::Vertex);
    if (!vs)
      return nullptr;

    ShaderObject fs = ShaderObject_Load("fragment/" + fragPath, GL_ShaderType::Fragment);
    if (!fs)
      return nullptr;

    ProgramObject self = ProgramObject_Create(vs, fs);
    if (self) {
      GetProgramCache()[programPath] = self;
      self->path = programPath;
    } else {
      Log_Error("Failed to load program <" + vertPath + ", " + fragPath + ">");
    }

    return self;
  }

  struct ShaderImpl : public ShaderT {
    ProgramObject program;

    ~ShaderImpl() {
      if (gActiveShader == this)
        gActiveShader = nullptr;
    }

    void BindInput(size_t attribIndex, char const* name) {
      program->BindInput(attribIndex, name);
    }

    void BindOutput(size_t bufferIndex, char const* name) {
      program->BindOutput(bufferIndex, name);
    }

    void BindMatrices(
      Matrix const& world,
      Matrix const& view,
      Matrix const& proj,
      Matrix const& worldIT,
      Matrix const& WVP)
    {
      if (program->mWorld >= 0)
        SetMatrix(program->mWorld, &world);
      if (program->mView >= 0)
        SetMatrix(program->mView, &view);
      if (program->mProj >= 0)
        SetMatrix(program->mProj, &proj);
      if (program->mWorldIT >= 0)
        SetMatrix(program->mWorldIT, &worldIT);
      if (program->mWVP >= 0)
        SetMatrix(program->mWVP, &WVP);
    }

    bool Create(String const& vertCode, String const& fragCode) {
      ShaderObject vertex =
        ShaderObject_Create(vertCode, GL_ShaderType::Vertex);
      if (!vertex)
        return false;

      ShaderObject fragment =
        ShaderObject_Create(fragCode, GL_ShaderType::Fragment);
      if (!fragment)
        return false;

      program = ProgramObject_Create(vertex, fragment);
      return program != nullptr;
    }

    int GetTextureUnit() {
      int index = program->textureUnitIndex++;
      program->textureUnitIndex = program->textureUnitIndex % kTextureUnits;
      return index;
    }

    int GetUniformLocation(char const* name) {
      return program->GetUniformLocation(name, true);
    }

    int QueryUniformLocation(char const* name) {
      return program->GetUniformLocation(name, false);
    }

    void PrintLogs() const {
      std::cout
        << ">>> Vertex Shader:\n"
        << program->vertShader->GetLog() << "\n\n"
        << ">>> Fragment Shader:\n"
        << program->fragShader->GetLog() << "\n\n"
        << ">>> Program:\n"
        << program->GetLog() << "\n\n";
    }

    void Relink() {
      program->Link();
    }

    ShaderT& SetCubeMap(char const* name, CubeMap const& cubeMap) {
      int varIndex = GetUniformLocation(name);
      if (varIndex < 0)
        return *this;
      return SetCubeMap(varIndex, cubeMap);
    }

    ShaderT& SetCubeMap(int varIndex, CubeMap const& cubeMap) {
      Use();
      int unit = GetTextureUnit();
      GL_Uniform(varIndex, unit);
      GL_ActiveTexture(unit);
      cubeMap->Bind();
      GL_ActiveTexture(0);
      return *this;
    }

    ShaderT& SetFloat(char const* name, float f) {
      int varIndex = GetUniformLocation(name);
      if (varIndex < 0)
        return *this;
      return SetFloat(varIndex, f);
    }

    ShaderT& SetFloat(int varIndex, float f) {
      Use();
      GL_Uniform(varIndex, f);
      return *this;
    }

    ShaderT& SetFloatArray(char const* name, float const* data, size_t size) {
      int varIndex = GetUniformLocation(name);
      if (varIndex < 0)
        return *this;
      return SetFloatArray(varIndex, data, size);
    }

    ShaderT& SetFloatArray(int varIndex, float const* data, size_t size) {
      Use();
      GL_UniformArray1(varIndex, size, data);
      return *this;
    }

    ShaderT& SetFloat2(char const* name, V2 const& v) {
      int varIndex = GetUniformLocation(name);
      if (varIndex < 0)
        return *this;
      return SetFloat2(varIndex, v);
    }

    ShaderT& SetFloat2(int varIndex, V2 const& v) {
      Use();
      GL_Uniform(varIndex, v.x, v.y);
      return *this;
    }

    ShaderT& SetFloat3(char const* name, V3 const& v) {
      int varIndex = GetUniformLocation(name);
      if (varIndex < 0)
        return *this;
      return SetFloat3(varIndex, v);
    }

    ShaderT& SetFloat3(int varIndex, V3 const& v) {
      Use();
      GL_Uniform(varIndex, v.x, v.y, v.z);
      return *this;
    }

    ShaderT& SetFloat4(char const* name, V4 const& v) {
      int varIndex = GetUniformLocation(name);
      if (varIndex < 0)
        return *this;
      return SetFloat4(varIndex, v);
    }

    ShaderT& SetFloat4(int varIndex, V4 const& v) {
      Use();
      GL_Uniform(varIndex, v.x, v.y, v.z, v.w);
      return *this;
    }

    ShaderT& SetFloat3Array(char const* name, V3 const* data, size_t size) {
      int varIndex = GetUniformLocation(name);
      if (varIndex < 0)
        return *this;
      return SetFloat3Array(varIndex, data, size);
    }

    ShaderT& SetFloat3Array(int varIndex, V3 const* data, size_t size) {
      Use();
      GL_UniformArray3(varIndex, size, (float const*)data);
      return *this;
    }

    ShaderT& SetFloat4Array(char const* name, V4 const* data, size_t size) {
      int varIndex = GetUniformLocation(name);
      if (varIndex < 0)
        return *this;
      return SetFloat4Array(varIndex, data, size);
    }

    ShaderT& SetFloat4Array(int varIndex, V4 const* data, size_t size) {
      Use();
      GL_UniformArray4(varIndex, size, (float const*)data);
      return *this;
    }

    ShaderT& SetMatrix(char const* name, Matrix const* m) {
      int varIndex = GetUniformLocation(name);
      if (varIndex < 0)
        return *this;
      return SetMatrix(varIndex, m);
    }

    ShaderT& SetMatrix(int varIndex, Matrix const* m) {
      Use();
      GL_UniformMatrix4(varIndex, &(m->e[0]));
      return *this;
    }

    ShaderT& SetInt(char const* name, int i) {
      int varIndex = GetUniformLocation(name);
      if (varIndex < 0)
        return *this;
      return SetInt(varIndex, i);
    }

    ShaderT& SetInt(int varIndex, int i) {
      Use();
      GL_Uniform(varIndex, i);
      return *this;
    }

    ShaderT& SetTexture2D(char const* name, Texture2D const& t) {
      int varIndex = GetUniformLocation(name);
      if (varIndex < 0)
        return *this;
      return SetTexture2D(varIndex, t);
    }

    ShaderT& SetTexture2D(int varIndex, Texture2D const& t) {
      Use();
      int unit = GetTextureUnit();
      GL_Uniform(varIndex, unit);
      if (t) {
        t->BindInput(unit);
      } else {
        GL_ActiveTexture(unit);
        GL_BindTexture(GL_TextureTargetBindable::T2D, GL_NullTexture);
      }
      GL_ActiveTexture(0);
      return *this;
    }

    ShaderT& SetTexture3D(char const* name, Texture3D const& t) {
      int varIndex = GetUniformLocation(name);
      if (varIndex < 0)
        return *this;
      return SetTexture3D(varIndex, t);
    }

    ShaderT& SetTexture3D(int varIndex, Texture3D const& t) {
      Use();
      int unit = GetTextureUnit();
      GL_Uniform(varIndex, unit);
      if (t) {
        t->Bind(unit);
      } else {
        GL_ActiveTexture(unit);
        GL_BindTexture(GL_TextureTargetBindable::T3D, GL_NullTexture);
      }
      GL_ActiveTexture(0);
      return *this;
    }

    void Use() {
      if (gActiveProgram != program->id) {
        gActiveProgram = program->id;
        gActiveShader = this;
        GL_UseProgram(program->id);
      }
    }
  };
}

DefineFunction(Shader_Create) {
  Reference<ShaderImpl> self = new ShaderImpl;
  self->program = ProgramObject_Load(args.vsPath, args.fsPath);
  LTE_ASSERT(self->program);
  return self;
}

Shader Shader_Create(
  String const& vs,
  String const& fs,
  String const& vsHeader,
  String const& fsHeader)
{
  Reference<ShaderImpl> self = new ShaderImpl;
  String vertCode =
    Location_Shader("vertex/" + vs)->ReadAscii();

  if (vertCode.empty()) {
    Log_Error("Failed to load shader <" + vs + ">");
    return nullptr;
  }
  vertCode = vsHeader + '\n' + vertCode;

  String fragCode =
    Location_Shader("fragment/" + fs)->ReadAscii();

  if (fragCode.empty()) {
    Log_Error("Could to load shader <" + fs + ">");
    return nullptr;
  }
  fragCode = fsHeader + '\n' + fragCode;

  if (!self->Create(vertCode, fragCode))
    Log_Error("Failed to create shader <" + vs + ", " + fs + ">");
  return self;
}

ShaderT* Shader_GetActive() {
  return gActiveShader;
}

GL_Program Shader_GetCurrentProgram() {
  return gActiveProgram;
}

DefineFunction(Shader_RecompileAll) {
  for (ShaderMap::iterator it = GetShaderCache().begin();
       it != GetShaderCache().end(); ++it)
  {
    ShaderObject const& shader = it->second;
    if (!shader->path.size())
      continue;

    String code = Location_Shader(shader->path)->ReadAscii();
    if (!code.size())
      continue;

    code = kVersionDirective + JSLPreprocess(code);
    if (code != shader->source) {
      shader->source = code;
      shader->Compile();
    }
  }

  for (ProgramMap::iterator it = GetProgramCache().begin();
       it != GetProgramCache().end(); ++it)
  {
    ProgramObject const& program = it->second;
    int version = program->vertShader->version ^ program->fragShader->version;
    if (program->version != version)
      it->second->Link();
  }
}

void Shader_UseFixedFunction() {
  if (gActiveProgram != GL_NullProgram) {
    gActiveProgram = GL_NullProgram;
    gActiveShader = nullptr;
    GL_UseProgram(GL_NullProgram);
  }
}
