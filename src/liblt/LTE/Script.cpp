#include "Script.h"

#include "Environment.h"
#include "Expressions.h"
#include "Hash.h"
#include "Location.h"
#include "LTSL.h"
#include "Map.h"
#include "ProgramLog.h"
#include "Serializer.h"
#include "StackFrame.h"
#include "StringList.h"
#include "Types.h"

#include "Debug.h"

#define AUTORELOAD 1

char const* const kScriptExtension = ".lts";

namespace {
  typedef Map<String, Script> ScriptCacheT;

  ScriptCacheT& GetScriptCache() {
    static ScriptCacheT cache;
    return cache;
  }

  Script& GetCache(String const& name) {
    return GetScriptCache()[name];
  }
}

namespace LTE {
  void ScriptT::Reload() {
    String scriptPath = name + kScriptExtension;
    Location location = Location_Script(scriptPath);
    if (!location->Exists())
      return;

    HashT hash = Max((HashT)1, location->GetHash());
    if (hash == this->hash)
      return;
    this->hash = hash;
    
    functions.clear();
    types.clear();
    dependencies.clear();

    StringList list = StringList_Load(location);
    list = LTSL_ApplyRewrites(list);

    FRAME(&name.front()) {
      CompileEnvironment env;
      env.script = this;
      for (size_t i = 0; i < list->GetSize(); ++i)
        Expression_Compile(list->Get(i), env);
    }
  }

  Script ScriptT::ResolveRelativePath(String const& path) const {
    Vector<String> pathComponents;
    String_Split(pathComponents, name, '/');

    Script script;
    for (size_t i = 0; i < pathComponents.size() && !script; ++i) {
      String potentialName;
      for (size_t j = 0; j + 1 < (pathComponents.size() - i); ++j)
        potentialName += pathComponents[j] + "/";
      potentialName += path;
      script = Script_Load(potentialName);
    }

    return script;
  }

  Type ScriptT::ResolveType(StringList const& list) const {
    if (list->IsAtom()) {
      String const& name = list->GetValue();

      /* Cross-script lookup. */
      if (name.contains(':')) {
        Vector<String> strings;
        String_Split(strings, name, ':');
        String const& scriptName = strings[0];
        String const& typeName = strings[1];

        Script script = ResolveRelativePath(scriptName);
        if (!script)
          return nullptr;

        ScriptType t = script->GetType(typeName);
        if (t) {
          Mutable(dependencies).push(script);
          return t->type;
        }
      }

      /* In-script lookup. */
      ScriptType type = GetType(name);
      if (type)
        return type->type;

      /* Global lookup. */
      return Type_Find(name);
    }

    if (list->GetSize() == 0)
      return nullptr;

    String const& value = list->Get(0)->GetValue();
    if (value == "Array") {
      if (list->GetSize() != 2)
        return nullptr;
      Type elemType = ResolveType(list->Get(1));
      if (!elemType)
        return nullptr;
      return Type_Array(elemType);
    }

    else if (value == "Pointer") {
      if (list->GetSize() != 2)
        return nullptr;
      Type elemType = ResolveType(list->Get(1));
      if (!elemType)
        return nullptr;
      return Type_Pointer(elemType);
    }

    return nullptr;
  }

  DefineFunction(Script_ClearCache) {
    GetScriptCache().clear();
  }

  DefineFunction(Script_Load) {
    Script& script = GetCache(args.name);
    if (script)
      return script;

    String scriptPath = args.name + kScriptExtension;
    Location location = Location_Script(scriptPath);
    if (!location->Exists())
      return nullptr;
    
    script = new ScriptT;
    script->name = args.name;
    script->Reload();
    return script;
  }

  DefineFunction(Script_Reload) {
    bool loaded = GetCache(args.name) != nullptr;
    Script script = Script_Load(args.name);
    if (loaded) {
      Vector<Script> scripts;
      scripts.push(script);
      for (size_t i = 0; i < scripts.size(); ++i) {
        Script script = scripts[i];
        for (size_t j = 0; j < script->dependencies.size(); ++j)
          if (!scripts.contains(script->dependencies[j]))
            scripts.push(script->dependencies[j]);
      }

      for (size_t i = 0; i < scripts.size(); ++i)
        scripts[i]->Reload();
    }
  }

  DefineFunction(ScriptFunction_Load) {
    Vector<String> strings;
    String_Split(strings, args.name, ':');
    if (strings.size() != 2) {
      Log_Error("ScriptFunction_Load received bad path");
      return nullptr;
    }

    String const& scriptName = strings[0];
    String const& functionName = strings[1];

    Script script = Script_Load(scriptName);
    if (!script) {
      Log_Error(Stringize() | "Failed to load script '" | scriptName | "'");
      return nullptr;
    }

#if AUTORELOAD
    Script_Reload(scriptName);
#endif

    ScriptFunction fn = script->GetFunction(functionName);
    if (!fn) {
      Log_Error(Stringize()
        | "Script '" | scriptName
        | "' does not contain function '" | functionName | "'");
      return nullptr;
    }

    return fn;
  }

  DefineFunction(ScriptType_Load) {
    Vector<String> strings;
    String_Split(strings, args.name, ':');
    if (strings.size() != 2) {
      Log_Error("ScriptType_Load received bad path");
      return nullptr;
    }

    String const& scriptName = strings[0];
    String const& typeName = strings[1];

    Script script = Script_Load(scriptName);
    if (!script) {
      Log_Error(Stringize() | "Failed to load script '" | scriptName | "'");
      return nullptr;
    }

    ScriptType type = script->GetType(typeName);
    if (!type) {
      Log_Error(Stringize()
        | "Script '" | scriptName
        | "' does not contain type '" | typeName | "'");
      return nullptr;
    }

    return type;
  }
}
