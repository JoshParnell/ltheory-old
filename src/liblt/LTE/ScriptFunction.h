#ifndef LTE_ScriptFunction_h__
#define LTE_ScriptFunction_h__

#include "AutoClass.h"
#include "Expression.h"
#include "String.h"
#include "Vector.h"

namespace LTE {
  AutoClassDerived(ScriptFunctionT, RefCounted,
    String, name,
    Expression, expression,
    Type, returnType,
    Vector<Parameter>, parameters,
    Function, function)

    ScriptFunctionT() {}

    LT_API void Call(void* returnValue, void** args);

    template <class T>
    void Call(T& returnValue) {
      LTE_ASSERT(returnType == Type_Get<T>())
      LTE_ASSERT(parameters.size() == 0)

      Call(&returnValue, (void**)nullptr);
    }

    template <class T, class P0>
    void Call(T& returnValue, P0 const& p0) {
      LTE_ASSERT(returnType == Type_Get<T>())
      LTE_ASSERT(parameters.size() == 1)
      LTE_ASSERT(parameters[0].type == Type_Get<P0>());

      void* args[] = { (void*)&p0 };
      Call(&returnValue, args);
    }

    template <class T, class P0, class P1>
    void Call(T& returnValue, P0 const& p0, P1 const& p1) {
      LTE_ASSERT(returnType == Type_Get<T>())
      LTE_ASSERT(parameters.size() == 2)
      LTE_ASSERT(parameters[0].type == Type_Get<P0>());
      LTE_ASSERT(parameters[1].type == Type_Get<P1>());

      void* args[] = { (void*)&p0, (void*)&p1 };
      Call(&returnValue, args);
    }

    template <class T, class P0, class P1, class P2>
    void Call(T& returnValue, P0 const& p0, P1 const& p1, P2 const& p2) {
      LTE_ASSERT(returnType == Type_Get<T>())
      LTE_ASSERT(parameters.size() == 3)
      LTE_ASSERT(parameters[0].type == Type_Get<P0>());
      LTE_ASSERT(parameters[1].type == Type_Get<P1>());
      LTE_ASSERT(parameters[2].type == Type_Get<P2>());

      void* args[] = { (void*)&p0, (void*)&p1, (void*)&p2 };
      Call(&returnValue, args);
    }

    void VoidCall(void* returnValue) {
      LTE_ASSERT(parameters.size() == 0)

      bool constructRV = !returnValue && returnType->allocate;
      if (constructRV) returnValue = returnType->Allocate();
      Call(returnValue, (void**)nullptr);
      if (constructRV) returnType->Deallocate(returnValue);
    }

    void VoidCall(
      void* returnValue,
      DataRef const& p0)
    {
      LTE_ASSERT(parameters.size() == 1)
      LTE_ASSERT(parameters[0].type == p0.type)

      void* args[] = { p0.data };
      bool constructRV = !returnValue && returnType->allocate;
      if (constructRV) returnValue = returnType->Allocate();
      Call(returnValue, args);
      if (constructRV) returnType->Deallocate(returnValue);
    }

    void VoidCall(
      void* returnValue,
      DataRef const& p0,
      DataRef const& p1)
    {
      LTE_ASSERT(parameters.size() == 2)
      LTE_ASSERT(parameters[0].type == p0.type)
      LTE_ASSERT(parameters[1].type == p1.type)

      void* args[] = { p0.data, p1.data };
      bool constructRV = !returnValue && returnType->allocate;
      if (constructRV) returnValue = returnType->Allocate();
      Call(returnValue, args);
      if (constructRV) returnType->Deallocate(returnValue);
    }

    void VoidCall(
      void* returnValue,
      DataRef const& p0,
      DataRef const& p1,
      DataRef const& p2)
    {
      LTE_ASSERT(parameters.size() == 3)
      LTE_ASSERT(parameters[0].type == p0.type)
      LTE_ASSERT(parameters[1].type == p1.type)
      LTE_ASSERT(parameters[2].type == p2.type)

      void* args[] = { p0.data, p1.data, p2.data };
      bool constructRV = !returnValue && returnType->allocate;
      if (constructRV) returnValue = returnType->Allocate();
      Call(returnValue, args);
      if (constructRV) returnType->Deallocate(returnValue);
    }
  };
}

#endif
