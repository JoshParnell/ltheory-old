#ifndef LTE_ScriptType_h__
#define LTE_ScriptType_h__

#include "Field.h"
#include "Map.h"
#include "ScriptFunction.h"
#include "Vector.h"

namespace LTE {
  typedef Map<String, ScriptFunction> ScriptFunctionMapT;

  AutoClassDerived(ScriptTypeT, RefCounted,
    String, name,
    size_t, size,
    Vector<Field>, fields,
    Vector<Expression>, initializers,
    Type, type,
    ScriptFunctionMapT, functions)

    ScriptTypeT() : size(0) {}

    ScriptFunction GetFunction(String const& name) const {
      ScriptFunction const* fn = functions.get(name);
      return fn ? *fn : nullptr;
    }
  };
}

#endif
