#ifndef LTE_Script_h__
#define LTE_Script_h__

#include "Array.h"
#include "DeclareFunction.h"
#include "ScriptType.h"

namespace LTE {
  typedef Map<String, ScriptType> ScriptTypeMapT;
  
  AutoClassDerived(ScriptT, RefCounted,
    String, name,
    HashT, hash,
    ScriptFunctionMapT, functions,
    ScriptTypeMapT, types,
    Vector<Script>, dependencies)

    ScriptT() : hash(0) {}

    bool DependsOn(Script const& script) const {
      if (script.t == this)
        return true;
      for (size_t i = 0; i < dependencies.size(); ++i)
        if (dependencies[i]->DependsOn(script))
          return true;
      return false;
    }

    ScriptFunction GetFunction(String const& name) const {
      ScriptFunction const* fn = functions.get(name);
      return fn ? *fn : nullptr;
    }

    ScriptType GetType(String const& name) const {
      ScriptType const* type = types.get(name);
      return type ? *type : nullptr;
    }

    Script ResolveRelativePath(String const& path) const;

    Type ResolveType(StringList const& name) const;

    LT_API void Reload();
  };

  DeclareFunctionNoParams(Script_ClearCache, void)

  DeclareFunction(Script_Load, Script,
    String, name)

  DeclareFunction(Script_Reload, void,
    String, name)

  DeclareFunction(ScriptFunction_Load, ScriptFunction,
    String, name)

  DeclareFunction(ScriptType_Load, ScriptType,
    String, name)
}

#endif
