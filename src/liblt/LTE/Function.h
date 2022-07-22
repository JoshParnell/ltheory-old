#ifndef LTE_Function_h__
#define LTE_Function_h__

#include "Type.h"
#include "Parameter.h"
#include "Reference.h"
#include "String.h"

#include "Function_Generated.h"

struct FunctionT : public RefCounted {
  String name;
  String description;
  void (*call)(void**, void*);
  uint paramCount;
  Parameter const* params;
  Type returnType;

  FunctionT() {}

  LT_API ~FunctionT();

  LT_API Data& GetAux(); 

  LT_API String GetSignature() const;

  template <class StreamT>
  friend void _ToStream(StreamT& s, FunctionT const& t) {
    if (t.description)
      s << "/* " << t.description << " */\n";
    s << t.GetSignature();
  }
};

LT_API Function Function_Create(String const& name);

LT_API void Function_AddAlias(String const& source, String const& alias);
LT_API Vector<Function> const& Function_Find(String const& name);
LT_API Vector<Function> const& Function_GetList();

#define DefineConversion(Name, SourceType, DestType)                           \
  LT_API void Name(SourceType const&, DestType&);                              \
                                                                               \
  inline void Name##_Call(TypeT*, void const* in, void* out) {                 \
    Name(*(SourceType const*)in, *(DestType*)out);                             \
  }                                                                            \
                                                                               \
  template <int unused>                                                        \
  Type Name##_Register() {                                                     \
    static Type type;                                                          \
    if (!type) {                                                               \
      type = Type_Get<SourceType>();                                           \
      ConversionType function;                                                 \
      function.other = Type_Get<DestType>();                                   \
      function.fn = Name##_Call;                                               \
      type->AddConversion(function);                                           \
    }                                                                          \
    return type;                                                               \
  }                                                                            \
                                                                               \
  volatile static Type Name##_Registration = Name##_Register<0>();             \
                                                                               \
  inline void Name(SourceType const& src, DestType& dest)

#define FunctionAlias(source, alias)                                           \
  template <int unused>                                                        \
  int __Register_FunctionAlias_##source() {                                    \
    static bool registered = false;                                            \
    if (!registered) {                                                         \
      registered = true;                                                       \
      Function_AddAlias(#source, #alias);                                      \
    }                                                                          \
    return 0;                                                                  \
  }                                                                            \
  volatile static int __FunctionAlias_##source##_Registration = __Register_FunctionAlias_##source<0>()

#endif
