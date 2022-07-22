#define LTE_CONSOLE
#include "LTE/LTE.h"

#include "Game/Objects.h"

#include "Module/PhysicsEngine.h"
#include "Module/SoundEngine.h"

#include "SFML/Graphics.hpp"

#include "LTE/Color.h"
#include "LTE/VectorNP.h"

void EmitFunctionSignature(ScriptFunction const& fn) {
  Type const& returnType = fn->expression->GetType();
  dbg | returnType->name | " " | fn->name | "(";
  for (size_t i = 0; i < fn->parameters.size(); ++i) {
    Parameter const& param = fn->parameters[i];
    if (i)
      dbg | ", ";
    dbg | param.type->name | " const& " | param.name;
  }
  dbg | ")";
}

void EmitFunctionDeclaration(ScriptFunction const& fn) {
  dbg | "EXPORT ";
  EmitFunctionSignature(fn);
  dbg | ";" | endl;
}

void EmitFunctionDefinition(ScriptFunction const& fn) {
  EmitFunctionSignature(fn);
  dbg | " {" | endl;

  Vector<String> local;
  String returnValue = fn->expression->Emit(local);
  for (size_t i = 0; i < local.size(); ++i)
    dbg | "  " | local[i] | endl;

  if (returnValue.size())
    dbg | "  return " | returnValue | ";" | endl;
  dbg | "}" | endl | endl;
}

void EmitTypeDefinition(ScriptType const& type) {
  dbg | "struct " | type->name | " {" | endl;
  for (size_t i = 0; i < type->fields.size(); ++i) {
    Field const& field = type->fields[i];
    dbg | "  " | field.type->name | " " | field.name | ";" | endl;
  }
  dbg | "};" | endl | endl;
}

void EmitC(Script const& script) {
  /* Header */ {
    dbg | "/* Module type declarations. */" | endl;
    for (ScriptTypeMapT::iterator it = script->types.begin();
         it != script->types.end(); ++it)
      EmitTypeDefinition(it->second);

    dbg | "/* Module function declarations. */" | endl;
    for (ScriptFunctionMapT::iterator it = script->functions.begin();
         it != script->functions.end(); ++it)
      EmitFunctionDeclaration(it->second);
  }

  /* Source */ {
    dbg | endl | "/* Module function definitions. */" | endl;
    for (ScriptFunctionMapT::iterator it = script->functions.begin();
         it != script->functions.end(); ++it)
      EmitFunctionDefinition(it->second);
  }
}

const float kSimulationDT = 1.0f / 60.f;

int main(int argc, char const* argv[]) {

#if 0
  Mesh m = Mesh_Sphere(10, 10);
  dbg | m->indices.size() / 3 | endl;
  SphereTree tree = SphereTree_Mesh(m);
  dbg | tree->GetCost() | endl;
  return 0;
#endif

#if 1
  ScriptFunction_Load("App/Benchmark:Main")->VoidCall(0);
  return 0;
#endif

#if 0
  Vector<Function> const& functions = Function_GetList();
  for (size_t i = 0; i < functions.size(); ++i)
    dbg | functions[i] | endl | endl;
  return 0;
#endif

#if 0
  Script script = Script_Load("test2");
  EmitC(script);
  return 0;
#endif

  sf::Context c;
  Renderer_Initialize();
  glewInit();

  Module_RegisterGlobal(CreatePhysicsEngine());
  Module_RegisterGlobal(SoundEngine_Null());

  Object universe = Object_Universe(130, 0);
  ObjectT* system = universe;
  while (system->GetType() != ObjectType_System)
    system = system->GetInteriorObjects();

  Timer timer;
  uint frameCount = 0;
  while (true) {
    UpdateState state(kSimulationDT, false);
    if (timer.GetElapsed() > 1.0f) {
      float frequency = (float)frameCount * kSimulationDT;
      dbg | "[ @ " | frequency | "x]" | endl;
      timer.Reset();
      frameCount = 0;
    }

    Module_UpdateGlobal(kSimulationDT);
    system->Update(state);
    frameCount++;
  }
  return 0;
}
