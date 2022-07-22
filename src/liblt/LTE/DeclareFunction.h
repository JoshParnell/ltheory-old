/* DeclareFunction.py ~ Automatically-generated code */

#ifndef LTE_DeclareFunction_h__
#define LTE_DeclareFunction_h__

#include "AutoClass.h"
#include "Function.h"
#include "Type.h"

#define _DeclareFunction(Name, ReturnType, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, x, ...) \
  MACRO_IDENTITY(DeclareFunction##x(Name, ReturnType, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23))

#define DeclareFunction(Name, ReturnType, ...) \
  MACRO_IDENTITY(_DeclareFunction(Name, ReturnType, __VA_ARGS__, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1))

#define _DeclareFunctionArgBind(Name, ReturnType, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, x, ...) \
  MACRO_IDENTITY(DeclareFunctionArgBind##x(Name, ReturnType, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23))

#define DeclareFunctionArgBind(Name, ReturnType, ...) \
  MACRO_IDENTITY(_DeclareFunctionArgBind(Name, ReturnType, __VA_ARGS__, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1))

#define DeclareFunctionNoParams(Name, ReturnType) \
  MACRO_IDENTITY(DeclareFunction0(Name, ReturnType))
#define DefineFunction(Name) RegisterFunction(Name) Name##_ReturnType Name(Name##_ArgRefs const& args)

#define RegisterFunction(Name)                                                 \
  Function Name##_GetMetadata() {                                              \
    static Function fn;                                                        \
    if (!fn) {                                                                 \
      fn = Function_Create(#Name);                                             \
      fn->description = "None";                                                \
      fn->call = Name##_Call;                                                  \
      fn->paramCount = Name##_ParamCount();                                    \
      fn->params = nullptr;                                                    \
      fn->returnType = Type_Get<Name##_ReturnType>();                          \
      Infer_MetaData(fn, &Name##_ExplicitCall);                                \
      for (uint i = 0; i < fn->paramCount; ++i)                                \
        Mutable(fn->params[i]).name = Name##_ParamName(i);                     \
    }                                                                          \
    return fn;                                                                 \
  }                                                                            \
  static Function Name##_Metadata = Name##_GetMetadata();                      \

template <class RT>
inline void CallAndAssign(void** in, void* out, RT (*fn)()) {
  *(RT*)out = fn();
}

template <>
inline void CallAndAssign(void** in, void* out, void (*fn)()) {
  fn();
}

template <class RT>
void Infer_MetaData(Function const& type, RT (*fn)()) {}

#define DeclareFunction0(Name, ReturnType)                                     \
  inline uint Name##_ParamCount() { return 0; }                                \
  inline char const* Name##_ParamName(uint i) { return 0; }                    \
  typedef ReturnType Name##_ReturnType;                                        \
                                                                               \
  typedef int Name##_ArgRefs;                                                  \
  LT_API ReturnType Name(Name##_ArgRefs const&);                               \
  inline ReturnType Name() { return Name(0); }                                 \
  inline ReturnType Name##_ExplicitCall() { return Name(0); }                  \
  inline void Name##_Call(void** in, void* out) {                              \
    CallAndAssign(in, out, Name##_ExplicitCall);                               \
  }                                                                            \

template <class RT, class T0>
inline void CallAndAssign(void** in, void* out, RT (*fn)(T0 const&)) {
  *(RT*)out = fn(*(T0*)in[0]);
}

template <class T0>
inline void CallAndAssign(void** in, void* out, void (*fn)(T0 const&)) {
  fn(*(T0*)in[0]);
}

template <class RT, class T0>
void Infer_MetaData(Function const& type, RT (*fn)(T0 const&)) {
  type->params = new Parameter[1];
  Mutable(type->params[0]).type = Type_Get<T0>();
}

#define DeclareFunction2(Name, ReturnType, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11)\
  inline uint Name##_ParamCount() { return 1; }                                \
  inline char const* Name##_ParamName(uint i) {                                \
    char const* const table[] = {#N0};                                         \
    return table[i];                                                           \
  }                                                                            \
  typedef ReturnType Name##_ReturnType;                                        \
  typedef T0 Name##_ParamType0;                                                \
                                                                               \
  struct Name##_ArgRefs {                                                      \
    T0 const& N0;                                                              \
    Name##_ArgRefs(T0 const& N0) : N0(N0) {}                                   \
  };                                                                           \
                                                                               \
  LT_API ReturnType Name(Name##_ArgRefs const&);                               \
  inline ReturnType Name(T0 const& N0) {                                       \
    return Name(Name##_ArgRefs(N0));                                           \
  }                                                                            \
  inline ReturnType Name##_ExplicitCall(T0 const& N0) {                        \
    return Name(Name##_ArgRefs(N0));                                           \
  }                                                                            \
  inline void Name##_Call(void** in, void* out) {                              \
    CallAndAssign(in, out, Name##_ExplicitCall);                               \
  }                                                                            \

#define DeclareFunctionArgBind2(Name, ReturnType, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11)\
  DeclareFunction2(Name, ReturnType, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11)\
  AutoClass(Name##_Args, T0, N0)                                               \
    Name##_Args() {}                                                           \
    Name##_Args(Name##_ArgRefs const& args) : N0(args.N0) {}                   \
  };                                                                           \
  inline ReturnType Name(Name##_Args const& args) {                            \
    return Name(Name##_ArgRefs(args.N0));                                      \
  }                                                                            \

template <class RT, class T0, class T1>
inline void CallAndAssign(void** in, void* out, RT (*fn)(T0 const&, T1 const&)) {
  *(RT*)out = fn(*(T0*)in[0], *(T1*)in[1]);
}

template <class T0, class T1>
inline void CallAndAssign(void** in, void* out, void (*fn)(T0 const&, T1 const&)) {
  fn(*(T0*)in[0], *(T1*)in[1]);
}

template <class RT, class T0, class T1>
void Infer_MetaData(Function const& type, RT (*fn)(T0 const&, T1 const&)) {
  type->params = new Parameter[2];
  Mutable(type->params[0]).type = Type_Get<T0>();
  Mutable(type->params[1]).type = Type_Get<T1>();
}

#define DeclareFunction4(Name, ReturnType, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11)\
  inline uint Name##_ParamCount() { return 2; }                                \
  inline char const* Name##_ParamName(uint i) {                                \
    char const* const table[] = {#N0, #N1};                                    \
    return table[i];                                                           \
  }                                                                            \
  typedef ReturnType Name##_ReturnType;                                        \
  typedef T0 Name##_ParamType0;                                                \
  typedef T1 Name##_ParamType1;                                                \
                                                                               \
  struct Name##_ArgRefs {                                                      \
    T0 const& N0;                                                              \
    T1 const& N1;                                                              \
    Name##_ArgRefs(T0 const& N0, T1 const& N1) : N0(N0), N1(N1) {}             \
  };                                                                           \
                                                                               \
  LT_API ReturnType Name(Name##_ArgRefs const&);                               \
  inline ReturnType Name(T0 const& N0, T1 const& N1) {                         \
    return Name(Name##_ArgRefs(N0, N1));                                       \
  }                                                                            \
  inline ReturnType Name##_ExplicitCall(T0 const& N0, T1 const& N1) {          \
    return Name(Name##_ArgRefs(N0, N1));                                       \
  }                                                                            \
  inline void Name##_Call(void** in, void* out) {                              \
    CallAndAssign(in, out, Name##_ExplicitCall);                               \
  }                                                                            \

#define DeclareFunctionArgBind4(Name, ReturnType, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11)\
  DeclareFunction4(Name, ReturnType, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11)\
  AutoClass(Name##_Args, T0, N0, T1, N1)                                       \
    Name##_Args() {}                                                           \
    Name##_Args(Name##_ArgRefs const& args) : N0(args.N0), N1(args.N1) {}      \
  };                                                                           \
  inline ReturnType Name(Name##_Args const& args) {                            \
    return Name(Name##_ArgRefs(args.N0, args.N1));                             \
  }                                                                            \

template <class RT, class T0, class T1, class T2>
inline void CallAndAssign(void** in, void* out, RT (*fn)(T0 const&, T1 const&, T2 const&)) {
  *(RT*)out = fn(*(T0*)in[0], *(T1*)in[1], *(T2*)in[2]);
}

template <class T0, class T1, class T2>
inline void CallAndAssign(void** in, void* out, void (*fn)(T0 const&, T1 const&, T2 const&)) {
  fn(*(T0*)in[0], *(T1*)in[1], *(T2*)in[2]);
}

template <class RT, class T0, class T1, class T2>
void Infer_MetaData(Function const& type, RT (*fn)(T0 const&, T1 const&, T2 const&)) {
  type->params = new Parameter[3];
  Mutable(type->params[0]).type = Type_Get<T0>();
  Mutable(type->params[1]).type = Type_Get<T1>();
  Mutable(type->params[2]).type = Type_Get<T2>();
}

#define DeclareFunction6(Name, ReturnType, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11)\
  inline uint Name##_ParamCount() { return 3; }                                \
  inline char const* Name##_ParamName(uint i) {                                \
    char const* const table[] = {#N0, #N1, #N2};                               \
    return table[i];                                                           \
  }                                                                            \
  typedef ReturnType Name##_ReturnType;                                        \
  typedef T0 Name##_ParamType0;                                                \
  typedef T1 Name##_ParamType1;                                                \
  typedef T2 Name##_ParamType2;                                                \
                                                                               \
  struct Name##_ArgRefs {                                                      \
    T0 const& N0;                                                              \
    T1 const& N1;                                                              \
    T2 const& N2;                                                              \
    Name##_ArgRefs(T0 const& N0, T1 const& N1, T2 const& N2) : N0(N0), N1(N1), N2(N2) {}\
  };                                                                           \
                                                                               \
  LT_API ReturnType Name(Name##_ArgRefs const&);                               \
  inline ReturnType Name(T0 const& N0, T1 const& N1, T2 const& N2) {           \
    return Name(Name##_ArgRefs(N0, N1, N2));                                   \
  }                                                                            \
  inline ReturnType Name##_ExplicitCall(T0 const& N0, T1 const& N1, T2 const& N2) {\
    return Name(Name##_ArgRefs(N0, N1, N2));                                   \
  }                                                                            \
  inline void Name##_Call(void** in, void* out) {                              \
    CallAndAssign(in, out, Name##_ExplicitCall);                               \
  }                                                                            \

#define DeclareFunctionArgBind6(Name, ReturnType, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11)\
  DeclareFunction6(Name, ReturnType, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11)\
  AutoClass(Name##_Args, T0, N0, T1, N1, T2, N2)                               \
    Name##_Args() {}                                                           \
    Name##_Args(Name##_ArgRefs const& args) : N0(args.N0), N1(args.N1), N2(args.N2) {}\
  };                                                                           \
  inline ReturnType Name(Name##_Args const& args) {                            \
    return Name(Name##_ArgRefs(args.N0, args.N1, args.N2));                    \
  }                                                                            \

template <class RT, class T0, class T1, class T2, class T3>
inline void CallAndAssign(void** in, void* out, RT (*fn)(T0 const&, T1 const&, T2 const&, T3 const&)) {
  *(RT*)out = fn(*(T0*)in[0], *(T1*)in[1], *(T2*)in[2], *(T3*)in[3]);
}

template <class T0, class T1, class T2, class T3>
inline void CallAndAssign(void** in, void* out, void (*fn)(T0 const&, T1 const&, T2 const&, T3 const&)) {
  fn(*(T0*)in[0], *(T1*)in[1], *(T2*)in[2], *(T3*)in[3]);
}

template <class RT, class T0, class T1, class T2, class T3>
void Infer_MetaData(Function const& type, RT (*fn)(T0 const&, T1 const&, T2 const&, T3 const&)) {
  type->params = new Parameter[4];
  Mutable(type->params[0]).type = Type_Get<T0>();
  Mutable(type->params[1]).type = Type_Get<T1>();
  Mutable(type->params[2]).type = Type_Get<T2>();
  Mutable(type->params[3]).type = Type_Get<T3>();
}

#define DeclareFunction8(Name, ReturnType, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11)\
  inline uint Name##_ParamCount() { return 4; }                                \
  inline char const* Name##_ParamName(uint i) {                                \
    char const* const table[] = {#N0, #N1, #N2, #N3};                          \
    return table[i];                                                           \
  }                                                                            \
  typedef ReturnType Name##_ReturnType;                                        \
  typedef T0 Name##_ParamType0;                                                \
  typedef T1 Name##_ParamType1;                                                \
  typedef T2 Name##_ParamType2;                                                \
  typedef T3 Name##_ParamType3;                                                \
                                                                               \
  struct Name##_ArgRefs {                                                      \
    T0 const& N0;                                                              \
    T1 const& N1;                                                              \
    T2 const& N2;                                                              \
    T3 const& N3;                                                              \
    Name##_ArgRefs(T0 const& N0, T1 const& N1, T2 const& N2, T3 const& N3) : N0(N0), N1(N1), N2(N2), N3(N3) {}\
  };                                                                           \
                                                                               \
  LT_API ReturnType Name(Name##_ArgRefs const&);                               \
  inline ReturnType Name(T0 const& N0, T1 const& N1, T2 const& N2, T3 const& N3) {\
    return Name(Name##_ArgRefs(N0, N1, N2, N3));                               \
  }                                                                            \
  inline ReturnType Name##_ExplicitCall(T0 const& N0, T1 const& N1, T2 const& N2, T3 const& N3) {\
    return Name(Name##_ArgRefs(N0, N1, N2, N3));                               \
  }                                                                            \
  inline void Name##_Call(void** in, void* out) {                              \
    CallAndAssign(in, out, Name##_ExplicitCall);                               \
  }                                                                            \

#define DeclareFunctionArgBind8(Name, ReturnType, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11)\
  DeclareFunction8(Name, ReturnType, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11)\
  AutoClass(Name##_Args, T0, N0, T1, N1, T2, N2, T3, N3)                       \
    Name##_Args() {}                                                           \
    Name##_Args(Name##_ArgRefs const& args) : N0(args.N0), N1(args.N1), N2(args.N2), N3(args.N3) {}\
  };                                                                           \
  inline ReturnType Name(Name##_Args const& args) {                            \
    return Name(Name##_ArgRefs(args.N0, args.N1, args.N2, args.N3));           \
  }                                                                            \

template <class RT, class T0, class T1, class T2, class T3, class T4>
inline void CallAndAssign(void** in, void* out, RT (*fn)(T0 const&, T1 const&, T2 const&, T3 const&, T4 const&)) {
  *(RT*)out = fn(*(T0*)in[0], *(T1*)in[1], *(T2*)in[2], *(T3*)in[3], *(T4*)in[4]);
}

template <class T0, class T1, class T2, class T3, class T4>
inline void CallAndAssign(void** in, void* out, void (*fn)(T0 const&, T1 const&, T2 const&, T3 const&, T4 const&)) {
  fn(*(T0*)in[0], *(T1*)in[1], *(T2*)in[2], *(T3*)in[3], *(T4*)in[4]);
}

template <class RT, class T0, class T1, class T2, class T3, class T4>
void Infer_MetaData(Function const& type, RT (*fn)(T0 const&, T1 const&, T2 const&, T3 const&, T4 const&)) {
  type->params = new Parameter[5];
  Mutable(type->params[0]).type = Type_Get<T0>();
  Mutable(type->params[1]).type = Type_Get<T1>();
  Mutable(type->params[2]).type = Type_Get<T2>();
  Mutable(type->params[3]).type = Type_Get<T3>();
  Mutable(type->params[4]).type = Type_Get<T4>();
}

#define DeclareFunction10(Name, ReturnType, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11)\
  inline uint Name##_ParamCount() { return 5; }                                \
  inline char const* Name##_ParamName(uint i) {                                \
    char const* const table[] = {#N0, #N1, #N2, #N3, #N4};                     \
    return table[i];                                                           \
  }                                                                            \
  typedef ReturnType Name##_ReturnType;                                        \
  typedef T0 Name##_ParamType0;                                                \
  typedef T1 Name##_ParamType1;                                                \
  typedef T2 Name##_ParamType2;                                                \
  typedef T3 Name##_ParamType3;                                                \
  typedef T4 Name##_ParamType4;                                                \
                                                                               \
  struct Name##_ArgRefs {                                                      \
    T0 const& N0;                                                              \
    T1 const& N1;                                                              \
    T2 const& N2;                                                              \
    T3 const& N3;                                                              \
    T4 const& N4;                                                              \
    Name##_ArgRefs(T0 const& N0, T1 const& N1, T2 const& N2, T3 const& N3, T4 const& N4) : N0(N0), N1(N1), N2(N2), N3(N3), N4(N4) {}\
  };                                                                           \
                                                                               \
  LT_API ReturnType Name(Name##_ArgRefs const&);                               \
  inline ReturnType Name(T0 const& N0, T1 const& N1, T2 const& N2, T3 const& N3, T4 const& N4) {\
    return Name(Name##_ArgRefs(N0, N1, N2, N3, N4));                           \
  }                                                                            \
  inline ReturnType Name##_ExplicitCall(T0 const& N0, T1 const& N1, T2 const& N2, T3 const& N3, T4 const& N4) {\
    return Name(Name##_ArgRefs(N0, N1, N2, N3, N4));                           \
  }                                                                            \
  inline void Name##_Call(void** in, void* out) {                              \
    CallAndAssign(in, out, Name##_ExplicitCall);                               \
  }                                                                            \

#define DeclareFunctionArgBind10(Name, ReturnType, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11)\
  DeclareFunction10(Name, ReturnType, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11)\
  AutoClass(Name##_Args, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4)               \
    Name##_Args() {}                                                           \
    Name##_Args(Name##_ArgRefs const& args) : N0(args.N0), N1(args.N1), N2(args.N2), N3(args.N3), N4(args.N4) {}\
  };                                                                           \
  inline ReturnType Name(Name##_Args const& args) {                            \
    return Name(Name##_ArgRefs(args.N0, args.N1, args.N2, args.N3, args.N4));  \
  }                                                                            \

template <class RT, class T0, class T1, class T2, class T3, class T4, class T5>
inline void CallAndAssign(void** in, void* out, RT (*fn)(T0 const&, T1 const&, T2 const&, T3 const&, T4 const&, T5 const&)) {
  *(RT*)out = fn(*(T0*)in[0], *(T1*)in[1], *(T2*)in[2], *(T3*)in[3], *(T4*)in[4], *(T5*)in[5]);
}

template <class T0, class T1, class T2, class T3, class T4, class T5>
inline void CallAndAssign(void** in, void* out, void (*fn)(T0 const&, T1 const&, T2 const&, T3 const&, T4 const&, T5 const&)) {
  fn(*(T0*)in[0], *(T1*)in[1], *(T2*)in[2], *(T3*)in[3], *(T4*)in[4], *(T5*)in[5]);
}

template <class RT, class T0, class T1, class T2, class T3, class T4, class T5>
void Infer_MetaData(Function const& type, RT (*fn)(T0 const&, T1 const&, T2 const&, T3 const&, T4 const&, T5 const&)) {
  type->params = new Parameter[6];
  Mutable(type->params[0]).type = Type_Get<T0>();
  Mutable(type->params[1]).type = Type_Get<T1>();
  Mutable(type->params[2]).type = Type_Get<T2>();
  Mutable(type->params[3]).type = Type_Get<T3>();
  Mutable(type->params[4]).type = Type_Get<T4>();
  Mutable(type->params[5]).type = Type_Get<T5>();
}

#define DeclareFunction12(Name, ReturnType, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11)\
  inline uint Name##_ParamCount() { return 6; }                                \
  inline char const* Name##_ParamName(uint i) {                                \
    char const* const table[] = {#N0, #N1, #N2, #N3, #N4, #N5};                \
    return table[i];                                                           \
  }                                                                            \
  typedef ReturnType Name##_ReturnType;                                        \
  typedef T0 Name##_ParamType0;                                                \
  typedef T1 Name##_ParamType1;                                                \
  typedef T2 Name##_ParamType2;                                                \
  typedef T3 Name##_ParamType3;                                                \
  typedef T4 Name##_ParamType4;                                                \
  typedef T5 Name##_ParamType5;                                                \
                                                                               \
  struct Name##_ArgRefs {                                                      \
    T0 const& N0;                                                              \
    T1 const& N1;                                                              \
    T2 const& N2;                                                              \
    T3 const& N3;                                                              \
    T4 const& N4;                                                              \
    T5 const& N5;                                                              \
    Name##_ArgRefs(T0 const& N0, T1 const& N1, T2 const& N2, T3 const& N3, T4 const& N4, T5 const& N5) : N0(N0), N1(N1), N2(N2), N3(N3), N4(N4), N5(N5) {}\
  };                                                                           \
                                                                               \
  LT_API ReturnType Name(Name##_ArgRefs const&);                               \
  inline ReturnType Name(T0 const& N0, T1 const& N1, T2 const& N2, T3 const& N3, T4 const& N4, T5 const& N5) {\
    return Name(Name##_ArgRefs(N0, N1, N2, N3, N4, N5));                       \
  }                                                                            \
  inline ReturnType Name##_ExplicitCall(T0 const& N0, T1 const& N1, T2 const& N2, T3 const& N3, T4 const& N4, T5 const& N5) {\
    return Name(Name##_ArgRefs(N0, N1, N2, N3, N4, N5));                       \
  }                                                                            \
  inline void Name##_Call(void** in, void* out) {                              \
    CallAndAssign(in, out, Name##_ExplicitCall);                               \
  }                                                                            \

#define DeclareFunctionArgBind12(Name, ReturnType, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11)\
  DeclareFunction12(Name, ReturnType, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11)\
  AutoClass(Name##_Args, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5)       \
    Name##_Args() {}                                                           \
    Name##_Args(Name##_ArgRefs const& args) : N0(args.N0), N1(args.N1), N2(args.N2), N3(args.N3), N4(args.N4), N5(args.N5) {}\
  };                                                                           \
  inline ReturnType Name(Name##_Args const& args) {                            \
    return Name(Name##_ArgRefs(args.N0, args.N1, args.N2, args.N3, args.N4, args.N5));\
  }                                                                            \

template <class RT, class T0, class T1, class T2, class T3, class T4, class T5, class T6>
inline void CallAndAssign(void** in, void* out, RT (*fn)(T0 const&, T1 const&, T2 const&, T3 const&, T4 const&, T5 const&, T6 const&)) {
  *(RT*)out = fn(*(T0*)in[0], *(T1*)in[1], *(T2*)in[2], *(T3*)in[3], *(T4*)in[4], *(T5*)in[5], *(T6*)in[6]);
}

template <class T0, class T1, class T2, class T3, class T4, class T5, class T6>
inline void CallAndAssign(void** in, void* out, void (*fn)(T0 const&, T1 const&, T2 const&, T3 const&, T4 const&, T5 const&, T6 const&)) {
  fn(*(T0*)in[0], *(T1*)in[1], *(T2*)in[2], *(T3*)in[3], *(T4*)in[4], *(T5*)in[5], *(T6*)in[6]);
}

template <class RT, class T0, class T1, class T2, class T3, class T4, class T5, class T6>
void Infer_MetaData(Function const& type, RT (*fn)(T0 const&, T1 const&, T2 const&, T3 const&, T4 const&, T5 const&, T6 const&)) {
  type->params = new Parameter[7];
  Mutable(type->params[0]).type = Type_Get<T0>();
  Mutable(type->params[1]).type = Type_Get<T1>();
  Mutable(type->params[2]).type = Type_Get<T2>();
  Mutable(type->params[3]).type = Type_Get<T3>();
  Mutable(type->params[4]).type = Type_Get<T4>();
  Mutable(type->params[5]).type = Type_Get<T5>();
  Mutable(type->params[6]).type = Type_Get<T6>();
}

#define DeclareFunction14(Name, ReturnType, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11)\
  inline uint Name##_ParamCount() { return 7; }                                \
  inline char const* Name##_ParamName(uint i) {                                \
    char const* const table[] = {#N0, #N1, #N2, #N3, #N4, #N5, #N6};           \
    return table[i];                                                           \
  }                                                                            \
  typedef ReturnType Name##_ReturnType;                                        \
  typedef T0 Name##_ParamType0;                                                \
  typedef T1 Name##_ParamType1;                                                \
  typedef T2 Name##_ParamType2;                                                \
  typedef T3 Name##_ParamType3;                                                \
  typedef T4 Name##_ParamType4;                                                \
  typedef T5 Name##_ParamType5;                                                \
  typedef T6 Name##_ParamType6;                                                \
                                                                               \
  struct Name##_ArgRefs {                                                      \
    T0 const& N0;                                                              \
    T1 const& N1;                                                              \
    T2 const& N2;                                                              \
    T3 const& N3;                                                              \
    T4 const& N4;                                                              \
    T5 const& N5;                                                              \
    T6 const& N6;                                                              \
    Name##_ArgRefs(T0 const& N0, T1 const& N1, T2 const& N2, T3 const& N3, T4 const& N4, T5 const& N5, T6 const& N6) : N0(N0), N1(N1), N2(N2), N3(N3), N4(N4), N5(N5), N6(N6) {}\
  };                                                                           \
                                                                               \
  LT_API ReturnType Name(Name##_ArgRefs const&);                               \
  inline ReturnType Name(T0 const& N0, T1 const& N1, T2 const& N2, T3 const& N3, T4 const& N4, T5 const& N5, T6 const& N6) {\
    return Name(Name##_ArgRefs(N0, N1, N2, N3, N4, N5, N6));                   \
  }                                                                            \
  inline ReturnType Name##_ExplicitCall(T0 const& N0, T1 const& N1, T2 const& N2, T3 const& N3, T4 const& N4, T5 const& N5, T6 const& N6) {\
    return Name(Name##_ArgRefs(N0, N1, N2, N3, N4, N5, N6));                   \
  }                                                                            \
  inline void Name##_Call(void** in, void* out) {                              \
    CallAndAssign(in, out, Name##_ExplicitCall);                               \
  }                                                                            \

#define DeclareFunctionArgBind14(Name, ReturnType, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11)\
  DeclareFunction14(Name, ReturnType, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11)\
  AutoClass(Name##_Args, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6) \
    Name##_Args() {}                                                           \
    Name##_Args(Name##_ArgRefs const& args) : N0(args.N0), N1(args.N1), N2(args.N2), N3(args.N3), N4(args.N4), N5(args.N5), N6(args.N6) {}\
  };                                                                           \
  inline ReturnType Name(Name##_Args const& args) {                            \
    return Name(Name##_ArgRefs(args.N0, args.N1, args.N2, args.N3, args.N4, args.N5, args.N6));\
  }                                                                            \

template <class RT, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
inline void CallAndAssign(void** in, void* out, RT (*fn)(T0 const&, T1 const&, T2 const&, T3 const&, T4 const&, T5 const&, T6 const&, T7 const&)) {
  *(RT*)out = fn(*(T0*)in[0], *(T1*)in[1], *(T2*)in[2], *(T3*)in[3], *(T4*)in[4], *(T5*)in[5], *(T6*)in[6], *(T7*)in[7]);
}

template <class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
inline void CallAndAssign(void** in, void* out, void (*fn)(T0 const&, T1 const&, T2 const&, T3 const&, T4 const&, T5 const&, T6 const&, T7 const&)) {
  fn(*(T0*)in[0], *(T1*)in[1], *(T2*)in[2], *(T3*)in[3], *(T4*)in[4], *(T5*)in[5], *(T6*)in[6], *(T7*)in[7]);
}

template <class RT, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
void Infer_MetaData(Function const& type, RT (*fn)(T0 const&, T1 const&, T2 const&, T3 const&, T4 const&, T5 const&, T6 const&, T7 const&)) {
  type->params = new Parameter[8];
  Mutable(type->params[0]).type = Type_Get<T0>();
  Mutable(type->params[1]).type = Type_Get<T1>();
  Mutable(type->params[2]).type = Type_Get<T2>();
  Mutable(type->params[3]).type = Type_Get<T3>();
  Mutable(type->params[4]).type = Type_Get<T4>();
  Mutable(type->params[5]).type = Type_Get<T5>();
  Mutable(type->params[6]).type = Type_Get<T6>();
  Mutable(type->params[7]).type = Type_Get<T7>();
}

#define DeclareFunction16(Name, ReturnType, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11)\
  inline uint Name##_ParamCount() { return 8; }                                \
  inline char const* Name##_ParamName(uint i) {                                \
    char const* const table[] = {#N0, #N1, #N2, #N3, #N4, #N5, #N6, #N7};      \
    return table[i];                                                           \
  }                                                                            \
  typedef ReturnType Name##_ReturnType;                                        \
  typedef T0 Name##_ParamType0;                                                \
  typedef T1 Name##_ParamType1;                                                \
  typedef T2 Name##_ParamType2;                                                \
  typedef T3 Name##_ParamType3;                                                \
  typedef T4 Name##_ParamType4;                                                \
  typedef T5 Name##_ParamType5;                                                \
  typedef T6 Name##_ParamType6;                                                \
  typedef T7 Name##_ParamType7;                                                \
                                                                               \
  struct Name##_ArgRefs {                                                      \
    T0 const& N0;                                                              \
    T1 const& N1;                                                              \
    T2 const& N2;                                                              \
    T3 const& N3;                                                              \
    T4 const& N4;                                                              \
    T5 const& N5;                                                              \
    T6 const& N6;                                                              \
    T7 const& N7;                                                              \
    Name##_ArgRefs(T0 const& N0, T1 const& N1, T2 const& N2, T3 const& N3, T4 const& N4, T5 const& N5, T6 const& N6, T7 const& N7) : N0(N0), N1(N1), N2(N2), N3(N3), N4(N4), N5(N5), N6(N6), N7(N7) {}\
  };                                                                           \
                                                                               \
  LT_API ReturnType Name(Name##_ArgRefs const&);                               \
  inline ReturnType Name(T0 const& N0, T1 const& N1, T2 const& N2, T3 const& N3, T4 const& N4, T5 const& N5, T6 const& N6, T7 const& N7) {\
    return Name(Name##_ArgRefs(N0, N1, N2, N3, N4, N5, N6, N7));               \
  }                                                                            \
  inline ReturnType Name##_ExplicitCall(T0 const& N0, T1 const& N1, T2 const& N2, T3 const& N3, T4 const& N4, T5 const& N5, T6 const& N6, T7 const& N7) {\
    return Name(Name##_ArgRefs(N0, N1, N2, N3, N4, N5, N6, N7));               \
  }                                                                            \
  inline void Name##_Call(void** in, void* out) {                              \
    CallAndAssign(in, out, Name##_ExplicitCall);                               \
  }                                                                            \

#define DeclareFunctionArgBind16(Name, ReturnType, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11)\
  DeclareFunction16(Name, ReturnType, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11)\
  AutoClass(Name##_Args, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
    Name##_Args() {}                                                           \
    Name##_Args(Name##_ArgRefs const& args) : N0(args.N0), N1(args.N1), N2(args.N2), N3(args.N3), N4(args.N4), N5(args.N5), N6(args.N6), N7(args.N7) {}\
  };                                                                           \
  inline ReturnType Name(Name##_Args const& args) {                            \
    return Name(Name##_ArgRefs(args.N0, args.N1, args.N2, args.N3, args.N4, args.N5, args.N6, args.N7));\
  }                                                                            \

template <class RT, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
inline void CallAndAssign(void** in, void* out, RT (*fn)(T0 const&, T1 const&, T2 const&, T3 const&, T4 const&, T5 const&, T6 const&, T7 const&, T8 const&)) {
  *(RT*)out = fn(*(T0*)in[0], *(T1*)in[1], *(T2*)in[2], *(T3*)in[3], *(T4*)in[4], *(T5*)in[5], *(T6*)in[6], *(T7*)in[7], *(T8*)in[8]);
}

template <class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
inline void CallAndAssign(void** in, void* out, void (*fn)(T0 const&, T1 const&, T2 const&, T3 const&, T4 const&, T5 const&, T6 const&, T7 const&, T8 const&)) {
  fn(*(T0*)in[0], *(T1*)in[1], *(T2*)in[2], *(T3*)in[3], *(T4*)in[4], *(T5*)in[5], *(T6*)in[6], *(T7*)in[7], *(T8*)in[8]);
}

template <class RT, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
void Infer_MetaData(Function const& type, RT (*fn)(T0 const&, T1 const&, T2 const&, T3 const&, T4 const&, T5 const&, T6 const&, T7 const&, T8 const&)) {
  type->params = new Parameter[9];
  Mutable(type->params[0]).type = Type_Get<T0>();
  Mutable(type->params[1]).type = Type_Get<T1>();
  Mutable(type->params[2]).type = Type_Get<T2>();
  Mutable(type->params[3]).type = Type_Get<T3>();
  Mutable(type->params[4]).type = Type_Get<T4>();
  Mutable(type->params[5]).type = Type_Get<T5>();
  Mutable(type->params[6]).type = Type_Get<T6>();
  Mutable(type->params[7]).type = Type_Get<T7>();
  Mutable(type->params[8]).type = Type_Get<T8>();
}

#define DeclareFunction18(Name, ReturnType, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11)\
  inline uint Name##_ParamCount() { return 9; }                                \
  inline char const* Name##_ParamName(uint i) {                                \
    char const* const table[] = {#N0, #N1, #N2, #N3, #N4, #N5, #N6, #N7, #N8}; \
    return table[i];                                                           \
  }                                                                            \
  typedef ReturnType Name##_ReturnType;                                        \
  typedef T0 Name##_ParamType0;                                                \
  typedef T1 Name##_ParamType1;                                                \
  typedef T2 Name##_ParamType2;                                                \
  typedef T3 Name##_ParamType3;                                                \
  typedef T4 Name##_ParamType4;                                                \
  typedef T5 Name##_ParamType5;                                                \
  typedef T6 Name##_ParamType6;                                                \
  typedef T7 Name##_ParamType7;                                                \
  typedef T8 Name##_ParamType8;                                                \
                                                                               \
  struct Name##_ArgRefs {                                                      \
    T0 const& N0;                                                              \
    T1 const& N1;                                                              \
    T2 const& N2;                                                              \
    T3 const& N3;                                                              \
    T4 const& N4;                                                              \
    T5 const& N5;                                                              \
    T6 const& N6;                                                              \
    T7 const& N7;                                                              \
    T8 const& N8;                                                              \
    Name##_ArgRefs(T0 const& N0, T1 const& N1, T2 const& N2, T3 const& N3, T4 const& N4, T5 const& N5, T6 const& N6, T7 const& N7, T8 const& N8) : N0(N0), N1(N1), N2(N2), N3(N3), N4(N4), N5(N5), N6(N6), N7(N7), N8(N8) {}\
  };                                                                           \
                                                                               \
  LT_API ReturnType Name(Name##_ArgRefs const&);                               \
  inline ReturnType Name(T0 const& N0, T1 const& N1, T2 const& N2, T3 const& N3, T4 const& N4, T5 const& N5, T6 const& N6, T7 const& N7, T8 const& N8) {\
    return Name(Name##_ArgRefs(N0, N1, N2, N3, N4, N5, N6, N7, N8));           \
  }                                                                            \
  inline ReturnType Name##_ExplicitCall(T0 const& N0, T1 const& N1, T2 const& N2, T3 const& N3, T4 const& N4, T5 const& N5, T6 const& N6, T7 const& N7, T8 const& N8) {\
    return Name(Name##_ArgRefs(N0, N1, N2, N3, N4, N5, N6, N7, N8));           \
  }                                                                            \
  inline void Name##_Call(void** in, void* out) {                              \
    CallAndAssign(in, out, Name##_ExplicitCall);                               \
  }                                                                            \

#define DeclareFunctionArgBind18(Name, ReturnType, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11)\
  DeclareFunction18(Name, ReturnType, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11)\
  AutoClass(Name##_Args, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8) \
    Name##_Args() {}                                                           \
    Name##_Args(Name##_ArgRefs const& args) : N0(args.N0), N1(args.N1), N2(args.N2), N3(args.N3), N4(args.N4), N5(args.N5), N6(args.N6), N7(args.N7), N8(args.N8) {}\
  };                                                                           \
  inline ReturnType Name(Name##_Args const& args) {                            \
    return Name(Name##_ArgRefs(args.N0, args.N1, args.N2, args.N3, args.N4, args.N5, args.N6, args.N7, args.N8));\
  }                                                                            \

template <class RT, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
inline void CallAndAssign(void** in, void* out, RT (*fn)(T0 const&, T1 const&, T2 const&, T3 const&, T4 const&, T5 const&, T6 const&, T7 const&, T8 const&, T9 const&)) {
  *(RT*)out = fn(*(T0*)in[0], *(T1*)in[1], *(T2*)in[2], *(T3*)in[3], *(T4*)in[4], *(T5*)in[5], *(T6*)in[6], *(T7*)in[7], *(T8*)in[8], *(T9*)in[9]);
}

template <class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
inline void CallAndAssign(void** in, void* out, void (*fn)(T0 const&, T1 const&, T2 const&, T3 const&, T4 const&, T5 const&, T6 const&, T7 const&, T8 const&, T9 const&)) {
  fn(*(T0*)in[0], *(T1*)in[1], *(T2*)in[2], *(T3*)in[3], *(T4*)in[4], *(T5*)in[5], *(T6*)in[6], *(T7*)in[7], *(T8*)in[8], *(T9*)in[9]);
}

template <class RT, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
void Infer_MetaData(Function const& type, RT (*fn)(T0 const&, T1 const&, T2 const&, T3 const&, T4 const&, T5 const&, T6 const&, T7 const&, T8 const&, T9 const&)) {
  type->params = new Parameter[10];
  Mutable(type->params[0]).type = Type_Get<T0>();
  Mutable(type->params[1]).type = Type_Get<T1>();
  Mutable(type->params[2]).type = Type_Get<T2>();
  Mutable(type->params[3]).type = Type_Get<T3>();
  Mutable(type->params[4]).type = Type_Get<T4>();
  Mutable(type->params[5]).type = Type_Get<T5>();
  Mutable(type->params[6]).type = Type_Get<T6>();
  Mutable(type->params[7]).type = Type_Get<T7>();
  Mutable(type->params[8]).type = Type_Get<T8>();
  Mutable(type->params[9]).type = Type_Get<T9>();
}

#define DeclareFunction20(Name, ReturnType, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11)\
  inline uint Name##_ParamCount() { return 10; }                               \
  inline char const* Name##_ParamName(uint i) {                                \
    char const* const table[] = {#N0, #N1, #N2, #N3, #N4, #N5, #N6, #N7, #N8, #N9};\
    return table[i];                                                           \
  }                                                                            \
  typedef ReturnType Name##_ReturnType;                                        \
  typedef T0 Name##_ParamType0;                                                \
  typedef T1 Name##_ParamType1;                                                \
  typedef T2 Name##_ParamType2;                                                \
  typedef T3 Name##_ParamType3;                                                \
  typedef T4 Name##_ParamType4;                                                \
  typedef T5 Name##_ParamType5;                                                \
  typedef T6 Name##_ParamType6;                                                \
  typedef T7 Name##_ParamType7;                                                \
  typedef T8 Name##_ParamType8;                                                \
  typedef T9 Name##_ParamType9;                                                \
                                                                               \
  struct Name##_ArgRefs {                                                      \
    T0 const& N0;                                                              \
    T1 const& N1;                                                              \
    T2 const& N2;                                                              \
    T3 const& N3;                                                              \
    T4 const& N4;                                                              \
    T5 const& N5;                                                              \
    T6 const& N6;                                                              \
    T7 const& N7;                                                              \
    T8 const& N8;                                                              \
    T9 const& N9;                                                              \
    Name##_ArgRefs(T0 const& N0, T1 const& N1, T2 const& N2, T3 const& N3, T4 const& N4, T5 const& N5, T6 const& N6, T7 const& N7, T8 const& N8, T9 const& N9) : N0(N0), N1(N1), N2(N2), N3(N3), N4(N4), N5(N5), N6(N6), N7(N7), N8(N8), N9(N9) {}\
  };                                                                           \
                                                                               \
  LT_API ReturnType Name(Name##_ArgRefs const&);                               \
  inline ReturnType Name(T0 const& N0, T1 const& N1, T2 const& N2, T3 const& N3, T4 const& N4, T5 const& N5, T6 const& N6, T7 const& N7, T8 const& N8, T9 const& N9) {\
    return Name(Name##_ArgRefs(N0, N1, N2, N3, N4, N5, N6, N7, N8, N9));       \
  }                                                                            \
  inline ReturnType Name##_ExplicitCall(T0 const& N0, T1 const& N1, T2 const& N2, T3 const& N3, T4 const& N4, T5 const& N5, T6 const& N6, T7 const& N7, T8 const& N8, T9 const& N9) {\
    return Name(Name##_ArgRefs(N0, N1, N2, N3, N4, N5, N6, N7, N8, N9));       \
  }                                                                            \
  inline void Name##_Call(void** in, void* out) {                              \
    CallAndAssign(in, out, Name##_ExplicitCall);                               \
  }                                                                            \

#define DeclareFunctionArgBind20(Name, ReturnType, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11)\
  DeclareFunction20(Name, ReturnType, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11)\
  AutoClass(Name##_Args, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9) \
    Name##_Args() {}                                                           \
    Name##_Args(Name##_ArgRefs const& args) : N0(args.N0), N1(args.N1), N2(args.N2), N3(args.N3), N4(args.N4), N5(args.N5), N6(args.N6), N7(args.N7), N8(args.N8), N9(args.N9) {}\
  };                                                                           \
  inline ReturnType Name(Name##_Args const& args) {                            \
    return Name(Name##_ArgRefs(args.N0, args.N1, args.N2, args.N3, args.N4, args.N5, args.N6, args.N7, args.N8, args.N9));\
  }                                                                            \

template <class RT, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10>
inline void CallAndAssign(void** in, void* out, RT (*fn)(T0 const&, T1 const&, T2 const&, T3 const&, T4 const&, T5 const&, T6 const&, T7 const&, T8 const&, T9 const&, T10 const&)) {
  *(RT*)out = fn(*(T0*)in[0], *(T1*)in[1], *(T2*)in[2], *(T3*)in[3], *(T4*)in[4], *(T5*)in[5], *(T6*)in[6], *(T7*)in[7], *(T8*)in[8], *(T9*)in[9], *(T10*)in[10]);
}

template <class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10>
inline void CallAndAssign(void** in, void* out, void (*fn)(T0 const&, T1 const&, T2 const&, T3 const&, T4 const&, T5 const&, T6 const&, T7 const&, T8 const&, T9 const&, T10 const&)) {
  fn(*(T0*)in[0], *(T1*)in[1], *(T2*)in[2], *(T3*)in[3], *(T4*)in[4], *(T5*)in[5], *(T6*)in[6], *(T7*)in[7], *(T8*)in[8], *(T9*)in[9], *(T10*)in[10]);
}

template <class RT, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10>
void Infer_MetaData(Function const& type, RT (*fn)(T0 const&, T1 const&, T2 const&, T3 const&, T4 const&, T5 const&, T6 const&, T7 const&, T8 const&, T9 const&, T10 const&)) {
  type->params = new Parameter[11];
  Mutable(type->params[0]).type = Type_Get<T0>();
  Mutable(type->params[1]).type = Type_Get<T1>();
  Mutable(type->params[2]).type = Type_Get<T2>();
  Mutable(type->params[3]).type = Type_Get<T3>();
  Mutable(type->params[4]).type = Type_Get<T4>();
  Mutable(type->params[5]).type = Type_Get<T5>();
  Mutable(type->params[6]).type = Type_Get<T6>();
  Mutable(type->params[7]).type = Type_Get<T7>();
  Mutable(type->params[8]).type = Type_Get<T8>();
  Mutable(type->params[9]).type = Type_Get<T9>();
  Mutable(type->params[10]).type = Type_Get<T10>();
}

#define DeclareFunction22(Name, ReturnType, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11)\
  inline uint Name##_ParamCount() { return 11; }                               \
  inline char const* Name##_ParamName(uint i) {                                \
    char const* const table[] = {#N0, #N1, #N2, #N3, #N4, #N5, #N6, #N7, #N8, #N9, #N10};\
    return table[i];                                                           \
  }                                                                            \
  typedef ReturnType Name##_ReturnType;                                        \
  typedef T0 Name##_ParamType0;                                                \
  typedef T1 Name##_ParamType1;                                                \
  typedef T2 Name##_ParamType2;                                                \
  typedef T3 Name##_ParamType3;                                                \
  typedef T4 Name##_ParamType4;                                                \
  typedef T5 Name##_ParamType5;                                                \
  typedef T6 Name##_ParamType6;                                                \
  typedef T7 Name##_ParamType7;                                                \
  typedef T8 Name##_ParamType8;                                                \
  typedef T9 Name##_ParamType9;                                                \
  typedef T10 Name##_ParamType10;                                              \
                                                                               \
  struct Name##_ArgRefs {                                                      \
    T0 const& N0;                                                              \
    T1 const& N1;                                                              \
    T2 const& N2;                                                              \
    T3 const& N3;                                                              \
    T4 const& N4;                                                              \
    T5 const& N5;                                                              \
    T6 const& N6;                                                              \
    T7 const& N7;                                                              \
    T8 const& N8;                                                              \
    T9 const& N9;                                                              \
    T10 const& N10;                                                            \
    Name##_ArgRefs(T0 const& N0, T1 const& N1, T2 const& N2, T3 const& N3, T4 const& N4, T5 const& N5, T6 const& N6, T7 const& N7, T8 const& N8, T9 const& N9, T10 const& N10) : N0(N0), N1(N1), N2(N2), N3(N3), N4(N4), N5(N5), N6(N6), N7(N7), N8(N8), N9(N9), N10(N10) {}\
  };                                                                           \
                                                                               \
  LT_API ReturnType Name(Name##_ArgRefs const&);                               \
  inline ReturnType Name(T0 const& N0, T1 const& N1, T2 const& N2, T3 const& N3, T4 const& N4, T5 const& N5, T6 const& N6, T7 const& N7, T8 const& N8, T9 const& N9, T10 const& N10) {\
    return Name(Name##_ArgRefs(N0, N1, N2, N3, N4, N5, N6, N7, N8, N9, N10));  \
  }                                                                            \
  inline ReturnType Name##_ExplicitCall(T0 const& N0, T1 const& N1, T2 const& N2, T3 const& N3, T4 const& N4, T5 const& N5, T6 const& N6, T7 const& N7, T8 const& N8, T9 const& N9, T10 const& N10) {\
    return Name(Name##_ArgRefs(N0, N1, N2, N3, N4, N5, N6, N7, N8, N9, N10));  \
  }                                                                            \
  inline void Name##_Call(void** in, void* out) {                              \
    CallAndAssign(in, out, Name##_ExplicitCall);                               \
  }                                                                            \

#define DeclareFunctionArgBind22(Name, ReturnType, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11)\
  DeclareFunction22(Name, ReturnType, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11)\
  AutoClass(Name##_Args, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10) \
    Name##_Args() {}                                                           \
    Name##_Args(Name##_ArgRefs const& args) : N0(args.N0), N1(args.N1), N2(args.N2), N3(args.N3), N4(args.N4), N5(args.N5), N6(args.N6), N7(args.N7), N8(args.N8), N9(args.N9), N10(args.N10) {}\
  };                                                                           \
  inline ReturnType Name(Name##_Args const& args) {                            \
    return Name(Name##_ArgRefs(args.N0, args.N1, args.N2, args.N3, args.N4, args.N5, args.N6, args.N7, args.N8, args.N9, args.N10));\
  }                                                                            \

template <class RT, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11>
inline void CallAndAssign(void** in, void* out, RT (*fn)(T0 const&, T1 const&, T2 const&, T3 const&, T4 const&, T5 const&, T6 const&, T7 const&, T8 const&, T9 const&, T10 const&, T11 const&)) {
  *(RT*)out = fn(*(T0*)in[0], *(T1*)in[1], *(T2*)in[2], *(T3*)in[3], *(T4*)in[4], *(T5*)in[5], *(T6*)in[6], *(T7*)in[7], *(T8*)in[8], *(T9*)in[9], *(T10*)in[10], *(T11*)in[11]);
}

template <class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11>
inline void CallAndAssign(void** in, void* out, void (*fn)(T0 const&, T1 const&, T2 const&, T3 const&, T4 const&, T5 const&, T6 const&, T7 const&, T8 const&, T9 const&, T10 const&, T11 const&)) {
  fn(*(T0*)in[0], *(T1*)in[1], *(T2*)in[2], *(T3*)in[3], *(T4*)in[4], *(T5*)in[5], *(T6*)in[6], *(T7*)in[7], *(T8*)in[8], *(T9*)in[9], *(T10*)in[10], *(T11*)in[11]);
}

template <class RT, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11>
void Infer_MetaData(Function const& type, RT (*fn)(T0 const&, T1 const&, T2 const&, T3 const&, T4 const&, T5 const&, T6 const&, T7 const&, T8 const&, T9 const&, T10 const&, T11 const&)) {
  type->params = new Parameter[12];
  Mutable(type->params[0]).type = Type_Get<T0>();
  Mutable(type->params[1]).type = Type_Get<T1>();
  Mutable(type->params[2]).type = Type_Get<T2>();
  Mutable(type->params[3]).type = Type_Get<T3>();
  Mutable(type->params[4]).type = Type_Get<T4>();
  Mutable(type->params[5]).type = Type_Get<T5>();
  Mutable(type->params[6]).type = Type_Get<T6>();
  Mutable(type->params[7]).type = Type_Get<T7>();
  Mutable(type->params[8]).type = Type_Get<T8>();
  Mutable(type->params[9]).type = Type_Get<T9>();
  Mutable(type->params[10]).type = Type_Get<T10>();
  Mutable(type->params[11]).type = Type_Get<T11>();
}

#define DeclareFunction24(Name, ReturnType, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11)\
  inline uint Name##_ParamCount() { return 12; }                               \
  inline char const* Name##_ParamName(uint i) {                                \
    char const* const table[] = {#N0, #N1, #N2, #N3, #N4, #N5, #N6, #N7, #N8, #N9, #N10, #N11};\
    return table[i];                                                           \
  }                                                                            \
  typedef ReturnType Name##_ReturnType;                                        \
  typedef T0 Name##_ParamType0;                                                \
  typedef T1 Name##_ParamType1;                                                \
  typedef T2 Name##_ParamType2;                                                \
  typedef T3 Name##_ParamType3;                                                \
  typedef T4 Name##_ParamType4;                                                \
  typedef T5 Name##_ParamType5;                                                \
  typedef T6 Name##_ParamType6;                                                \
  typedef T7 Name##_ParamType7;                                                \
  typedef T8 Name##_ParamType8;                                                \
  typedef T9 Name##_ParamType9;                                                \
  typedef T10 Name##_ParamType10;                                              \
  typedef T11 Name##_ParamType11;                                              \
                                                                               \
  struct Name##_ArgRefs {                                                      \
    T0 const& N0;                                                              \
    T1 const& N1;                                                              \
    T2 const& N2;                                                              \
    T3 const& N3;                                                              \
    T4 const& N4;                                                              \
    T5 const& N5;                                                              \
    T6 const& N6;                                                              \
    T7 const& N7;                                                              \
    T8 const& N8;                                                              \
    T9 const& N9;                                                              \
    T10 const& N10;                                                            \
    T11 const& N11;                                                            \
    Name##_ArgRefs(T0 const& N0, T1 const& N1, T2 const& N2, T3 const& N3, T4 const& N4, T5 const& N5, T6 const& N6, T7 const& N7, T8 const& N8, T9 const& N9, T10 const& N10, T11 const& N11) : N0(N0), N1(N1), N2(N2), N3(N3), N4(N4), N5(N5), N6(N6), N7(N7), N8(N8), N9(N9), N10(N10), N11(N11) {}\
  };                                                                           \
                                                                               \
  LT_API ReturnType Name(Name##_ArgRefs const&);                               \
  inline ReturnType Name(T0 const& N0, T1 const& N1, T2 const& N2, T3 const& N3, T4 const& N4, T5 const& N5, T6 const& N6, T7 const& N7, T8 const& N8, T9 const& N9, T10 const& N10, T11 const& N11) {\
    return Name(Name##_ArgRefs(N0, N1, N2, N3, N4, N5, N6, N7, N8, N9, N10, N11));\
  }                                                                            \
  inline ReturnType Name##_ExplicitCall(T0 const& N0, T1 const& N1, T2 const& N2, T3 const& N3, T4 const& N4, T5 const& N5, T6 const& N6, T7 const& N7, T8 const& N8, T9 const& N9, T10 const& N10, T11 const& N11) {\
    return Name(Name##_ArgRefs(N0, N1, N2, N3, N4, N5, N6, N7, N8, N9, N10, N11));\
  }                                                                            \
  inline void Name##_Call(void** in, void* out) {                              \
    CallAndAssign(in, out, Name##_ExplicitCall);                               \
  }                                                                            \

#define DeclareFunctionArgBind24(Name, ReturnType, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11)\
  DeclareFunction24(Name, ReturnType, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11)\
  AutoClass(Name##_Args, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8, T9, N9, T10, N10, T11, N11) \
    Name##_Args() {}                                                           \
    Name##_Args(Name##_ArgRefs const& args) : N0(args.N0), N1(args.N1), N2(args.N2), N3(args.N3), N4(args.N4), N5(args.N5), N6(args.N6), N7(args.N7), N8(args.N8), N9(args.N9), N10(args.N10), N11(args.N11) {}\
  };                                                                           \
  inline ReturnType Name(Name##_Args const& args) {                            \
    return Name(Name##_ArgRefs(args.N0, args.N1, args.N2, args.N3, args.N4, args.N5, args.N6, args.N7, args.N8, args.N9, args.N10, args.N11));\
  }                                                                            \


#endif
