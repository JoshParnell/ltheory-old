#include "../Expressions.h"

#include "LTE/Environment.h"
#include "LTE/ProgramLog.h"
#include "LTE/Script.h"
#include "LTE/StringList.h"

namespace {
  void ScriptFunction_Call(void**, void*) {
  }
}

namespace LTE {
  Expression Expression_Function(
    StringList const& list,
    CompileEnvironment& env)
  {
    if (list->GetSize() < 4) {
      Log_Error(Stringize()
        | "'function' expects at least 3 arguments, but got "
        | (list->GetSize() - 1));
      return nullptr;
    }

    String const& name = list->Get(2)->GetValue();
    ScriptFunction fn = env.script->functions[name];
    if (fn) {
      Log_Error(Stringize() | "Function '" | name | "' already exists");
      return nullptr;
    }

    CompileEnvironment subEnv;
    subEnv.script = env.script;
    subEnv.context = env.context;

    fn = new ScriptFunctionT;
    fn->name = name;

    /* Push 'this' pointer and member fields for sub environment. */
    for (size_t i = 0; i < subEnv.context.size(); ++i) {
      ScriptType context = subEnv.context[i];
      uint contextRegister = subEnv.Allocate("this", context->type, false, false);

      for (size_t j = 0; j < context->fields.size(); ++j) {
        Field const& field = context->fields[j];
        subEnv.variables[field.name].push(
          Variable(field.name, contextRegister, field.offset, field.type, false, false));
      }

      fn->parameters.push(Parameter("this", context->type));
    }

    /* Compile parameters. */
    StringList const& params = list->Get(3);
    for (size_t i = 0; i < params->GetSize(); i += 2) {
      StringList const& typeName = params->Get(i + 0);
      String varName = params->Get(i + 1)->GetValue();
      Type type = env.script->ResolveType(typeName);
      if (!type) {
        Log_Error(Stringize() | "Unknown type '" | typeName->GetString() | "'");
        return nullptr;
      }

      fn->parameters.push(Parameter(varName, type));
      subEnv.Allocate(varName, type, false, false);
    }

    /* TODO : Support recursion */
    fn->expression = Expression_Block(list, subEnv, 4);
    if (!fn->expression)
      fn->expression = Expression_Noop();
    fn->returnType = fn->expression->GetType();

    if (env.context.size())
      env.context.back()->functions[name] = fn;
    else
      env.script->functions[name] = fn;

    /* Native function. */
#if 0
    Function native = Function_Create("LTSL_Function_" + name);
    native->name = name;
    native->description = "LTSL-Defined Function";
    native->call = ScriptFunction_Call;
    native->paramCount = fn->parameters.size();
    native->params = new Parameter[native->paramCount];
    for (size_t i = 0; i < native->paramCount; i++)
      Mutable(native->params[i]) = fn->parameters[i];
#endif

    return nullptr;
  }
}
