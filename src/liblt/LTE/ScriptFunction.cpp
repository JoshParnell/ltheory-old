#include "ScriptFunction.h"

#include "Environment.h"
#include "StackFrame.h"

//#define ENABLE_SCRIPT_CACHING

#if 0

#ifdef ENABLE_SCRIPT_CACHING
      String cacheName = script->name;
      Hasher hash;
      for (size_t i = 0; i < args.size(); ++i)
        hash(args[i].data, args[i].type->size);

      cacheName += "_" + ToString((HashT)hash) + ".bin";
      Location cache = Location_Cache(cacheName);
      if (cache->Exists()) {
        debugprint;
        LoadFrom(result, cache, 0, 0);
        return;
      }
#endif

#ifdef ENABLE_SCRIPT_CACHING
      SaveTo(result, cache, 0);
#endif

#endif

namespace LTE {
  void ScriptFunctionT::Call(void* returnValue, void** args) {
    if (!expression)
      return;

    FRAME(&name.front()) {
      Environment env;
      env.registers.reserve(32);
      for (size_t i = 0; i < parameters.size(); ++i)
        env.registers.push(args[i]);
      expression->Evaluate(returnValue, env);
    }
  }
}
