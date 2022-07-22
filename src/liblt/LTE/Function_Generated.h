#define FreeFunction0(Prefix, Postfix, ReturnType, Name, Description, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
  LT_API Prefix ReturnType Name##_Impl() Postfix;                              \
                                                                               \
  inline void Name##_Call(                                                     \
    void** in,                                                                 \
    void* out)                                                                 \
  {                                                                            \
    *(ReturnType*)out =                                                        \
    Name##_Impl();                                                             \
  }                                                                            \
                                                                               \
  template <int unused>                                                        \
  Function Name##_GetMetadata() {                              \
    static Function fn;                                    \
    if (!fn) {                                                                 \
      fn = Function_Create(#Name);                                       \
      fn->call = Name##_Call;                                                  \
      fn->description = Description;                                           \
      fn->paramCount = 0;                                                      \
      fn->params = nullptr;                                                    \
      fn->returnType = Type_Get<ReturnType>();                           \
    }                                                                          \
    return fn;                                                                 \
  }                                                                            \
                                                                               \
  static Function Name##_Metadata = Name##_GetMetadata<0>();   \
  Prefix ReturnType Name##_Impl() Postfix                                      \

#define MemberFunction0(Prefix, Postfix, ReturnType, Name, Description, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
  static void Name##_Call(                                                     \
    void** in,                                                                 \
    void* out)                                                                 \
  {                                                                            \
    *(ReturnType*)out =                                                        \
    ((SelfType*)in[0])->                                                       \
    Name##_Impl();                                                             \
  }                                                                            \
                                                                               \
  template <int unused>                                                        \
  static Function Name##_GetMetadata() {                       \
    static Function fn;                                    \
    if (!fn) {                                                                 \
      fn = Function_Create(#Name);                                       \
      fn->call = Name##_Call;                                                  \
      fn->description = Description;                                           \
      fn->paramCount = 1;                                                      \
      fn->params = new Parameter[1];                                      \
      Mutable(fn->params[0]) = Parameter("object", nullptr);             \
      fn->returnType = Type_Get<ReturnType>();                           \
    }                                                                          \
    return fn;                                                                 \
  }                                                                            \
                                                                               \
  Prefix ReturnType Name##_Impl() Postfix                                      \

#define VoidFreeFunction0(Prefix, Postfix, ReturnType, Name, Description, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
  LT_API Prefix ReturnType Name##_Impl() Postfix;                              \
                                                                               \
  inline void Name##_Call(                                                     \
    void** in,                                                                 \
    void* out)                                                                 \
  {                                                                            \
    Name##_Impl();                                                             \
  }                                                                            \
                                                                               \
  template <int unused>                                                        \
  Function Name##_GetMetadata() {                              \
    static Function fn;                                    \
    if (!fn) {                                                                 \
      fn = Function_Create(#Name);                                       \
      fn->call = Name##_Call;                                                  \
      fn->description = Description;                                           \
      fn->paramCount = 0;                                                      \
      fn->params = nullptr;                                                    \
      fn->returnType = Type_Get<ReturnType>();                           \
    }                                                                          \
    return fn;                                                                 \
  }                                                                            \
                                                                               \
  static Function Name##_Metadata = Name##_GetMetadata<0>();   \
  Prefix ReturnType Name##_Impl() Postfix                                      \

#define VoidMemberFunction0(Prefix, Postfix, ReturnType, Name, Description, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
  static void Name##_Call(                                                     \
    void** in,                                                                 \
    void* out)                                                                 \
  {                                                                            \
    ((SelfType*)in[0])->                                                       \
    Name##_Impl();                                                             \
  }                                                                            \
                                                                               \
  template <int unused>                                                        \
  static Function Name##_GetMetadata() {                       \
    static Function fn;                                    \
    if (!fn) {                                                                 \
      fn = Function_Create(#Name);                                       \
      fn->call = Name##_Call;                                                  \
      fn->description = Description;                                           \
      fn->paramCount = 1;                                                      \
      fn->params = new Parameter[1];                                      \
      Mutable(fn->params[0]) = Parameter("object", nullptr);             \
      fn->returnType = Type_Get<ReturnType>();                           \
    }                                                                          \
    return fn;                                                                 \
  }                                                                            \
                                                                               \
  Prefix ReturnType Name##_Impl() Postfix                                      \

#define FreeFunction1(Prefix, Postfix, ReturnType, Name, Description, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
  LT_API Prefix ReturnType Name##_Impl(T0 const& N0) Postfix;                  \
                                                                               \
  inline void Name##_Call(                                                     \
    void** in,                                                                 \
    void* out)                                                                 \
  {                                                                            \
    *(ReturnType*)out =                                                        \
    Name##_Impl(                                                               \
      *(T0*)in[0]);                                                            \
  }                                                                            \
                                                                               \
  template <int unused>                                                        \
  Function Name##_GetMetadata() {                              \
    static Function fn;                                    \
    if (!fn) {                                                                 \
      fn = Function_Create(#Name);                                       \
      fn->call = Name##_Call;                                                  \
      fn->description = Description;                                           \
      fn->paramCount = 1;                                                      \
      fn->params = new Parameter[1];                                      \
      Mutable(fn->params[0]) = Parameter(#N0, Type_Get<T0>());     \
      fn->returnType = Type_Get<ReturnType>();                           \
    }                                                                          \
    return fn;                                                                 \
  }                                                                            \
                                                                               \
  static Function Name##_Metadata = Name##_GetMetadata<0>();   \
  Prefix ReturnType Name##_Impl(T0 const& N0) Postfix                          \

#define MemberFunction1(Prefix, Postfix, ReturnType, Name, Description, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
  static void Name##_Call(                                                     \
    void** in,                                                                 \
    void* out)                                                                 \
  {                                                                            \
    *(ReturnType*)out =                                                        \
    ((SelfType*)in[0])->                                                       \
    Name##_Impl(                                                               \
      *(T0*)in[1]);                                                            \
  }                                                                            \
                                                                               \
  template <int unused>                                                        \
  static Function Name##_GetMetadata() {                       \
    static Function fn;                                    \
    if (!fn) {                                                                 \
      fn = Function_Create(#Name);                                       \
      fn->call = Name##_Call;                                                  \
      fn->description = Description;                                           \
      fn->paramCount = 2;                                                      \
      fn->params = new Parameter[2];                                      \
      Mutable(fn->params[0]) = Parameter("object", nullptr);             \
      Mutable(fn->params[1]) = Parameter(#N0, Type_Get<T0>());     \
      fn->returnType = Type_Get<ReturnType>();                           \
    }                                                                          \
    return fn;                                                                 \
  }                                                                            \
                                                                               \
  Prefix ReturnType Name##_Impl(T0 const& N0) Postfix                          \

#define VoidFreeFunction1(Prefix, Postfix, ReturnType, Name, Description, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
  LT_API Prefix ReturnType Name##_Impl(T0 const& N0) Postfix;                  \
                                                                               \
  inline void Name##_Call(                                                     \
    void** in,                                                                 \
    void* out)                                                                 \
  {                                                                            \
    Name##_Impl(                                                               \
      *(T0*)in[0]);                                                            \
  }                                                                            \
                                                                               \
  template <int unused>                                                        \
  Function Name##_GetMetadata() {                              \
    static Function fn;                                    \
    if (!fn) {                                                                 \
      fn = Function_Create(#Name);                                       \
      fn->call = Name##_Call;                                                  \
      fn->description = Description;                                           \
      fn->paramCount = 1;                                                      \
      fn->params = new Parameter[1];                                      \
      Mutable(fn->params[0]) = Parameter(#N0, Type_Get<T0>());     \
      fn->returnType = Type_Get<ReturnType>();                           \
    }                                                                          \
    return fn;                                                                 \
  }                                                                            \
                                                                               \
  static Function Name##_Metadata = Name##_GetMetadata<0>();   \
  Prefix ReturnType Name##_Impl(T0 const& N0) Postfix                          \

#define VoidMemberFunction1(Prefix, Postfix, ReturnType, Name, Description, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
  static void Name##_Call(                                                     \
    void** in,                                                                 \
    void* out)                                                                 \
  {                                                                            \
    ((SelfType*)in[0])->                                                       \
    Name##_Impl(                                                               \
      *(T0*)in[1]);                                                            \
  }                                                                            \
                                                                               \
  template <int unused>                                                        \
  static Function Name##_GetMetadata() {                       \
    static Function fn;                                    \
    if (!fn) {                                                                 \
      fn = Function_Create(#Name);                                       \
      fn->call = Name##_Call;                                                  \
      fn->description = Description;                                           \
      fn->paramCount = 2;                                                      \
      fn->params = new Parameter[2];                                      \
      Mutable(fn->params[0]) = Parameter("object", nullptr);             \
      Mutable(fn->params[1]) = Parameter(#N0, Type_Get<T0>());     \
      fn->returnType = Type_Get<ReturnType>();                           \
    }                                                                          \
    return fn;                                                                 \
  }                                                                            \
                                                                               \
  Prefix ReturnType Name##_Impl(T0 const& N0) Postfix                          \

#define FreeFunction2(Prefix, Postfix, ReturnType, Name, Description, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
  LT_API Prefix ReturnType Name##_Impl(                                        \
    T0 const& N0,                                                              \
    T1 const& N1) Postfix;                                                     \
                                                                               \
  inline void Name##_Call(                                                     \
    void** in,                                                                 \
    void* out)                                                                 \
  {                                                                            \
    *(ReturnType*)out =                                                        \
    Name##_Impl(                                                               \
      *(T0*)in[0],                                                             \
      *(T1*)in[1]);                                                            \
  }                                                                            \
                                                                               \
  template <int unused>                                                        \
  Function Name##_GetMetadata() {                              \
    static Function fn;                                    \
    if (!fn) {                                                                 \
      fn = Function_Create(#Name);                                       \
      fn->call = Name##_Call;                                                  \
      fn->description = Description;                                           \
      fn->paramCount = 2;                                                      \
      fn->params = new Parameter[2];                                      \
      Mutable(fn->params[0]) = Parameter(#N0, Type_Get<T0>());     \
      Mutable(fn->params[1]) = Parameter(#N1, Type_Get<T1>());     \
      fn->returnType = Type_Get<ReturnType>();                           \
    }                                                                          \
    return fn;                                                                 \
  }                                                                            \
                                                                               \
  static Function Name##_Metadata = Name##_GetMetadata<0>();   \
  Prefix ReturnType Name##_Impl(                                               \
    T0 const& N0,                                                              \
    T1 const& N1) Postfix                                                      \

#define MemberFunction2(Prefix, Postfix, ReturnType, Name, Description, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
  static void Name##_Call(                                                     \
    void** in,                                                                 \
    void* out)                                                                 \
  {                                                                            \
    *(ReturnType*)out =                                                        \
    ((SelfType*)in[0])->                                                       \
    Name##_Impl(                                                               \
      *(T0*)in[1],                                                             \
      *(T1*)in[2]);                                                            \
  }                                                                            \
                                                                               \
  template <int unused>                                                        \
  static Function Name##_GetMetadata() {                       \
    static Function fn;                                    \
    if (!fn) {                                                                 \
      fn = Function_Create(#Name);                                       \
      fn->call = Name##_Call;                                                  \
      fn->description = Description;                                           \
      fn->paramCount = 3;                                                      \
      fn->params = new Parameter[3];                                      \
      Mutable(fn->params[0]) = Parameter("object", nullptr);             \
      Mutable(fn->params[1]) = Parameter(#N0, Type_Get<T0>());     \
      Mutable(fn->params[2]) = Parameter(#N1, Type_Get<T1>());     \
      fn->returnType = Type_Get<ReturnType>();                           \
    }                                                                          \
    return fn;                                                                 \
  }                                                                            \
                                                                               \
  Prefix ReturnType Name##_Impl(                                               \
    T0 const& N0,                                                              \
    T1 const& N1) Postfix                                                      \

#define VoidFreeFunction2(Prefix, Postfix, ReturnType, Name, Description, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
  LT_API Prefix ReturnType Name##_Impl(                                        \
    T0 const& N0,                                                              \
    T1 const& N1) Postfix;                                                     \
                                                                               \
  inline void Name##_Call(                                                     \
    void** in,                                                                 \
    void* out)                                                                 \
  {                                                                            \
    Name##_Impl(                                                               \
      *(T0*)in[0],                                                             \
      *(T1*)in[1]);                                                            \
  }                                                                            \
                                                                               \
  template <int unused>                                                        \
  Function Name##_GetMetadata() {                              \
    static Function fn;                                    \
    if (!fn) {                                                                 \
      fn = Function_Create(#Name);                                       \
      fn->call = Name##_Call;                                                  \
      fn->description = Description;                                           \
      fn->paramCount = 2;                                                      \
      fn->params = new Parameter[2];                                      \
      Mutable(fn->params[0]) = Parameter(#N0, Type_Get<T0>());     \
      Mutable(fn->params[1]) = Parameter(#N1, Type_Get<T1>());     \
      fn->returnType = Type_Get<ReturnType>();                           \
    }                                                                          \
    return fn;                                                                 \
  }                                                                            \
                                                                               \
  static Function Name##_Metadata = Name##_GetMetadata<0>();   \
  Prefix ReturnType Name##_Impl(                                               \
    T0 const& N0,                                                              \
    T1 const& N1) Postfix                                                      \

#define VoidMemberFunction2(Prefix, Postfix, ReturnType, Name, Description, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
  static void Name##_Call(                                                     \
    void** in,                                                                 \
    void* out)                                                                 \
  {                                                                            \
    ((SelfType*)in[0])->                                                       \
    Name##_Impl(                                                               \
      *(T0*)in[1],                                                             \
      *(T1*)in[2]);                                                            \
  }                                                                            \
                                                                               \
  template <int unused>                                                        \
  static Function Name##_GetMetadata() {                       \
    static Function fn;                                    \
    if (!fn) {                                                                 \
      fn = Function_Create(#Name);                                       \
      fn->call = Name##_Call;                                                  \
      fn->description = Description;                                           \
      fn->paramCount = 3;                                                      \
      fn->params = new Parameter[3];                                      \
      Mutable(fn->params[0]) = Parameter("object", nullptr);             \
      Mutable(fn->params[1]) = Parameter(#N0, Type_Get<T0>());     \
      Mutable(fn->params[2]) = Parameter(#N1, Type_Get<T1>());     \
      fn->returnType = Type_Get<ReturnType>();                           \
    }                                                                          \
    return fn;                                                                 \
  }                                                                            \
                                                                               \
  Prefix ReturnType Name##_Impl(                                               \
    T0 const& N0,                                                              \
    T1 const& N1) Postfix                                                      \

#define FreeFunction3(Prefix, Postfix, ReturnType, Name, Description, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
  LT_API Prefix ReturnType Name##_Impl(                                        \
    T0 const& N0,                                                              \
    T1 const& N1,                                                              \
    T2 const& N2) Postfix;                                                     \
                                                                               \
  inline void Name##_Call(                                                     \
    void** in,                                                                 \
    void* out)                                                                 \
  {                                                                            \
    *(ReturnType*)out =                                                        \
    Name##_Impl(                                                               \
      *(T0*)in[0],                                                             \
      *(T1*)in[1],                                                             \
      *(T2*)in[2]);                                                            \
  }                                                                            \
                                                                               \
  template <int unused>                                                        \
  Function Name##_GetMetadata() {                              \
    static Function fn;                                    \
    if (!fn) {                                                                 \
      fn = Function_Create(#Name);                                       \
      fn->call = Name##_Call;                                                  \
      fn->description = Description;                                           \
      fn->paramCount = 3;                                                      \
      fn->params = new Parameter[3];                                      \
      Mutable(fn->params[0]) = Parameter(#N0, Type_Get<T0>());     \
      Mutable(fn->params[1]) = Parameter(#N1, Type_Get<T1>());     \
      Mutable(fn->params[2]) = Parameter(#N2, Type_Get<T2>());     \
      fn->returnType = Type_Get<ReturnType>();                           \
    }                                                                          \
    return fn;                                                                 \
  }                                                                            \
                                                                               \
  static Function Name##_Metadata = Name##_GetMetadata<0>();   \
  Prefix ReturnType Name##_Impl(                                               \
    T0 const& N0,                                                              \
    T1 const& N1,                                                              \
    T2 const& N2) Postfix                                                      \

#define MemberFunction3(Prefix, Postfix, ReturnType, Name, Description, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
  static void Name##_Call(                                                     \
    void** in,                                                                 \
    void* out)                                                                 \
  {                                                                            \
    *(ReturnType*)out =                                                        \
    ((SelfType*)in[0])->                                                       \
    Name##_Impl(                                                               \
      *(T0*)in[1],                                                             \
      *(T1*)in[2],                                                             \
      *(T2*)in[3]);                                                            \
  }                                                                            \
                                                                               \
  template <int unused>                                                        \
  static Function Name##_GetMetadata() {                       \
    static Function fn;                                    \
    if (!fn) {                                                                 \
      fn = Function_Create(#Name);                                       \
      fn->call = Name##_Call;                                                  \
      fn->description = Description;                                           \
      fn->paramCount = 4;                                                      \
      fn->params = new Parameter[4];                                      \
      Mutable(fn->params[0]) = Parameter("object", nullptr);             \
      Mutable(fn->params[1]) = Parameter(#N0, Type_Get<T0>());     \
      Mutable(fn->params[2]) = Parameter(#N1, Type_Get<T1>());     \
      Mutable(fn->params[3]) = Parameter(#N2, Type_Get<T2>());     \
      fn->returnType = Type_Get<ReturnType>();                           \
    }                                                                          \
    return fn;                                                                 \
  }                                                                            \
                                                                               \
  Prefix ReturnType Name##_Impl(                                               \
    T0 const& N0,                                                              \
    T1 const& N1,                                                              \
    T2 const& N2) Postfix                                                      \

#define VoidFreeFunction3(Prefix, Postfix, ReturnType, Name, Description, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
  LT_API Prefix ReturnType Name##_Impl(                                        \
    T0 const& N0,                                                              \
    T1 const& N1,                                                              \
    T2 const& N2) Postfix;                                                     \
                                                                               \
  inline void Name##_Call(                                                     \
    void** in,                                                                 \
    void* out)                                                                 \
  {                                                                            \
    Name##_Impl(                                                               \
      *(T0*)in[0],                                                             \
      *(T1*)in[1],                                                             \
      *(T2*)in[2]);                                                            \
  }                                                                            \
                                                                               \
  template <int unused>                                                        \
  Function Name##_GetMetadata() {                              \
    static Function fn;                                    \
    if (!fn) {                                                                 \
      fn = Function_Create(#Name);                                       \
      fn->call = Name##_Call;                                                  \
      fn->description = Description;                                           \
      fn->paramCount = 3;                                                      \
      fn->params = new Parameter[3];                                      \
      Mutable(fn->params[0]) = Parameter(#N0, Type_Get<T0>());     \
      Mutable(fn->params[1]) = Parameter(#N1, Type_Get<T1>());     \
      Mutable(fn->params[2]) = Parameter(#N2, Type_Get<T2>());     \
      fn->returnType = Type_Get<ReturnType>();                           \
    }                                                                          \
    return fn;                                                                 \
  }                                                                            \
                                                                               \
  static Function Name##_Metadata = Name##_GetMetadata<0>();   \
  Prefix ReturnType Name##_Impl(                                               \
    T0 const& N0,                                                              \
    T1 const& N1,                                                              \
    T2 const& N2) Postfix                                                      \

#define VoidMemberFunction3(Prefix, Postfix, ReturnType, Name, Description, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
  static void Name##_Call(                                                     \
    void** in,                                                                 \
    void* out)                                                                 \
  {                                                                            \
    ((SelfType*)in[0])->                                                       \
    Name##_Impl(                                                               \
      *(T0*)in[1],                                                             \
      *(T1*)in[2],                                                             \
      *(T2*)in[3]);                                                            \
  }                                                                            \
                                                                               \
  template <int unused>                                                        \
  static Function Name##_GetMetadata() {                       \
    static Function fn;                                    \
    if (!fn) {                                                                 \
      fn = Function_Create(#Name);                                       \
      fn->call = Name##_Call;                                                  \
      fn->description = Description;                                           \
      fn->paramCount = 4;                                                      \
      fn->params = new Parameter[4];                                      \
      Mutable(fn->params[0]) = Parameter("object", nullptr);             \
      Mutable(fn->params[1]) = Parameter(#N0, Type_Get<T0>());     \
      Mutable(fn->params[2]) = Parameter(#N1, Type_Get<T1>());     \
      Mutable(fn->params[3]) = Parameter(#N2, Type_Get<T2>());     \
      fn->returnType = Type_Get<ReturnType>();                           \
    }                                                                          \
    return fn;                                                                 \
  }                                                                            \
                                                                               \
  Prefix ReturnType Name##_Impl(                                               \
    T0 const& N0,                                                              \
    T1 const& N1,                                                              \
    T2 const& N2) Postfix                                                      \

#define FreeFunction4(Prefix, Postfix, ReturnType, Name, Description, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
  LT_API Prefix ReturnType Name##_Impl(                                        \
    T0 const& N0,                                                              \
    T1 const& N1,                                                              \
    T2 const& N2,                                                              \
    T3 const& N3) Postfix;                                                     \
                                                                               \
  inline void Name##_Call(                                                     \
    void** in,                                                                 \
    void* out)                                                                 \
  {                                                                            \
    *(ReturnType*)out =                                                        \
    Name##_Impl(                                                               \
      *(T0*)in[0],                                                             \
      *(T1*)in[1],                                                             \
      *(T2*)in[2],                                                             \
      *(T3*)in[3]);                                                            \
  }                                                                            \
                                                                               \
  template <int unused>                                                        \
  Function Name##_GetMetadata() {                              \
    static Function fn;                                    \
    if (!fn) {                                                                 \
      fn = Function_Create(#Name);                                       \
      fn->call = Name##_Call;                                                  \
      fn->description = Description;                                           \
      fn->paramCount = 4;                                                      \
      fn->params = new Parameter[4];                                      \
      Mutable(fn->params[0]) = Parameter(#N0, Type_Get<T0>());     \
      Mutable(fn->params[1]) = Parameter(#N1, Type_Get<T1>());     \
      Mutable(fn->params[2]) = Parameter(#N2, Type_Get<T2>());     \
      Mutable(fn->params[3]) = Parameter(#N3, Type_Get<T3>());     \
      fn->returnType = Type_Get<ReturnType>();                           \
    }                                                                          \
    return fn;                                                                 \
  }                                                                            \
                                                                               \
  static Function Name##_Metadata = Name##_GetMetadata<0>();   \
  Prefix ReturnType Name##_Impl(                                               \
    T0 const& N0,                                                              \
    T1 const& N1,                                                              \
    T2 const& N2,                                                              \
    T3 const& N3) Postfix                                                      \

#define MemberFunction4(Prefix, Postfix, ReturnType, Name, Description, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
  static void Name##_Call(                                                     \
    void** in,                                                                 \
    void* out)                                                                 \
  {                                                                            \
    *(ReturnType*)out =                                                        \
    ((SelfType*)in[0])->                                                       \
    Name##_Impl(                                                               \
      *(T0*)in[1],                                                             \
      *(T1*)in[2],                                                             \
      *(T2*)in[3],                                                             \
      *(T3*)in[4]);                                                            \
  }                                                                            \
                                                                               \
  template <int unused>                                                        \
  static Function Name##_GetMetadata() {                       \
    static Function fn;                                    \
    if (!fn) {                                                                 \
      fn = Function_Create(#Name);                                       \
      fn->call = Name##_Call;                                                  \
      fn->description = Description;                                           \
      fn->paramCount = 5;                                                      \
      fn->params = new Parameter[5];                                      \
      Mutable(fn->params[0]) = Parameter("object", nullptr);             \
      Mutable(fn->params[1]) = Parameter(#N0, Type_Get<T0>());     \
      Mutable(fn->params[2]) = Parameter(#N1, Type_Get<T1>());     \
      Mutable(fn->params[3]) = Parameter(#N2, Type_Get<T2>());     \
      Mutable(fn->params[4]) = Parameter(#N3, Type_Get<T3>());     \
      fn->returnType = Type_Get<ReturnType>();                           \
    }                                                                          \
    return fn;                                                                 \
  }                                                                            \
                                                                               \
  Prefix ReturnType Name##_Impl(                                               \
    T0 const& N0,                                                              \
    T1 const& N1,                                                              \
    T2 const& N2,                                                              \
    T3 const& N3) Postfix                                                      \

#define VoidFreeFunction4(Prefix, Postfix, ReturnType, Name, Description, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
  LT_API Prefix ReturnType Name##_Impl(                                        \
    T0 const& N0,                                                              \
    T1 const& N1,                                                              \
    T2 const& N2,                                                              \
    T3 const& N3) Postfix;                                                     \
                                                                               \
  inline void Name##_Call(                                                     \
    void** in,                                                                 \
    void* out)                                                                 \
  {                                                                            \
    Name##_Impl(                                                               \
      *(T0*)in[0],                                                             \
      *(T1*)in[1],                                                             \
      *(T2*)in[2],                                                             \
      *(T3*)in[3]);                                                            \
  }                                                                            \
                                                                               \
  template <int unused>                                                        \
  Function Name##_GetMetadata() {                              \
    static Function fn;                                    \
    if (!fn) {                                                                 \
      fn = Function_Create(#Name);                                       \
      fn->call = Name##_Call;                                                  \
      fn->description = Description;                                           \
      fn->paramCount = 4;                                                      \
      fn->params = new Parameter[4];                                      \
      Mutable(fn->params[0]) = Parameter(#N0, Type_Get<T0>());     \
      Mutable(fn->params[1]) = Parameter(#N1, Type_Get<T1>());     \
      Mutable(fn->params[2]) = Parameter(#N2, Type_Get<T2>());     \
      Mutable(fn->params[3]) = Parameter(#N3, Type_Get<T3>());     \
      fn->returnType = Type_Get<ReturnType>();                           \
    }                                                                          \
    return fn;                                                                 \
  }                                                                            \
                                                                               \
  static Function Name##_Metadata = Name##_GetMetadata<0>();   \
  Prefix ReturnType Name##_Impl(                                               \
    T0 const& N0,                                                              \
    T1 const& N1,                                                              \
    T2 const& N2,                                                              \
    T3 const& N3) Postfix                                                      \

#define VoidMemberFunction4(Prefix, Postfix, ReturnType, Name, Description, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
  static void Name##_Call(                                                     \
    void** in,                                                                 \
    void* out)                                                                 \
  {                                                                            \
    ((SelfType*)in[0])->                                                       \
    Name##_Impl(                                                               \
      *(T0*)in[1],                                                             \
      *(T1*)in[2],                                                             \
      *(T2*)in[3],                                                             \
      *(T3*)in[4]);                                                            \
  }                                                                            \
                                                                               \
  template <int unused>                                                        \
  static Function Name##_GetMetadata() {                       \
    static Function fn;                                    \
    if (!fn) {                                                                 \
      fn = Function_Create(#Name);                                       \
      fn->call = Name##_Call;                                                  \
      fn->description = Description;                                           \
      fn->paramCount = 5;                                                      \
      fn->params = new Parameter[5];                                      \
      Mutable(fn->params[0]) = Parameter("object", nullptr);             \
      Mutable(fn->params[1]) = Parameter(#N0, Type_Get<T0>());     \
      Mutable(fn->params[2]) = Parameter(#N1, Type_Get<T1>());     \
      Mutable(fn->params[3]) = Parameter(#N2, Type_Get<T2>());     \
      Mutable(fn->params[4]) = Parameter(#N3, Type_Get<T3>());     \
      fn->returnType = Type_Get<ReturnType>();                           \
    }                                                                          \
    return fn;                                                                 \
  }                                                                            \
                                                                               \
  Prefix ReturnType Name##_Impl(                                               \
    T0 const& N0,                                                              \
    T1 const& N1,                                                              \
    T2 const& N2,                                                              \
    T3 const& N3) Postfix                                                      \

#define FreeFunction5(Prefix, Postfix, ReturnType, Name, Description, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
  LT_API Prefix ReturnType Name##_Impl(                                        \
    T0 const& N0,                                                              \
    T1 const& N1,                                                              \
    T2 const& N2,                                                              \
    T3 const& N3,                                                              \
    T4 const& N4) Postfix;                                                     \
                                                                               \
  inline void Name##_Call(                                                     \
    void** in,                                                                 \
    void* out)                                                                 \
  {                                                                            \
    *(ReturnType*)out =                                                        \
    Name##_Impl(                                                               \
      *(T0*)in[0],                                                             \
      *(T1*)in[1],                                                             \
      *(T2*)in[2],                                                             \
      *(T3*)in[3],                                                             \
      *(T4*)in[4]);                                                            \
  }                                                                            \
                                                                               \
  template <int unused>                                                        \
  Function Name##_GetMetadata() {                              \
    static Function fn;                                    \
    if (!fn) {                                                                 \
      fn = Function_Create(#Name);                                       \
      fn->call = Name##_Call;                                                  \
      fn->description = Description;                                           \
      fn->paramCount = 5;                                                      \
      fn->params = new Parameter[5];                                      \
      Mutable(fn->params[0]) = Parameter(#N0, Type_Get<T0>());     \
      Mutable(fn->params[1]) = Parameter(#N1, Type_Get<T1>());     \
      Mutable(fn->params[2]) = Parameter(#N2, Type_Get<T2>());     \
      Mutable(fn->params[3]) = Parameter(#N3, Type_Get<T3>());     \
      Mutable(fn->params[4]) = Parameter(#N4, Type_Get<T4>());     \
      fn->returnType = Type_Get<ReturnType>();                           \
    }                                                                          \
    return fn;                                                                 \
  }                                                                            \
                                                                               \
  static Function Name##_Metadata = Name##_GetMetadata<0>();   \
  Prefix ReturnType Name##_Impl(                                               \
    T0 const& N0,                                                              \
    T1 const& N1,                                                              \
    T2 const& N2,                                                              \
    T3 const& N3,                                                              \
    T4 const& N4) Postfix                                                      \

#define MemberFunction5(Prefix, Postfix, ReturnType, Name, Description, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
  static void Name##_Call(                                                     \
    void** in,                                                                 \
    void* out)                                                                 \
  {                                                                            \
    *(ReturnType*)out =                                                        \
    ((SelfType*)in[0])->                                                       \
    Name##_Impl(                                                               \
      *(T0*)in[1],                                                             \
      *(T1*)in[2],                                                             \
      *(T2*)in[3],                                                             \
      *(T3*)in[4],                                                             \
      *(T4*)in[5]);                                                            \
  }                                                                            \
                                                                               \
  template <int unused>                                                        \
  static Function Name##_GetMetadata() {                       \
    static Function fn;                                    \
    if (!fn) {                                                                 \
      fn = Function_Create(#Name);                                       \
      fn->call = Name##_Call;                                                  \
      fn->description = Description;                                           \
      fn->paramCount = 6;                                                      \
      fn->params = new Parameter[6];                                      \
      Mutable(fn->params[0]) = Parameter("object", nullptr);             \
      Mutable(fn->params[1]) = Parameter(#N0, Type_Get<T0>());     \
      Mutable(fn->params[2]) = Parameter(#N1, Type_Get<T1>());     \
      Mutable(fn->params[3]) = Parameter(#N2, Type_Get<T2>());     \
      Mutable(fn->params[4]) = Parameter(#N3, Type_Get<T3>());     \
      Mutable(fn->params[5]) = Parameter(#N4, Type_Get<T4>());     \
      fn->returnType = Type_Get<ReturnType>();                           \
    }                                                                          \
    return fn;                                                                 \
  }                                                                            \
                                                                               \
  Prefix ReturnType Name##_Impl(                                               \
    T0 const& N0,                                                              \
    T1 const& N1,                                                              \
    T2 const& N2,                                                              \
    T3 const& N3,                                                              \
    T4 const& N4) Postfix                                                      \

#define VoidFreeFunction5(Prefix, Postfix, ReturnType, Name, Description, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
  LT_API Prefix ReturnType Name##_Impl(                                        \
    T0 const& N0,                                                              \
    T1 const& N1,                                                              \
    T2 const& N2,                                                              \
    T3 const& N3,                                                              \
    T4 const& N4) Postfix;                                                     \
                                                                               \
  inline void Name##_Call(                                                     \
    void** in,                                                                 \
    void* out)                                                                 \
  {                                                                            \
    Name##_Impl(                                                               \
      *(T0*)in[0],                                                             \
      *(T1*)in[1],                                                             \
      *(T2*)in[2],                                                             \
      *(T3*)in[3],                                                             \
      *(T4*)in[4]);                                                            \
  }                                                                            \
                                                                               \
  template <int unused>                                                        \
  Function Name##_GetMetadata() {                              \
    static Function fn;                                    \
    if (!fn) {                                                                 \
      fn = Function_Create(#Name);                                       \
      fn->call = Name##_Call;                                                  \
      fn->description = Description;                                           \
      fn->paramCount = 5;                                                      \
      fn->params = new Parameter[5];                                      \
      Mutable(fn->params[0]) = Parameter(#N0, Type_Get<T0>());     \
      Mutable(fn->params[1]) = Parameter(#N1, Type_Get<T1>());     \
      Mutable(fn->params[2]) = Parameter(#N2, Type_Get<T2>());     \
      Mutable(fn->params[3]) = Parameter(#N3, Type_Get<T3>());     \
      Mutable(fn->params[4]) = Parameter(#N4, Type_Get<T4>());     \
      fn->returnType = Type_Get<ReturnType>();                           \
    }                                                                          \
    return fn;                                                                 \
  }                                                                            \
                                                                               \
  static Function Name##_Metadata = Name##_GetMetadata<0>();   \
  Prefix ReturnType Name##_Impl(                                               \
    T0 const& N0,                                                              \
    T1 const& N1,                                                              \
    T2 const& N2,                                                              \
    T3 const& N3,                                                              \
    T4 const& N4) Postfix                                                      \

#define VoidMemberFunction5(Prefix, Postfix, ReturnType, Name, Description, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
  static void Name##_Call(                                                     \
    void** in,                                                                 \
    void* out)                                                                 \
  {                                                                            \
    ((SelfType*)in[0])->                                                       \
    Name##_Impl(                                                               \
      *(T0*)in[1],                                                             \
      *(T1*)in[2],                                                             \
      *(T2*)in[3],                                                             \
      *(T3*)in[4],                                                             \
      *(T4*)in[5]);                                                            \
  }                                                                            \
                                                                               \
  template <int unused>                                                        \
  static Function Name##_GetMetadata() {                       \
    static Function fn;                                    \
    if (!fn) {                                                                 \
      fn = Function_Create(#Name);                                       \
      fn->call = Name##_Call;                                                  \
      fn->description = Description;                                           \
      fn->paramCount = 6;                                                      \
      fn->params = new Parameter[6];                                      \
      Mutable(fn->params[0]) = Parameter("object", nullptr);             \
      Mutable(fn->params[1]) = Parameter(#N0, Type_Get<T0>());     \
      Mutable(fn->params[2]) = Parameter(#N1, Type_Get<T1>());     \
      Mutable(fn->params[3]) = Parameter(#N2, Type_Get<T2>());     \
      Mutable(fn->params[4]) = Parameter(#N3, Type_Get<T3>());     \
      Mutable(fn->params[5]) = Parameter(#N4, Type_Get<T4>());     \
      fn->returnType = Type_Get<ReturnType>();                           \
    }                                                                          \
    return fn;                                                                 \
  }                                                                            \
                                                                               \
  Prefix ReturnType Name##_Impl(                                               \
    T0 const& N0,                                                              \
    T1 const& N1,                                                              \
    T2 const& N2,                                                              \
    T3 const& N3,                                                              \
    T4 const& N4) Postfix                                                      \

#define FreeFunction6(Prefix, Postfix, ReturnType, Name, Description, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
  LT_API Prefix ReturnType Name##_Impl(                                        \
    T0 const& N0,                                                              \
    T1 const& N1,                                                              \
    T2 const& N2,                                                              \
    T3 const& N3,                                                              \
    T4 const& N4,                                                              \
    T5 const& N5) Postfix;                                                     \
                                                                               \
  inline void Name##_Call(                                                     \
    void** in,                                                                 \
    void* out)                                                                 \
  {                                                                            \
    *(ReturnType*)out =                                                        \
    Name##_Impl(                                                               \
      *(T0*)in[0],                                                             \
      *(T1*)in[1],                                                             \
      *(T2*)in[2],                                                             \
      *(T3*)in[3],                                                             \
      *(T4*)in[4],                                                             \
      *(T5*)in[5]);                                                            \
  }                                                                            \
                                                                               \
  template <int unused>                                                        \
  Function Name##_GetMetadata() {                              \
    static Function fn;                                    \
    if (!fn) {                                                                 \
      fn = Function_Create(#Name);                                       \
      fn->call = Name##_Call;                                                  \
      fn->description = Description;                                           \
      fn->paramCount = 6;                                                      \
      fn->params = new Parameter[6];                                      \
      Mutable(fn->params[0]) = Parameter(#N0, Type_Get<T0>());     \
      Mutable(fn->params[1]) = Parameter(#N1, Type_Get<T1>());     \
      Mutable(fn->params[2]) = Parameter(#N2, Type_Get<T2>());     \
      Mutable(fn->params[3]) = Parameter(#N3, Type_Get<T3>());     \
      Mutable(fn->params[4]) = Parameter(#N4, Type_Get<T4>());     \
      Mutable(fn->params[5]) = Parameter(#N5, Type_Get<T5>());     \
      fn->returnType = Type_Get<ReturnType>();                           \
    }                                                                          \
    return fn;                                                                 \
  }                                                                            \
                                                                               \
  static Function Name##_Metadata = Name##_GetMetadata<0>();   \
  Prefix ReturnType Name##_Impl(                                               \
    T0 const& N0,                                                              \
    T1 const& N1,                                                              \
    T2 const& N2,                                                              \
    T3 const& N3,                                                              \
    T4 const& N4,                                                              \
    T5 const& N5) Postfix                                                      \

#define MemberFunction6(Prefix, Postfix, ReturnType, Name, Description, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
  static void Name##_Call(                                                     \
    void** in,                                                                 \
    void* out)                                                                 \
  {                                                                            \
    *(ReturnType*)out =                                                        \
    ((SelfType*)in[0])->                                                       \
    Name##_Impl(                                                               \
      *(T0*)in[1],                                                             \
      *(T1*)in[2],                                                             \
      *(T2*)in[3],                                                             \
      *(T3*)in[4],                                                             \
      *(T4*)in[5],                                                             \
      *(T5*)in[6]);                                                            \
  }                                                                            \
                                                                               \
  template <int unused>                                                        \
  static Function Name##_GetMetadata() {                       \
    static Function fn;                                    \
    if (!fn) {                                                                 \
      fn = Function_Create(#Name);                                       \
      fn->call = Name##_Call;                                                  \
      fn->description = Description;                                           \
      fn->paramCount = 7;                                                      \
      fn->params = new Parameter[7];                                      \
      Mutable(fn->params[0]) = Parameter("object", nullptr);             \
      Mutable(fn->params[1]) = Parameter(#N0, Type_Get<T0>());     \
      Mutable(fn->params[2]) = Parameter(#N1, Type_Get<T1>());     \
      Mutable(fn->params[3]) = Parameter(#N2, Type_Get<T2>());     \
      Mutable(fn->params[4]) = Parameter(#N3, Type_Get<T3>());     \
      Mutable(fn->params[5]) = Parameter(#N4, Type_Get<T4>());     \
      Mutable(fn->params[6]) = Parameter(#N5, Type_Get<T5>());     \
      fn->returnType = Type_Get<ReturnType>();                           \
    }                                                                          \
    return fn;                                                                 \
  }                                                                            \
                                                                               \
  Prefix ReturnType Name##_Impl(                                               \
    T0 const& N0,                                                              \
    T1 const& N1,                                                              \
    T2 const& N2,                                                              \
    T3 const& N3,                                                              \
    T4 const& N4,                                                              \
    T5 const& N5) Postfix                                                      \

#define VoidFreeFunction6(Prefix, Postfix, ReturnType, Name, Description, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
  LT_API Prefix ReturnType Name##_Impl(                                        \
    T0 const& N0,                                                              \
    T1 const& N1,                                                              \
    T2 const& N2,                                                              \
    T3 const& N3,                                                              \
    T4 const& N4,                                                              \
    T5 const& N5) Postfix;                                                     \
                                                                               \
  inline void Name##_Call(                                                     \
    void** in,                                                                 \
    void* out)                                                                 \
  {                                                                            \
    Name##_Impl(                                                               \
      *(T0*)in[0],                                                             \
      *(T1*)in[1],                                                             \
      *(T2*)in[2],                                                             \
      *(T3*)in[3],                                                             \
      *(T4*)in[4],                                                             \
      *(T5*)in[5]);                                                            \
  }                                                                            \
                                                                               \
  template <int unused>                                                        \
  Function Name##_GetMetadata() {                              \
    static Function fn;                                    \
    if (!fn) {                                                                 \
      fn = Function_Create(#Name);                                       \
      fn->call = Name##_Call;                                                  \
      fn->description = Description;                                           \
      fn->paramCount = 6;                                                      \
      fn->params = new Parameter[6];                                      \
      Mutable(fn->params[0]) = Parameter(#N0, Type_Get<T0>());     \
      Mutable(fn->params[1]) = Parameter(#N1, Type_Get<T1>());     \
      Mutable(fn->params[2]) = Parameter(#N2, Type_Get<T2>());     \
      Mutable(fn->params[3]) = Parameter(#N3, Type_Get<T3>());     \
      Mutable(fn->params[4]) = Parameter(#N4, Type_Get<T4>());     \
      Mutable(fn->params[5]) = Parameter(#N5, Type_Get<T5>());     \
      fn->returnType = Type_Get<ReturnType>();                           \
    }                                                                          \
    return fn;                                                                 \
  }                                                                            \
                                                                               \
  static Function Name##_Metadata = Name##_GetMetadata<0>();   \
  Prefix ReturnType Name##_Impl(                                               \
    T0 const& N0,                                                              \
    T1 const& N1,                                                              \
    T2 const& N2,                                                              \
    T3 const& N3,                                                              \
    T4 const& N4,                                                              \
    T5 const& N5) Postfix                                                      \

#define VoidMemberFunction6(Prefix, Postfix, ReturnType, Name, Description, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
  static void Name##_Call(                                                     \
    void** in,                                                                 \
    void* out)                                                                 \
  {                                                                            \
    ((SelfType*)in[0])->                                                       \
    Name##_Impl(                                                               \
      *(T0*)in[1],                                                             \
      *(T1*)in[2],                                                             \
      *(T2*)in[3],                                                             \
      *(T3*)in[4],                                                             \
      *(T4*)in[5],                                                             \
      *(T5*)in[6]);                                                            \
  }                                                                            \
                                                                               \
  template <int unused>                                                        \
  static Function Name##_GetMetadata() {                       \
    static Function fn;                                    \
    if (!fn) {                                                                 \
      fn = Function_Create(#Name);                                       \
      fn->call = Name##_Call;                                                  \
      fn->description = Description;                                           \
      fn->paramCount = 7;                                                      \
      fn->params = new Parameter[7];                                      \
      Mutable(fn->params[0]) = Parameter("object", nullptr);             \
      Mutable(fn->params[1]) = Parameter(#N0, Type_Get<T0>());     \
      Mutable(fn->params[2]) = Parameter(#N1, Type_Get<T1>());     \
      Mutable(fn->params[3]) = Parameter(#N2, Type_Get<T2>());     \
      Mutable(fn->params[4]) = Parameter(#N3, Type_Get<T3>());     \
      Mutable(fn->params[5]) = Parameter(#N4, Type_Get<T4>());     \
      Mutable(fn->params[6]) = Parameter(#N5, Type_Get<T5>());     \
      fn->returnType = Type_Get<ReturnType>();                           \
    }                                                                          \
    return fn;                                                                 \
  }                                                                            \
                                                                               \
  Prefix ReturnType Name##_Impl(                                               \
    T0 const& N0,                                                              \
    T1 const& N1,                                                              \
    T2 const& N2,                                                              \
    T3 const& N3,                                                              \
    T4 const& N4,                                                              \
    T5 const& N5) Postfix                                                      \

#define FreeFunction7(Prefix, Postfix, ReturnType, Name, Description, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
  LT_API Prefix ReturnType Name##_Impl(                                        \
    T0 const& N0,                                                              \
    T1 const& N1,                                                              \
    T2 const& N2,                                                              \
    T3 const& N3,                                                              \
    T4 const& N4,                                                              \
    T5 const& N5,                                                              \
    T6 const& N6) Postfix;                                                     \
                                                                               \
  inline void Name##_Call(                                                     \
    void** in,                                                                 \
    void* out)                                                                 \
  {                                                                            \
    *(ReturnType*)out =                                                        \
    Name##_Impl(                                                               \
      *(T0*)in[0],                                                             \
      *(T1*)in[1],                                                             \
      *(T2*)in[2],                                                             \
      *(T3*)in[3],                                                             \
      *(T4*)in[4],                                                             \
      *(T5*)in[5],                                                             \
      *(T6*)in[6]);                                                            \
  }                                                                            \
                                                                               \
  template <int unused>                                                        \
  Function Name##_GetMetadata() {                              \
    static Function fn;                                    \
    if (!fn) {                                                                 \
      fn = Function_Create(#Name);                                       \
      fn->call = Name##_Call;                                                  \
      fn->description = Description;                                           \
      fn->paramCount = 7;                                                      \
      fn->params = new Parameter[7];                                      \
      Mutable(fn->params[0]) = Parameter(#N0, Type_Get<T0>());     \
      Mutable(fn->params[1]) = Parameter(#N1, Type_Get<T1>());     \
      Mutable(fn->params[2]) = Parameter(#N2, Type_Get<T2>());     \
      Mutable(fn->params[3]) = Parameter(#N3, Type_Get<T3>());     \
      Mutable(fn->params[4]) = Parameter(#N4, Type_Get<T4>());     \
      Mutable(fn->params[5]) = Parameter(#N5, Type_Get<T5>());     \
      Mutable(fn->params[6]) = Parameter(#N6, Type_Get<T6>());     \
      fn->returnType = Type_Get<ReturnType>();                           \
    }                                                                          \
    return fn;                                                                 \
  }                                                                            \
                                                                               \
  static Function Name##_Metadata = Name##_GetMetadata<0>();   \
  Prefix ReturnType Name##_Impl(                                               \
    T0 const& N0,                                                              \
    T1 const& N1,                                                              \
    T2 const& N2,                                                              \
    T3 const& N3,                                                              \
    T4 const& N4,                                                              \
    T5 const& N5,                                                              \
    T6 const& N6) Postfix                                                      \

#define MemberFunction7(Prefix, Postfix, ReturnType, Name, Description, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
  static void Name##_Call(                                                     \
    void** in,                                                                 \
    void* out)                                                                 \
  {                                                                            \
    *(ReturnType*)out =                                                        \
    ((SelfType*)in[0])->                                                       \
    Name##_Impl(                                                               \
      *(T0*)in[1],                                                             \
      *(T1*)in[2],                                                             \
      *(T2*)in[3],                                                             \
      *(T3*)in[4],                                                             \
      *(T4*)in[5],                                                             \
      *(T5*)in[6],                                                             \
      *(T6*)in[7]);                                                            \
  }                                                                            \
                                                                               \
  template <int unused>                                                        \
  static Function Name##_GetMetadata() {                       \
    static Function fn;                                    \
    if (!fn) {                                                                 \
      fn = Function_Create(#Name);                                       \
      fn->call = Name##_Call;                                                  \
      fn->description = Description;                                           \
      fn->paramCount = 8;                                                      \
      fn->params = new Parameter[8];                                      \
      Mutable(fn->params[0]) = Parameter("object", nullptr);             \
      Mutable(fn->params[1]) = Parameter(#N0, Type_Get<T0>());     \
      Mutable(fn->params[2]) = Parameter(#N1, Type_Get<T1>());     \
      Mutable(fn->params[3]) = Parameter(#N2, Type_Get<T2>());     \
      Mutable(fn->params[4]) = Parameter(#N3, Type_Get<T3>());     \
      Mutable(fn->params[5]) = Parameter(#N4, Type_Get<T4>());     \
      Mutable(fn->params[6]) = Parameter(#N5, Type_Get<T5>());     \
      Mutable(fn->params[7]) = Parameter(#N6, Type_Get<T6>());     \
      fn->returnType = Type_Get<ReturnType>();                           \
    }                                                                          \
    return fn;                                                                 \
  }                                                                            \
                                                                               \
  Prefix ReturnType Name##_Impl(                                               \
    T0 const& N0,                                                              \
    T1 const& N1,                                                              \
    T2 const& N2,                                                              \
    T3 const& N3,                                                              \
    T4 const& N4,                                                              \
    T5 const& N5,                                                              \
    T6 const& N6) Postfix                                                      \

#define VoidFreeFunction7(Prefix, Postfix, ReturnType, Name, Description, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
  LT_API Prefix ReturnType Name##_Impl(                                        \
    T0 const& N0,                                                              \
    T1 const& N1,                                                              \
    T2 const& N2,                                                              \
    T3 const& N3,                                                              \
    T4 const& N4,                                                              \
    T5 const& N5,                                                              \
    T6 const& N6) Postfix;                                                     \
                                                                               \
  inline void Name##_Call(                                                     \
    void** in,                                                                 \
    void* out)                                                                 \
  {                                                                            \
    Name##_Impl(                                                               \
      *(T0*)in[0],                                                             \
      *(T1*)in[1],                                                             \
      *(T2*)in[2],                                                             \
      *(T3*)in[3],                                                             \
      *(T4*)in[4],                                                             \
      *(T5*)in[5],                                                             \
      *(T6*)in[6]);                                                            \
  }                                                                            \
                                                                               \
  template <int unused>                                                        \
  Function Name##_GetMetadata() {                              \
    static Function fn;                                    \
    if (!fn) {                                                                 \
      fn = Function_Create(#Name);                                       \
      fn->call = Name##_Call;                                                  \
      fn->description = Description;                                           \
      fn->paramCount = 7;                                                      \
      fn->params = new Parameter[7];                                      \
      Mutable(fn->params[0]) = Parameter(#N0, Type_Get<T0>());     \
      Mutable(fn->params[1]) = Parameter(#N1, Type_Get<T1>());     \
      Mutable(fn->params[2]) = Parameter(#N2, Type_Get<T2>());     \
      Mutable(fn->params[3]) = Parameter(#N3, Type_Get<T3>());     \
      Mutable(fn->params[4]) = Parameter(#N4, Type_Get<T4>());     \
      Mutable(fn->params[5]) = Parameter(#N5, Type_Get<T5>());     \
      Mutable(fn->params[6]) = Parameter(#N6, Type_Get<T6>());     \
      fn->returnType = Type_Get<ReturnType>();                           \
    }                                                                          \
    return fn;                                                                 \
  }                                                                            \
                                                                               \
  static Function Name##_Metadata = Name##_GetMetadata<0>();   \
  Prefix ReturnType Name##_Impl(                                               \
    T0 const& N0,                                                              \
    T1 const& N1,                                                              \
    T2 const& N2,                                                              \
    T3 const& N3,                                                              \
    T4 const& N4,                                                              \
    T5 const& N5,                                                              \
    T6 const& N6) Postfix                                                      \

#define VoidMemberFunction7(Prefix, Postfix, ReturnType, Name, Description, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
  static void Name##_Call(                                                     \
    void** in,                                                                 \
    void* out)                                                                 \
  {                                                                            \
    ((SelfType*)in[0])->                                                       \
    Name##_Impl(                                                               \
      *(T0*)in[1],                                                             \
      *(T1*)in[2],                                                             \
      *(T2*)in[3],                                                             \
      *(T3*)in[4],                                                             \
      *(T4*)in[5],                                                             \
      *(T5*)in[6],                                                             \
      *(T6*)in[7]);                                                            \
  }                                                                            \
                                                                               \
  template <int unused>                                                        \
  static Function Name##_GetMetadata() {                       \
    static Function fn;                                    \
    if (!fn) {                                                                 \
      fn = Function_Create(#Name);                                       \
      fn->call = Name##_Call;                                                  \
      fn->description = Description;                                           \
      fn->paramCount = 8;                                                      \
      fn->params = new Parameter[8];                                      \
      Mutable(fn->params[0]) = Parameter("object", nullptr);             \
      Mutable(fn->params[1]) = Parameter(#N0, Type_Get<T0>());     \
      Mutable(fn->params[2]) = Parameter(#N1, Type_Get<T1>());     \
      Mutable(fn->params[3]) = Parameter(#N2, Type_Get<T2>());     \
      Mutable(fn->params[4]) = Parameter(#N3, Type_Get<T3>());     \
      Mutable(fn->params[5]) = Parameter(#N4, Type_Get<T4>());     \
      Mutable(fn->params[6]) = Parameter(#N5, Type_Get<T5>());     \
      Mutable(fn->params[7]) = Parameter(#N6, Type_Get<T6>());     \
      fn->returnType = Type_Get<ReturnType>();                           \
    }                                                                          \
    return fn;                                                                 \
  }                                                                            \
                                                                               \
  Prefix ReturnType Name##_Impl(                                               \
    T0 const& N0,                                                              \
    T1 const& N1,                                                              \
    T2 const& N2,                                                              \
    T3 const& N3,                                                              \
    T4 const& N4,                                                              \
    T5 const& N5,                                                              \
    T6 const& N6) Postfix                                                      \

#define FreeFunction8(Prefix, Postfix, ReturnType, Name, Description, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
  LT_API Prefix ReturnType Name##_Impl(                                        \
    T0 const& N0,                                                              \
    T1 const& N1,                                                              \
    T2 const& N2,                                                              \
    T3 const& N3,                                                              \
    T4 const& N4,                                                              \
    T5 const& N5,                                                              \
    T6 const& N6,                                                              \
    T7 const& N7) Postfix;                                                     \
                                                                               \
  inline void Name##_Call(                                                     \
    void** in,                                                                 \
    void* out)                                                                 \
  {                                                                            \
    *(ReturnType*)out =                                                        \
    Name##_Impl(                                                               \
      *(T0*)in[0],                                                             \
      *(T1*)in[1],                                                             \
      *(T2*)in[2],                                                             \
      *(T3*)in[3],                                                             \
      *(T4*)in[4],                                                             \
      *(T5*)in[5],                                                             \
      *(T6*)in[6],                                                             \
      *(T7*)in[7]);                                                            \
  }                                                                            \
                                                                               \
  template <int unused>                                                        \
  Function Name##_GetMetadata() {                              \
    static Function fn;                                    \
    if (!fn) {                                                                 \
      fn = Function_Create(#Name);                                       \
      fn->call = Name##_Call;                                                  \
      fn->description = Description;                                           \
      fn->paramCount = 8;                                                      \
      fn->params = new Parameter[8];                                      \
      Mutable(fn->params[0]) = Parameter(#N0, Type_Get<T0>());     \
      Mutable(fn->params[1]) = Parameter(#N1, Type_Get<T1>());     \
      Mutable(fn->params[2]) = Parameter(#N2, Type_Get<T2>());     \
      Mutable(fn->params[3]) = Parameter(#N3, Type_Get<T3>());     \
      Mutable(fn->params[4]) = Parameter(#N4, Type_Get<T4>());     \
      Mutable(fn->params[5]) = Parameter(#N5, Type_Get<T5>());     \
      Mutable(fn->params[6]) = Parameter(#N6, Type_Get<T6>());     \
      Mutable(fn->params[7]) = Parameter(#N7, Type_Get<T7>());     \
      fn->returnType = Type_Get<ReturnType>();                           \
    }                                                                          \
    return fn;                                                                 \
  }                                                                            \
                                                                               \
  static Function Name##_Metadata = Name##_GetMetadata<0>();   \
  Prefix ReturnType Name##_Impl(                                               \
    T0 const& N0,                                                              \
    T1 const& N1,                                                              \
    T2 const& N2,                                                              \
    T3 const& N3,                                                              \
    T4 const& N4,                                                              \
    T5 const& N5,                                                              \
    T6 const& N6,                                                              \
    T7 const& N7) Postfix                                                      \

#define MemberFunction8(Prefix, Postfix, ReturnType, Name, Description, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
  static void Name##_Call(                                                     \
    void** in,                                                                 \
    void* out)                                                                 \
  {                                                                            \
    *(ReturnType*)out =                                                        \
    ((SelfType*)in[0])->                                                       \
    Name##_Impl(                                                               \
      *(T0*)in[1],                                                             \
      *(T1*)in[2],                                                             \
      *(T2*)in[3],                                                             \
      *(T3*)in[4],                                                             \
      *(T4*)in[5],                                                             \
      *(T5*)in[6],                                                             \
      *(T6*)in[7],                                                             \
      *(T7*)in[8]);                                                            \
  }                                                                            \
                                                                               \
  template <int unused>                                                        \
  static Function Name##_GetMetadata() {                       \
    static Function fn;                                    \
    if (!fn) {                                                                 \
      fn = Function_Create(#Name);                                       \
      fn->call = Name##_Call;                                                  \
      fn->description = Description;                                           \
      fn->paramCount = 9;                                                      \
      fn->params = new Parameter[9];                                      \
      Mutable(fn->params[0]) = Parameter("object", nullptr);             \
      Mutable(fn->params[1]) = Parameter(#N0, Type_Get<T0>());     \
      Mutable(fn->params[2]) = Parameter(#N1, Type_Get<T1>());     \
      Mutable(fn->params[3]) = Parameter(#N2, Type_Get<T2>());     \
      Mutable(fn->params[4]) = Parameter(#N3, Type_Get<T3>());     \
      Mutable(fn->params[5]) = Parameter(#N4, Type_Get<T4>());     \
      Mutable(fn->params[6]) = Parameter(#N5, Type_Get<T5>());     \
      Mutable(fn->params[7]) = Parameter(#N6, Type_Get<T6>());     \
      Mutable(fn->params[8]) = Parameter(#N7, Type_Get<T7>());     \
      fn->returnType = Type_Get<ReturnType>();                           \
    }                                                                          \
    return fn;                                                                 \
  }                                                                            \
                                                                               \
  Prefix ReturnType Name##_Impl(                                               \
    T0 const& N0,                                                              \
    T1 const& N1,                                                              \
    T2 const& N2,                                                              \
    T3 const& N3,                                                              \
    T4 const& N4,                                                              \
    T5 const& N5,                                                              \
    T6 const& N6,                                                              \
    T7 const& N7) Postfix                                                      \

#define VoidFreeFunction8(Prefix, Postfix, ReturnType, Name, Description, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
  LT_API Prefix ReturnType Name##_Impl(                                        \
    T0 const& N0,                                                              \
    T1 const& N1,                                                              \
    T2 const& N2,                                                              \
    T3 const& N3,                                                              \
    T4 const& N4,                                                              \
    T5 const& N5,                                                              \
    T6 const& N6,                                                              \
    T7 const& N7) Postfix;                                                     \
                                                                               \
  inline void Name##_Call(                                                     \
    void** in,                                                                 \
    void* out)                                                                 \
  {                                                                            \
    Name##_Impl(                                                               \
      *(T0*)in[0],                                                             \
      *(T1*)in[1],                                                             \
      *(T2*)in[2],                                                             \
      *(T3*)in[3],                                                             \
      *(T4*)in[4],                                                             \
      *(T5*)in[5],                                                             \
      *(T6*)in[6],                                                             \
      *(T7*)in[7]);                                                            \
  }                                                                            \
                                                                               \
  template <int unused>                                                        \
  Function Name##_GetMetadata() {                              \
    static Function fn;                                    \
    if (!fn) {                                                                 \
      fn = Function_Create(#Name);                                       \
      fn->call = Name##_Call;                                                  \
      fn->description = Description;                                           \
      fn->paramCount = 8;                                                      \
      fn->params = new Parameter[8];                                      \
      Mutable(fn->params[0]) = Parameter(#N0, Type_Get<T0>());     \
      Mutable(fn->params[1]) = Parameter(#N1, Type_Get<T1>());     \
      Mutable(fn->params[2]) = Parameter(#N2, Type_Get<T2>());     \
      Mutable(fn->params[3]) = Parameter(#N3, Type_Get<T3>());     \
      Mutable(fn->params[4]) = Parameter(#N4, Type_Get<T4>());     \
      Mutable(fn->params[5]) = Parameter(#N5, Type_Get<T5>());     \
      Mutable(fn->params[6]) = Parameter(#N6, Type_Get<T6>());     \
      Mutable(fn->params[7]) = Parameter(#N7, Type_Get<T7>());     \
      fn->returnType = Type_Get<ReturnType>();                           \
    }                                                                          \
    return fn;                                                                 \
  }                                                                            \
                                                                               \
  static Function Name##_Metadata = Name##_GetMetadata<0>();   \
  Prefix ReturnType Name##_Impl(                                               \
    T0 const& N0,                                                              \
    T1 const& N1,                                                              \
    T2 const& N2,                                                              \
    T3 const& N3,                                                              \
    T4 const& N4,                                                              \
    T5 const& N5,                                                              \
    T6 const& N6,                                                              \
    T7 const& N7) Postfix                                                      \

#define VoidMemberFunction8(Prefix, Postfix, ReturnType, Name, Description, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
  static void Name##_Call(                                                     \
    void** in,                                                                 \
    void* out)                                                                 \
  {                                                                            \
    ((SelfType*)in[0])->                                                       \
    Name##_Impl(                                                               \
      *(T0*)in[1],                                                             \
      *(T1*)in[2],                                                             \
      *(T2*)in[3],                                                             \
      *(T3*)in[4],                                                             \
      *(T4*)in[5],                                                             \
      *(T5*)in[6],                                                             \
      *(T6*)in[7],                                                             \
      *(T7*)in[8]);                                                            \
  }                                                                            \
                                                                               \
  template <int unused>                                                        \
  static Function Name##_GetMetadata() {                       \
    static Function fn;                                    \
    if (!fn) {                                                                 \
      fn = Function_Create(#Name);                                       \
      fn->call = Name##_Call;                                                  \
      fn->description = Description;                                           \
      fn->paramCount = 9;                                                      \
      fn->params = new Parameter[9];                                      \
      Mutable(fn->params[0]) = Parameter("object", nullptr);             \
      Mutable(fn->params[1]) = Parameter(#N0, Type_Get<T0>());     \
      Mutable(fn->params[2]) = Parameter(#N1, Type_Get<T1>());     \
      Mutable(fn->params[3]) = Parameter(#N2, Type_Get<T2>());     \
      Mutable(fn->params[4]) = Parameter(#N3, Type_Get<T3>());     \
      Mutable(fn->params[5]) = Parameter(#N4, Type_Get<T4>());     \
      Mutable(fn->params[6]) = Parameter(#N5, Type_Get<T5>());     \
      Mutable(fn->params[7]) = Parameter(#N6, Type_Get<T6>());     \
      Mutable(fn->params[8]) = Parameter(#N7, Type_Get<T7>());     \
      fn->returnType = Type_Get<ReturnType>();                           \
    }                                                                          \
    return fn;                                                                 \
  }                                                                            \
                                                                               \
  Prefix ReturnType Name##_Impl(                                               \
    T0 const& N0,                                                              \
    T1 const& N1,                                                              \
    T2 const& N2,                                                              \
    T3 const& N3,                                                              \
    T4 const& N4,                                                              \
    T5 const& N5,                                                              \
    T6 const& N6,                                                              \
    T7 const& N7) Postfix                                                      \

#define _FreeFunction(RT, Name, Desc, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, x, ...) \
  MACRO_IDENTITY(FreeFunction##x(,,RT,Name,Desc,T0,N0,T1,N1,T2,N2,T3,N3,T4,N4,T5,N5,T6,N6,T7,N7))\

#define FreeFunction(RT, Name, Desc, ...) \
  MACRO_IDENTITY(_FreeFunction(RT,Name,Desc,__VA_ARGS__,8,8,7,7,6,6,5,5,4,4,3,3,2,2,1,1,0,0))\

#define _VoidFreeFunction(RT, Name, Desc, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, x, ...) \
  MACRO_IDENTITY(VoidFreeFunction##x(,,RT,Name,Desc,T0,N0,T1,N1,T2,N2,T3,N3,T4,N4,T5,N5,T6,N6,T7,N7))\

#define VoidFreeFunction(Name, Desc, ...) \
  MACRO_IDENTITY(_VoidFreeFunction(void ,Name,Desc,__VA_ARGS__,8,8,7,7,6,6,5,5,4,4,3,3,2,2,1,1,0,0))\

#define _MemberFunction(RT, Name, Desc, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, x, ...) \
  MACRO_IDENTITY(MemberFunction##x(,,RT,Name,Desc,T0,N0,T1,N1,T2,N2,T3,N3,T4,N4,T5,N5,T6,N6,T7,N7))\

#define MemberFunction(RT, Name, Desc, ...) \
  MACRO_IDENTITY(_MemberFunction(RT,Name,Desc,__VA_ARGS__,8,8,7,7,6,6,5,5,4,4,3,3,2,2,1,1,0,0))\

#define _VoidMemberFunction(RT, Name, Desc, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, x, ...) \
  MACRO_IDENTITY(VoidMemberFunction##x(,,RT,Name,Desc,T0,N0,T1,N1,T2,N2,T3,N3,T4,N4,T5,N5,T6,N6,T7,N7))\

#define VoidMemberFunction(Name, Desc, ...) \
  MACRO_IDENTITY(_VoidMemberFunction(void ,Name,Desc,__VA_ARGS__,8,8,7,7,6,6,5,5,4,4,3,3,2,2,1,1,0,0))\

#define _ConstMemberFunction(RT, Name, Desc, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, x, ...) \
  MACRO_IDENTITY(MemberFunction##x(,const,RT,Name,Desc,T0,N0,T1,N1,T2,N2,T3,N3,T4,N4,T5,N5,T6,N6,T7,N7))\

#define ConstMemberFunction(RT, Name, Desc, ...) \
  MACRO_IDENTITY(_ConstMemberFunction(RT,Name,Desc,__VA_ARGS__,8,8,7,7,6,6,5,5,4,4,3,3,2,2,1,1,0,0))\

#define _ConstVoidMemberFunction(RT, Name, Desc, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, x, ...) \
  MACRO_IDENTITY(VoidMemberFunction##x(,const,RT,Name,Desc,T0,N0,T1,N1,T2,N2,T3,N3,T4,N4,T5,N5,T6,N6,T7,N7))\

#define ConstVoidMemberFunction(Name, Desc, ...) \
  MACRO_IDENTITY(_ConstVoidMemberFunction(void ,Name,Desc,__VA_ARGS__,8,8,7,7,6,6,5,5,4,4,3,3,2,2,1,1,0,0))\

#define _VirtualMemberFunction(RT, Name, Desc, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, x, ...) \
  MACRO_IDENTITY(MemberFunction##x(virtual,,RT,Name,Desc,T0,N0,T1,N1,T2,N2,T3,N3,T4,N4,T5,N5,T6,N6,T7,N7))\

#define VirtualMemberFunction(RT, Name, Desc, ...) \
  MACRO_IDENTITY(_VirtualMemberFunction(RT,Name,Desc,__VA_ARGS__,8,8,7,7,6,6,5,5,4,4,3,3,2,2,1,1,0,0))\

#define _VirtualVoidMemberFunction(RT, Name, Desc, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, x, ...) \
  MACRO_IDENTITY(VoidMemberFunction##x(virtual,,RT,Name,Desc,T0,N0,T1,N1,T2,N2,T3,N3,T4,N4,T5,N5,T6,N6,T7,N7))\

#define VirtualVoidMemberFunction(Name, Desc, ...) \
  MACRO_IDENTITY(_VirtualVoidMemberFunction(void ,Name,Desc,__VA_ARGS__,8,8,7,7,6,6,5,5,4,4,3,3,2,2,1,1,0,0))\

#define _ConstVirtualMemberFunction(RT, Name, Desc, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, x, ...) \
  MACRO_IDENTITY(MemberFunction##x(virtual,const,RT,Name,Desc,T0,N0,T1,N1,T2,N2,T3,N3,T4,N4,T5,N5,T6,N6,T7,N7))\

#define ConstVirtualMemberFunction(RT, Name, Desc, ...) \
  MACRO_IDENTITY(_ConstVirtualMemberFunction(RT,Name,Desc,__VA_ARGS__,8,8,7,7,6,6,5,5,4,4,3,3,2,2,1,1,0,0))\

#define _ConstVirtualVoidMemberFunction(RT, Name, Desc, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, x, ...) \
  MACRO_IDENTITY(VoidMemberFunction##x(virtual,const,RT,Name,Desc,T0,N0,T1,N1,T2,N2,T3,N3,T4,N4,T5,N5,T6,N6,T7,N7))\

#define ConstVirtualVoidMemberFunction(Name, Desc, ...) \
  MACRO_IDENTITY(_ConstVirtualVoidMemberFunction(void ,Name,Desc,__VA_ARGS__,8,8,7,7,6,6,5,5,4,4,3,3,2,2,1,1,0,0))\

#define _FreeFunctionNoParams(RT, Name, Desc, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, x, ...) \
  MACRO_IDENTITY(FreeFunction##x(,,RT,Name,Desc,T0,N0,T1,N1,T2,N2,T3,N3,T4,N4,T5,N5,T6,N6,T7,N7))\

#define FreeFunctionNoParams(RT, Name, Desc) \
  MACRO_IDENTITY(_FreeFunctionNoParams(RT,Name,Desc,8,8,7,7,6,6,5,5,4,4,3,3,2,2,1,1,0,0))\

#define _VoidFreeFunctionNoParams(RT, Name, Desc, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, x, ...) \
  MACRO_IDENTITY(VoidFreeFunction##x(,,RT,Name,Desc,T0,N0,T1,N1,T2,N2,T3,N3,T4,N4,T5,N5,T6,N6,T7,N7))\

#define VoidFreeFunctionNoParams(Name, Desc) \
  MACRO_IDENTITY(_VoidFreeFunctionNoParams(void ,Name,Desc,8,8,7,7,6,6,5,5,4,4,3,3,2,2,1,1,0,0))\

#define _MemberFunctionNoParams(RT, Name, Desc, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, x, ...) \
  MACRO_IDENTITY(MemberFunction##x(,,RT,Name,Desc,T0,N0,T1,N1,T2,N2,T3,N3,T4,N4,T5,N5,T6,N6,T7,N7))\

#define MemberFunctionNoParams(RT, Name, Desc) \
  MACRO_IDENTITY(_MemberFunctionNoParams(RT,Name,Desc,8,8,7,7,6,6,5,5,4,4,3,3,2,2,1,1,0,0))\

#define _VoidMemberFunctionNoParams(RT, Name, Desc, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, x, ...) \
  MACRO_IDENTITY(VoidMemberFunction##x(,,RT,Name,Desc,T0,N0,T1,N1,T2,N2,T3,N3,T4,N4,T5,N5,T6,N6,T7,N7))\

#define VoidMemberFunctionNoParams(Name, Desc) \
  MACRO_IDENTITY(_VoidMemberFunctionNoParams(void ,Name,Desc,8,8,7,7,6,6,5,5,4,4,3,3,2,2,1,1,0,0))\

#define _ConstMemberFunctionNoParams(RT, Name, Desc, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, x, ...) \
  MACRO_IDENTITY(MemberFunction##x(,const,RT,Name,Desc,T0,N0,T1,N1,T2,N2,T3,N3,T4,N4,T5,N5,T6,N6,T7,N7))\

#define ConstMemberFunctionNoParams(RT, Name, Desc) \
  MACRO_IDENTITY(_ConstMemberFunctionNoParams(RT,Name,Desc,8,8,7,7,6,6,5,5,4,4,3,3,2,2,1,1,0,0))\

#define _ConstVoidMemberFunctionNoParams(RT, Name, Desc, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, x, ...) \
  MACRO_IDENTITY(VoidMemberFunction##x(,const,RT,Name,Desc,T0,N0,T1,N1,T2,N2,T3,N3,T4,N4,T5,N5,T6,N6,T7,N7))\

#define ConstVoidMemberFunctionNoParams(Name, Desc) \
  MACRO_IDENTITY(_ConstVoidMemberFunctionNoParams(void ,Name,Desc,8,8,7,7,6,6,5,5,4,4,3,3,2,2,1,1,0,0))\

#define _VirtualMemberFunctionNoParams(RT, Name, Desc, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, x, ...) \
  MACRO_IDENTITY(MemberFunction##x(virtual,,RT,Name,Desc,T0,N0,T1,N1,T2,N2,T3,N3,T4,N4,T5,N5,T6,N6,T7,N7))\

#define VirtualMemberFunctionNoParams(RT, Name, Desc) \
  MACRO_IDENTITY(_VirtualMemberFunctionNoParams(RT,Name,Desc,8,8,7,7,6,6,5,5,4,4,3,3,2,2,1,1,0,0))\

#define _VirtualVoidMemberFunctionNoParams(RT, Name, Desc, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, x, ...) \
  MACRO_IDENTITY(VoidMemberFunction##x(virtual,,RT,Name,Desc,T0,N0,T1,N1,T2,N2,T3,N3,T4,N4,T5,N5,T6,N6,T7,N7))\

#define VirtualVoidMemberFunctionNoParams(Name, Desc) \
  MACRO_IDENTITY(_VirtualVoidMemberFunctionNoParams(void ,Name,Desc,8,8,7,7,6,6,5,5,4,4,3,3,2,2,1,1,0,0))\

#define _ConstVirtualMemberFunctionNoParams(RT, Name, Desc, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, x, ...) \
  MACRO_IDENTITY(MemberFunction##x(virtual,const,RT,Name,Desc,T0,N0,T1,N1,T2,N2,T3,N3,T4,N4,T5,N5,T6,N6,T7,N7))\

#define ConstVirtualMemberFunctionNoParams(RT, Name, Desc) \
  MACRO_IDENTITY(_ConstVirtualMemberFunctionNoParams(RT,Name,Desc,8,8,7,7,6,6,5,5,4,4,3,3,2,2,1,1,0,0))\

#define _ConstVirtualVoidMemberFunctionNoParams(RT, Name, Desc, T0, N0, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, x, ...) \
  MACRO_IDENTITY(VoidMemberFunction##x(virtual,const,RT,Name,Desc,T0,N0,T1,N1,T2,N2,T3,N3,T4,N4,T5,N5,T6,N6,T7,N7))\

#define ConstVirtualVoidMemberFunctionNoParams(Name, Desc) \
  MACRO_IDENTITY(_ConstVirtualVoidMemberFunctionNoParams(void ,Name,Desc,8,8,7,7,6,6,5,5,4,4,3,3,2,2,1,1,0,0))\

