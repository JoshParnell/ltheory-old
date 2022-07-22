#define AUTOCLASS2(name, base, T0, n0, T1, n1, T2, n2, T3, n3, T4, n4, T5, n5, T6, n6, T7, n7, T8, n8, T9, n9, T10, n10, T11, n11, T12, n12, T13, n13, T14, n14, T15, n15) \
  struct name : public base {                                                  \
    typedef name SelfType;                                                     \
    typedef base BaseType;                                                     \
    DefineMetadataInline(name)                                                 \
    T0 n0;                                                                     \
                                                                               \
    name(                                                                      \
        T0 const& n0) :                                                        \
      n0(n0)                                                                   \
    {                                                                          \
      Initializer(0);                                                          \
    }                                                                          \
                                                                               \
    void Initializer(void* ) {                                                 \
                                                                               \
    }                                                                          \
                                                                               \
    T0 Get##n0() const {                                                       \
      return n0;                                                               \
    }                                                                          \
                                                                               \
    static void MapFields(                                                     \
      TypeT* type,                                                             \
      void* b,                                                                 \
      FieldMapper& m,                                                          \
      void* aux)                                                               \
    {                                                                          \
      SelfType* self = (SelfType*)b;                                           \
      static Type type0 = Type_Get(self->n0);                                  \
      m(&self->n0, #n0, type0, aux);                                           \
      base::MapFields(type, b, m, aux);                                        \
    }                                                                          \
                                                                               \
    template <class StreamT>                                                   \
    friend void _ToStream(                                                     \
      StreamT& s,                                                              \
      name const& t)                                                           \
    {                                                                          \
      ToStream(s, #name);                                                      \
      ToStream(s, "(");                                                        \
      ToStream(s, t.n0);                                                       \
      ToStream(s, ")");                                                        \
    }                                                                          \
                                                                               \
  /* AUTOCLASS2. */

#define AUTOCLASS4(name, base, T0, n0, T1, n1, T2, n2, T3, n3, T4, n4, T5, n5, T6, n6, T7, n7, T8, n8, T9, n9, T10, n10, T11, n11, T12, n12, T13, n13, T14, n14, T15, n15) \
  struct name : public base {                                                  \
    typedef name SelfType;                                                     \
    typedef base BaseType;                                                     \
    DefineMetadataInline(name)                                                 \
    T0 n0;                                                                     \
    T1 n1;                                                                     \
                                                                               \
    name(                                                                      \
        T0 const& n0,                                                          \
        T1 const& n1) :                                                        \
      n0(n0),                                                                  \
      n1(n1)                                                                   \
    {                                                                          \
      Initializer(0);                                                          \
    }                                                                          \
                                                                               \
    void Initializer(void* ) {                                                 \
                                                                               \
    }                                                                          \
                                                                               \
    T0 Get##n0() const {                                                       \
      return n0;                                                               \
    }                                                                          \
                                                                               \
    T1 Get##n1() const {                                                       \
      return n1;                                                               \
    }                                                                          \
                                                                               \
    static void MapFields(                                                     \
      TypeT* type,                                                              \
      void* b,                                                                 \
      FieldMapper& m,                                                          \
      void* aux)                                                               \
    {                                                                          \
      SelfType* self = (SelfType*)b;                                           \
      static Type type0 = Type_Get(self->n0);                                 \
      static Type type1 = Type_Get(self->n1);                                 \
      m(&self->n0, #n0, type0, aux);                                           \
      m(&self->n1, #n1, type1, aux);                                           \
      base::MapFields(type, b, m, aux);                                        \
    }                                                                          \
                                                                               \
    template <class StreamT>                                                   \
    friend void _ToStream(                                                     \
      StreamT& s,                                                              \
      name const& t)                                                           \
    {                                                                          \
      ToStream(s, #name);                                                      \
      ToStream(s, "(");                                                        \
      ToStream(s, t.n0);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n1);                                                       \
      ToStream(s, ")");                                                        \
    }                                                                          \
                                                                               \
  /* AUTOCLASS4. */

#define AUTOCLASS6(name, base, T0, n0, T1, n1, T2, n2, T3, n3, T4, n4, T5, n5, T6, n6, T7, n7, T8, n8, T9, n9, T10, n10, T11, n11, T12, n12, T13, n13, T14, n14, T15, n15) \
  struct name : public base {                                                  \
    typedef name SelfType;                                                     \
    typedef base BaseType;                                                     \
    DefineMetadataInline(name)                                                 \
    T0 n0;                                                                     \
    T1 n1;                                                                     \
    T2 n2;                                                                     \
                                                                               \
    name(                                                                      \
        T0 const& n0,                                                          \
        T1 const& n1,                                                          \
        T2 const& n2) :                                                        \
      n0(n0),                                                                  \
      n1(n1),                                                                  \
      n2(n2)                                                                   \
    {                                                                          \
      Initializer(0);                                                          \
    }                                                                          \
                                                                               \
    void Initializer(void* ) {                                                 \
                                                                               \
    }                                                                          \
                                                                               \
    T0 Get##n0() const {                                                       \
      return n0;                                                               \
    }                                                                          \
                                                                               \
    T1 Get##n1() const {                                                       \
      return n1;                                                               \
    }                                                                          \
                                                                               \
    T2 Get##n2() const {                                                       \
      return n2;                                                               \
    }                                                                          \
                                                                               \
    static void MapFields(                                                     \
      TypeT* type,                                                              \
      void* b,                                                                 \
      FieldMapper& m,                                                          \
      void* aux)                                                               \
    {                                                                          \
      SelfType* self = (SelfType*)b;                                           \
      static Type type0 = Type_Get(self->n0);                                 \
      static Type type1 = Type_Get(self->n1);                                 \
      static Type type2 = Type_Get(self->n2);                                 \
      m(&self->n0, #n0, type0, aux);                                           \
      m(&self->n1, #n1, type1, aux);                                           \
      m(&self->n2, #n2, type2, aux);                                           \
      base::MapFields(type, b, m, aux);                                        \
    }                                                                          \
                                                                               \
    template <class StreamT>                                                   \
    friend void _ToStream(                                                     \
      StreamT& s,                                                              \
      name const& t)                                                           \
    {                                                                          \
      ToStream(s, #name);                                                      \
      ToStream(s, "(");                                                        \
      ToStream(s, t.n0);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n1);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n2);                                                       \
      ToStream(s, ")");                                                        \
    }                                                                          \
                                                                               \
  /* AUTOCLASS6. */

#define AUTOCLASS8(name, base, T0, n0, T1, n1, T2, n2, T3, n3, T4, n4, T5, n5, T6, n6, T7, n7, T8, n8, T9, n9, T10, n10, T11, n11, T12, n12, T13, n13, T14, n14, T15, n15) \
  struct name : public base {                                                  \
    typedef name SelfType;                                                     \
    typedef base BaseType;                                                     \
    DefineMetadataInline(name)                                                 \
    T0 n0;                                                                     \
    T1 n1;                                                                     \
    T2 n2;                                                                     \
    T3 n3;                                                                     \
                                                                               \
    name(                                                                      \
        T0 const& n0,                                                          \
        T1 const& n1,                                                          \
        T2 const& n2,                                                          \
        T3 const& n3) :                                                        \
      n0(n0),                                                                  \
      n1(n1),                                                                  \
      n2(n2),                                                                  \
      n3(n3)                                                                   \
    {                                                                          \
      Initializer(0);                                                          \
    }                                                                          \
                                                                               \
    void Initializer(void* ) {                                                 \
                                                                               \
    }                                                                          \
                                                                               \
    T0 Get##n0() const {                                                       \
      return n0;                                                               \
    }                                                                          \
                                                                               \
    T1 Get##n1() const {                                                       \
      return n1;                                                               \
    }                                                                          \
                                                                               \
    T2 Get##n2() const {                                                       \
      return n2;                                                               \
    }                                                                          \
                                                                               \
    T3 Get##n3() const {                                                       \
      return n3;                                                               \
    }                                                                          \
                                                                               \
    static void MapFields(                                                     \
      TypeT* type,                                                              \
      void* b,                                                                 \
      FieldMapper& m,                                                          \
      void* aux)                                                               \
    {                                                                          \
      SelfType* self = (SelfType*)b;                                           \
      static Type type0 = Type_Get(self->n0);                                 \
      static Type type1 = Type_Get(self->n1);                                 \
      static Type type2 = Type_Get(self->n2);                                 \
      static Type type3 = Type_Get(self->n3);                                 \
      m(&self->n0, #n0, type0, aux);                                           \
      m(&self->n1, #n1, type1, aux);                                           \
      m(&self->n2, #n2, type2, aux);                                           \
      m(&self->n3, #n3, type3, aux);                                           \
      base::MapFields(type, b, m, aux);                                        \
    }                                                                          \
                                                                               \
    template <class StreamT>                                                   \
    friend void _ToStream(                                                     \
      StreamT& s,                                                              \
      name const& t)                                                           \
    {                                                                          \
      ToStream(s, #name);                                                      \
      ToStream(s, "(");                                                        \
      ToStream(s, t.n0);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n1);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n2);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n3);                                                       \
      ToStream(s, ")");                                                        \
    }                                                                          \
                                                                               \
  /* AUTOCLASS8. */

#define AUTOCLASS10(name, base, T0, n0, T1, n1, T2, n2, T3, n3, T4, n4, T5, n5, T6, n6, T7, n7, T8, n8, T9, n9, T10, n10, T11, n11, T12, n12, T13, n13, T14, n14, T15, n15) \
  struct name : public base {                                                  \
    typedef name SelfType;                                                     \
    typedef base BaseType;                                                     \
    DefineMetadataInline(name)                                                 \
    T0 n0;                                                                     \
    T1 n1;                                                                     \
    T2 n2;                                                                     \
    T3 n3;                                                                     \
    T4 n4;                                                                     \
                                                                               \
    name(                                                                      \
        T0 const& n0,                                                          \
        T1 const& n1,                                                          \
        T2 const& n2,                                                          \
        T3 const& n3,                                                          \
        T4 const& n4) :                                                        \
      n0(n0),                                                                  \
      n1(n1),                                                                  \
      n2(n2),                                                                  \
      n3(n3),                                                                  \
      n4(n4)                                                                   \
    {                                                                          \
      Initializer(0);                                                          \
    }                                                                          \
                                                                               \
    void Initializer(void* ) {                                                 \
                                                                               \
    }                                                                          \
                                                                               \
    T0 Get##n0() const {                                                       \
      return n0;                                                               \
    }                                                                          \
                                                                               \
    T1 Get##n1() const {                                                       \
      return n1;                                                               \
    }                                                                          \
                                                                               \
    T2 Get##n2() const {                                                       \
      return n2;                                                               \
    }                                                                          \
                                                                               \
    T3 Get##n3() const {                                                       \
      return n3;                                                               \
    }                                                                          \
                                                                               \
    T4 Get##n4() const {                                                       \
      return n4;                                                               \
    }                                                                          \
                                                                               \
    static void MapFields(                                                     \
      TypeT* type,                                                              \
      void* b,                                                                 \
      FieldMapper& m,                                                          \
      void* aux)                                                               \
    {                                                                          \
      SelfType* self = (SelfType*)b;                                           \
      static Type type0 = Type_Get(self->n0);                                 \
      static Type type1 = Type_Get(self->n1);                                 \
      static Type type2 = Type_Get(self->n2);                                 \
      static Type type3 = Type_Get(self->n3);                                 \
      static Type type4 = Type_Get(self->n4);                                 \
      m(&self->n0, #n0, type0, aux);                                           \
      m(&self->n1, #n1, type1, aux);                                           \
      m(&self->n2, #n2, type2, aux);                                           \
      m(&self->n3, #n3, type3, aux);                                           \
      m(&self->n4, #n4, type4, aux);                                           \
      base::MapFields(type, b, m, aux);                                        \
    }                                                                          \
                                                                               \
    template <class StreamT>                                                   \
    friend void _ToStream(                                                     \
      StreamT& s,                                                              \
      name const& t)                                                           \
    {                                                                          \
      ToStream(s, #name);                                                      \
      ToStream(s, "(");                                                        \
      ToStream(s, t.n0);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n1);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n2);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n3);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n4);                                                       \
      ToStream(s, ")");                                                        \
    }                                                                          \
                                                                               \
  /* AUTOCLASS10. */

#define AUTOCLASS12(name, base, T0, n0, T1, n1, T2, n2, T3, n3, T4, n4, T5, n5, T6, n6, T7, n7, T8, n8, T9, n9, T10, n10, T11, n11, T12, n12, T13, n13, T14, n14, T15, n15) \
  struct name : public base {                                                  \
    typedef name SelfType;                                                     \
    typedef base BaseType;                                                     \
    DefineMetadataInline(name)                                                 \
    T0 n0;                                                                     \
    T1 n1;                                                                     \
    T2 n2;                                                                     \
    T3 n3;                                                                     \
    T4 n4;                                                                     \
    T5 n5;                                                                     \
                                                                               \
    name(                                                                      \
        T0 const& n0,                                                          \
        T1 const& n1,                                                          \
        T2 const& n2,                                                          \
        T3 const& n3,                                                          \
        T4 const& n4,                                                          \
        T5 const& n5) :                                                        \
      n0(n0),                                                                  \
      n1(n1),                                                                  \
      n2(n2),                                                                  \
      n3(n3),                                                                  \
      n4(n4),                                                                  \
      n5(n5)                                                                   \
    {                                                                          \
      Initializer(0);                                                          \
    }                                                                          \
                                                                               \
    void Initializer(void* ) {                                                 \
                                                                               \
    }                                                                          \
                                                                               \
    T0 Get##n0() const {                                                       \
      return n0;                                                               \
    }                                                                          \
                                                                               \
    T1 Get##n1() const {                                                       \
      return n1;                                                               \
    }                                                                          \
                                                                               \
    T2 Get##n2() const {                                                       \
      return n2;                                                               \
    }                                                                          \
                                                                               \
    T3 Get##n3() const {                                                       \
      return n3;                                                               \
    }                                                                          \
                                                                               \
    T4 Get##n4() const {                                                       \
      return n4;                                                               \
    }                                                                          \
                                                                               \
    T5 Get##n5() const {                                                       \
      return n5;                                                               \
    }                                                                          \
                                                                               \
    static void MapFields(                                                     \
      TypeT* type,                                                              \
      void* b,                                                                 \
      FieldMapper& m,                                                          \
      void* aux)                                                               \
    {                                                                          \
      SelfType* self = (SelfType*)b;                                           \
      static Type type0 = Type_Get(self->n0);                                 \
      static Type type1 = Type_Get(self->n1);                                 \
      static Type type2 = Type_Get(self->n2);                                 \
      static Type type3 = Type_Get(self->n3);                                 \
      static Type type4 = Type_Get(self->n4);                                 \
      static Type type5 = Type_Get(self->n5);                                 \
      m(&self->n0, #n0, type0, aux);                                           \
      m(&self->n1, #n1, type1, aux);                                           \
      m(&self->n2, #n2, type2, aux);                                           \
      m(&self->n3, #n3, type3, aux);                                           \
      m(&self->n4, #n4, type4, aux);                                           \
      m(&self->n5, #n5, type5, aux);                                           \
      base::MapFields(type, b, m, aux);                                        \
    }                                                                          \
                                                                               \
    template <class StreamT>                                                   \
    friend void _ToStream(                                                     \
      StreamT& s,                                                              \
      name const& t)                                                           \
    {                                                                          \
      ToStream(s, #name);                                                      \
      ToStream(s, "(");                                                        \
      ToStream(s, t.n0);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n1);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n2);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n3);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n4);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n5);                                                       \
      ToStream(s, ")");                                                        \
    }                                                                          \
                                                                               \
  /* AUTOCLASS12. */

#define AUTOCLASS14(name, base, T0, n0, T1, n1, T2, n2, T3, n3, T4, n4, T5, n5, T6, n6, T7, n7, T8, n8, T9, n9, T10, n10, T11, n11, T12, n12, T13, n13, T14, n14, T15, n15) \
  struct name : public base {                                                  \
    typedef name SelfType;                                                     \
    typedef base BaseType;                                                     \
    DefineMetadataInline(name)                                                 \
    T0 n0;                                                                     \
    T1 n1;                                                                     \
    T2 n2;                                                                     \
    T3 n3;                                                                     \
    T4 n4;                                                                     \
    T5 n5;                                                                     \
    T6 n6;                                                                     \
                                                                               \
    name(                                                                      \
        T0 const& n0,                                                          \
        T1 const& n1,                                                          \
        T2 const& n2,                                                          \
        T3 const& n3,                                                          \
        T4 const& n4,                                                          \
        T5 const& n5,                                                          \
        T6 const& n6) :                                                        \
      n0(n0),                                                                  \
      n1(n1),                                                                  \
      n2(n2),                                                                  \
      n3(n3),                                                                  \
      n4(n4),                                                                  \
      n5(n5),                                                                  \
      n6(n6)                                                                   \
    {                                                                          \
      Initializer(0);                                                          \
    }                                                                          \
                                                                               \
    void Initializer(void* ) {                                                 \
                                                                               \
    }                                                                          \
                                                                               \
    T0 Get##n0() const {                                                       \
      return n0;                                                               \
    }                                                                          \
                                                                               \
    T1 Get##n1() const {                                                       \
      return n1;                                                               \
    }                                                                          \
                                                                               \
    T2 Get##n2() const {                                                       \
      return n2;                                                               \
    }                                                                          \
                                                                               \
    T3 Get##n3() const {                                                       \
      return n3;                                                               \
    }                                                                          \
                                                                               \
    T4 Get##n4() const {                                                       \
      return n4;                                                               \
    }                                                                          \
                                                                               \
    T5 Get##n5() const {                                                       \
      return n5;                                                               \
    }                                                                          \
                                                                               \
    T6 Get##n6() const {                                                       \
      return n6;                                                               \
    }                                                                          \
                                                                               \
    static void MapFields(                                                     \
      TypeT* type,                                                              \
      void* b,                                                                 \
      FieldMapper& m,                                                          \
      void* aux)                                                               \
    {                                                                          \
      SelfType* self = (SelfType*)b;                                           \
      static Type type0 = Type_Get(self->n0);                                 \
      static Type type1 = Type_Get(self->n1);                                 \
      static Type type2 = Type_Get(self->n2);                                 \
      static Type type3 = Type_Get(self->n3);                                 \
      static Type type4 = Type_Get(self->n4);                                 \
      static Type type5 = Type_Get(self->n5);                                 \
      static Type type6 = Type_Get(self->n6);                                 \
      m(&self->n0, #n0, type0, aux);                                           \
      m(&self->n1, #n1, type1, aux);                                           \
      m(&self->n2, #n2, type2, aux);                                           \
      m(&self->n3, #n3, type3, aux);                                           \
      m(&self->n4, #n4, type4, aux);                                           \
      m(&self->n5, #n5, type5, aux);                                           \
      m(&self->n6, #n6, type6, aux);                                           \
      base::MapFields(type, b, m, aux);                                        \
    }                                                                          \
                                                                               \
    template <class StreamT>                                                   \
    friend void _ToStream(                                                     \
      StreamT& s,                                                              \
      name const& t)                                                           \
    {                                                                          \
      ToStream(s, #name);                                                      \
      ToStream(s, "(");                                                        \
      ToStream(s, t.n0);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n1);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n2);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n3);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n4);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n5);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n6);                                                       \
      ToStream(s, ")");                                                        \
    }                                                                          \
                                                                               \
  /* AUTOCLASS14. */

#define AUTOCLASS16(name, base, T0, n0, T1, n1, T2, n2, T3, n3, T4, n4, T5, n5, T6, n6, T7, n7, T8, n8, T9, n9, T10, n10, T11, n11, T12, n12, T13, n13, T14, n14, T15, n15) \
  struct name : public base {                                                  \
    typedef name SelfType;                                                     \
    typedef base BaseType;                                                     \
    DefineMetadataInline(name)                                                 \
    T0 n0;                                                                     \
    T1 n1;                                                                     \
    T2 n2;                                                                     \
    T3 n3;                                                                     \
    T4 n4;                                                                     \
    T5 n5;                                                                     \
    T6 n6;                                                                     \
    T7 n7;                                                                     \
                                                                               \
    name(                                                                      \
        T0 const& n0,                                                          \
        T1 const& n1,                                                          \
        T2 const& n2,                                                          \
        T3 const& n3,                                                          \
        T4 const& n4,                                                          \
        T5 const& n5,                                                          \
        T6 const& n6,                                                          \
        T7 const& n7) :                                                        \
      n0(n0),                                                                  \
      n1(n1),                                                                  \
      n2(n2),                                                                  \
      n3(n3),                                                                  \
      n4(n4),                                                                  \
      n5(n5),                                                                  \
      n6(n6),                                                                  \
      n7(n7)                                                                   \
    {                                                                          \
      Initializer(0);                                                          \
    }                                                                          \
                                                                               \
    void Initializer(void* ) {                                                 \
                                                                               \
    }                                                                          \
                                                                               \
    T0 Get##n0() const {                                                       \
      return n0;                                                               \
    }                                                                          \
                                                                               \
    T1 Get##n1() const {                                                       \
      return n1;                                                               \
    }                                                                          \
                                                                               \
    T2 Get##n2() const {                                                       \
      return n2;                                                               \
    }                                                                          \
                                                                               \
    T3 Get##n3() const {                                                       \
      return n3;                                                               \
    }                                                                          \
                                                                               \
    T4 Get##n4() const {                                                       \
      return n4;                                                               \
    }                                                                          \
                                                                               \
    T5 Get##n5() const {                                                       \
      return n5;                                                               \
    }                                                                          \
                                                                               \
    T6 Get##n6() const {                                                       \
      return n6;                                                               \
    }                                                                          \
                                                                               \
    T7 Get##n7() const {                                                       \
      return n7;                                                               \
    }                                                                          \
                                                                               \
    static void MapFields(                                                     \
      TypeT* type,                                                              \
      void* b,                                                                 \
      FieldMapper& m,                                                          \
      void* aux)                                                               \
    {                                                                          \
      SelfType* self = (SelfType*)b;                                           \
      static Type type0 = Type_Get(self->n0);                                 \
      static Type type1 = Type_Get(self->n1);                                 \
      static Type type2 = Type_Get(self->n2);                                 \
      static Type type3 = Type_Get(self->n3);                                 \
      static Type type4 = Type_Get(self->n4);                                 \
      static Type type5 = Type_Get(self->n5);                                 \
      static Type type6 = Type_Get(self->n6);                                 \
      static Type type7 = Type_Get(self->n7);                                 \
      m(&self->n0, #n0, type0, aux);                                           \
      m(&self->n1, #n1, type1, aux);                                           \
      m(&self->n2, #n2, type2, aux);                                           \
      m(&self->n3, #n3, type3, aux);                                           \
      m(&self->n4, #n4, type4, aux);                                           \
      m(&self->n5, #n5, type5, aux);                                           \
      m(&self->n6, #n6, type6, aux);                                           \
      m(&self->n7, #n7, type7, aux);                                           \
      base::MapFields(type, b, m, aux);                                        \
    }                                                                          \
                                                                               \
    template <class StreamT>                                                   \
    friend void _ToStream(                                                     \
      StreamT& s,                                                              \
      name const& t)                                                           \
    {                                                                          \
      ToStream(s, #name);                                                      \
      ToStream(s, "(");                                                        \
      ToStream(s, t.n0);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n1);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n2);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n3);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n4);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n5);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n6);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n7);                                                       \
      ToStream(s, ")");                                                        \
    }                                                                          \
                                                                               \
  /* AUTOCLASS16. */

#define AUTOCLASS18(name, base, T0, n0, T1, n1, T2, n2, T3, n3, T4, n4, T5, n5, T6, n6, T7, n7, T8, n8, T9, n9, T10, n10, T11, n11, T12, n12, T13, n13, T14, n14, T15, n15) \
  struct name : public base {                                                  \
    typedef name SelfType;                                                     \
    typedef base BaseType;                                                     \
    DefineMetadataInline(name)                                                 \
    T0 n0;                                                                     \
    T1 n1;                                                                     \
    T2 n2;                                                                     \
    T3 n3;                                                                     \
    T4 n4;                                                                     \
    T5 n5;                                                                     \
    T6 n6;                                                                     \
    T7 n7;                                                                     \
    T8 n8;                                                                     \
                                                                               \
    name(                                                                      \
        T0 const& n0,                                                          \
        T1 const& n1,                                                          \
        T2 const& n2,                                                          \
        T3 const& n3,                                                          \
        T4 const& n4,                                                          \
        T5 const& n5,                                                          \
        T6 const& n6,                                                          \
        T7 const& n7,                                                          \
        T8 const& n8) :                                                        \
      n0(n0),                                                                  \
      n1(n1),                                                                  \
      n2(n2),                                                                  \
      n3(n3),                                                                  \
      n4(n4),                                                                  \
      n5(n5),                                                                  \
      n6(n6),                                                                  \
      n7(n7),                                                                  \
      n8(n8)                                                                   \
    {                                                                          \
      Initializer(0);                                                          \
    }                                                                          \
                                                                               \
    void Initializer(void* ) {                                                 \
                                                                               \
    }                                                                          \
                                                                               \
    T0 Get##n0() const {                                                       \
      return n0;                                                               \
    }                                                                          \
                                                                               \
    T1 Get##n1() const {                                                       \
      return n1;                                                               \
    }                                                                          \
                                                                               \
    T2 Get##n2() const {                                                       \
      return n2;                                                               \
    }                                                                          \
                                                                               \
    T3 Get##n3() const {                                                       \
      return n3;                                                               \
    }                                                                          \
                                                                               \
    T4 Get##n4() const {                                                       \
      return n4;                                                               \
    }                                                                          \
                                                                               \
    T5 Get##n5() const {                                                       \
      return n5;                                                               \
    }                                                                          \
                                                                               \
    T6 Get##n6() const {                                                       \
      return n6;                                                               \
    }                                                                          \
                                                                               \
    T7 Get##n7() const {                                                       \
      return n7;                                                               \
    }                                                                          \
                                                                               \
    T8 Get##n8() const {                                                       \
      return n8;                                                               \
    }                                                                          \
                                                                               \
    static void MapFields(                                                     \
      TypeT* type,                                                              \
      void* b,                                                                 \
      FieldMapper& m,                                                          \
      void* aux)                                                               \
    {                                                                          \
      SelfType* self = (SelfType*)b;                                           \
      static Type type0 = Type_Get(self->n0);                                 \
      static Type type1 = Type_Get(self->n1);                                 \
      static Type type2 = Type_Get(self->n2);                                 \
      static Type type3 = Type_Get(self->n3);                                 \
      static Type type4 = Type_Get(self->n4);                                 \
      static Type type5 = Type_Get(self->n5);                                 \
      static Type type6 = Type_Get(self->n6);                                 \
      static Type type7 = Type_Get(self->n7);                                 \
      static Type type8 = Type_Get(self->n8);                                 \
      m(&self->n0, #n0, type0, aux);                                           \
      m(&self->n1, #n1, type1, aux);                                           \
      m(&self->n2, #n2, type2, aux);                                           \
      m(&self->n3, #n3, type3, aux);                                           \
      m(&self->n4, #n4, type4, aux);                                           \
      m(&self->n5, #n5, type5, aux);                                           \
      m(&self->n6, #n6, type6, aux);                                           \
      m(&self->n7, #n7, type7, aux);                                           \
      m(&self->n8, #n8, type8, aux);                                           \
      base::MapFields(type, b, m, aux);                                        \
    }                                                                          \
                                                                               \
    template <class StreamT>                                                   \
    friend void _ToStream(                                                     \
      StreamT& s,                                                              \
      name const& t)                                                           \
    {                                                                          \
      ToStream(s, #name);                                                      \
      ToStream(s, "(");                                                        \
      ToStream(s, t.n0);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n1);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n2);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n3);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n4);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n5);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n6);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n7);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n8);                                                       \
      ToStream(s, ")");                                                        \
    }                                                                          \
                                                                               \
  /* AUTOCLASS18. */

#define AUTOCLASS20(name, base, T0, n0, T1, n1, T2, n2, T3, n3, T4, n4, T5, n5, T6, n6, T7, n7, T8, n8, T9, n9, T10, n10, T11, n11, T12, n12, T13, n13, T14, n14, T15, n15) \
  struct name : public base {                                                  \
    typedef name SelfType;                                                     \
    typedef base BaseType;                                                     \
    DefineMetadataInline(name)                                                 \
    T0 n0;                                                                     \
    T1 n1;                                                                     \
    T2 n2;                                                                     \
    T3 n3;                                                                     \
    T4 n4;                                                                     \
    T5 n5;                                                                     \
    T6 n6;                                                                     \
    T7 n7;                                                                     \
    T8 n8;                                                                     \
    T9 n9;                                                                     \
                                                                               \
    name(                                                                      \
        T0 const& n0,                                                          \
        T1 const& n1,                                                          \
        T2 const& n2,                                                          \
        T3 const& n3,                                                          \
        T4 const& n4,                                                          \
        T5 const& n5,                                                          \
        T6 const& n6,                                                          \
        T7 const& n7,                                                          \
        T8 const& n8,                                                          \
        T9 const& n9) :                                                        \
      n0(n0),                                                                  \
      n1(n1),                                                                  \
      n2(n2),                                                                  \
      n3(n3),                                                                  \
      n4(n4),                                                                  \
      n5(n5),                                                                  \
      n6(n6),                                                                  \
      n7(n7),                                                                  \
      n8(n8),                                                                  \
      n9(n9)                                                                   \
    {                                                                          \
      Initializer(0);                                                          \
    }                                                                          \
                                                                               \
    void Initializer(void* ) {                                                 \
                                                                               \
    }                                                                          \
                                                                               \
    T0 Get##n0() const {                                                       \
      return n0;                                                               \
    }                                                                          \
                                                                               \
    T1 Get##n1() const {                                                       \
      return n1;                                                               \
    }                                                                          \
                                                                               \
    T2 Get##n2() const {                                                       \
      return n2;                                                               \
    }                                                                          \
                                                                               \
    T3 Get##n3() const {                                                       \
      return n3;                                                               \
    }                                                                          \
                                                                               \
    T4 Get##n4() const {                                                       \
      return n4;                                                               \
    }                                                                          \
                                                                               \
    T5 Get##n5() const {                                                       \
      return n5;                                                               \
    }                                                                          \
                                                                               \
    T6 Get##n6() const {                                                       \
      return n6;                                                               \
    }                                                                          \
                                                                               \
    T7 Get##n7() const {                                                       \
      return n7;                                                               \
    }                                                                          \
                                                                               \
    T8 Get##n8() const {                                                       \
      return n8;                                                               \
    }                                                                          \
                                                                               \
    T9 Get##n9() const {                                                       \
      return n9;                                                               \
    }                                                                          \
                                                                               \
    static void MapFields(                                                     \
      TypeT* type,                                                              \
      void* b,                                                                 \
      FieldMapper& m,                                                          \
      void* aux)                                                               \
    {                                                                          \
      SelfType* self = (SelfType*)b;                                           \
      static Type type0 = Type_Get(self->n0);                                 \
      static Type type1 = Type_Get(self->n1);                                 \
      static Type type2 = Type_Get(self->n2);                                 \
      static Type type3 = Type_Get(self->n3);                                 \
      static Type type4 = Type_Get(self->n4);                                 \
      static Type type5 = Type_Get(self->n5);                                 \
      static Type type6 = Type_Get(self->n6);                                 \
      static Type type7 = Type_Get(self->n7);                                 \
      static Type type8 = Type_Get(self->n8);                                 \
      static Type type9 = Type_Get(self->n9);                                 \
      m(&self->n0, #n0, type0, aux);                                           \
      m(&self->n1, #n1, type1, aux);                                           \
      m(&self->n2, #n2, type2, aux);                                           \
      m(&self->n3, #n3, type3, aux);                                           \
      m(&self->n4, #n4, type4, aux);                                           \
      m(&self->n5, #n5, type5, aux);                                           \
      m(&self->n6, #n6, type6, aux);                                           \
      m(&self->n7, #n7, type7, aux);                                           \
      m(&self->n8, #n8, type8, aux);                                           \
      m(&self->n9, #n9, type9, aux);                                           \
      base::MapFields(type, b, m, aux);                                        \
    }                                                                          \
                                                                               \
    template <class StreamT>                                                   \
    friend void _ToStream(                                                     \
      StreamT& s,                                                              \
      name const& t)                                                           \
    {                                                                          \
      ToStream(s, #name);                                                      \
      ToStream(s, "(");                                                        \
      ToStream(s, t.n0);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n1);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n2);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n3);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n4);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n5);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n6);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n7);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n8);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n9);                                                       \
      ToStream(s, ")");                                                        \
    }                                                                          \
                                                                               \
  /* AUTOCLASS20. */

#define AUTOCLASS22(name, base, T0, n0, T1, n1, T2, n2, T3, n3, T4, n4, T5, n5, T6, n6, T7, n7, T8, n8, T9, n9, T10, n10, T11, n11, T12, n12, T13, n13, T14, n14, T15, n15) \
  struct name : public base {                                                  \
    typedef name SelfType;                                                     \
    typedef base BaseType;                                                     \
    DefineMetadataInline(name)                                                 \
    T0 n0;                                                                     \
    T1 n1;                                                                     \
    T2 n2;                                                                     \
    T3 n3;                                                                     \
    T4 n4;                                                                     \
    T5 n5;                                                                     \
    T6 n6;                                                                     \
    T7 n7;                                                                     \
    T8 n8;                                                                     \
    T9 n9;                                                                     \
    T10 n10;                                                                   \
                                                                               \
    name(                                                                      \
        T0 const& n0,                                                          \
        T1 const& n1,                                                          \
        T2 const& n2,                                                          \
        T3 const& n3,                                                          \
        T4 const& n4,                                                          \
        T5 const& n5,                                                          \
        T6 const& n6,                                                          \
        T7 const& n7,                                                          \
        T8 const& n8,                                                          \
        T9 const& n9,                                                          \
        T10 const& n10) :                                                      \
      n0(n0),                                                                  \
      n1(n1),                                                                  \
      n2(n2),                                                                  \
      n3(n3),                                                                  \
      n4(n4),                                                                  \
      n5(n5),                                                                  \
      n6(n6),                                                                  \
      n7(n7),                                                                  \
      n8(n8),                                                                  \
      n9(n9),                                                                  \
      n10(n10)                                                                 \
    {                                                                          \
      Initializer(0);                                                          \
    }                                                                          \
                                                                               \
    void Initializer(void* ) {                                                 \
                                                                               \
    }                                                                          \
                                                                               \
    T0 Get##n0() const {                                                       \
      return n0;                                                               \
    }                                                                          \
                                                                               \
    T1 Get##n1() const {                                                       \
      return n1;                                                               \
    }                                                                          \
                                                                               \
    T2 Get##n2() const {                                                       \
      return n2;                                                               \
    }                                                                          \
                                                                               \
    T3 Get##n3() const {                                                       \
      return n3;                                                               \
    }                                                                          \
                                                                               \
    T4 Get##n4() const {                                                       \
      return n4;                                                               \
    }                                                                          \
                                                                               \
    T5 Get##n5() const {                                                       \
      return n5;                                                               \
    }                                                                          \
                                                                               \
    T6 Get##n6() const {                                                       \
      return n6;                                                               \
    }                                                                          \
                                                                               \
    T7 Get##n7() const {                                                       \
      return n7;                                                               \
    }                                                                          \
                                                                               \
    T8 Get##n8() const {                                                       \
      return n8;                                                               \
    }                                                                          \
                                                                               \
    T9 Get##n9() const {                                                       \
      return n9;                                                               \
    }                                                                          \
                                                                               \
    T10 Get##n10() const {                                                     \
      return n10;                                                              \
    }                                                                          \
                                                                               \
    static void MapFields(                                                     \
      TypeT* type,                                                              \
      void* b,                                                                 \
      FieldMapper& m,                                                          \
      void* aux)                                                               \
    {                                                                          \
      SelfType* self = (SelfType*)b;                                           \
      static Type type0 = Type_Get(self->n0);                                 \
      static Type type1 = Type_Get(self->n1);                                 \
      static Type type2 = Type_Get(self->n2);                                 \
      static Type type3 = Type_Get(self->n3);                                 \
      static Type type4 = Type_Get(self->n4);                                 \
      static Type type5 = Type_Get(self->n5);                                 \
      static Type type6 = Type_Get(self->n6);                                 \
      static Type type7 = Type_Get(self->n7);                                 \
      static Type type8 = Type_Get(self->n8);                                 \
      static Type type9 = Type_Get(self->n9);                                 \
      static Type type10 = Type_Get(self->n10);                               \
      m(&self->n0, #n0, type0, aux);                                           \
      m(&self->n1, #n1, type1, aux);                                           \
      m(&self->n2, #n2, type2, aux);                                           \
      m(&self->n3, #n3, type3, aux);                                           \
      m(&self->n4, #n4, type4, aux);                                           \
      m(&self->n5, #n5, type5, aux);                                           \
      m(&self->n6, #n6, type6, aux);                                           \
      m(&self->n7, #n7, type7, aux);                                           \
      m(&self->n8, #n8, type8, aux);                                           \
      m(&self->n9, #n9, type9, aux);                                           \
      m(&self->n10, #n10, type10, aux);                                        \
      base::MapFields(type, b, m, aux);                                        \
    }                                                                          \
                                                                               \
    template <class StreamT>                                                   \
    friend void _ToStream(                                                     \
      StreamT& s,                                                              \
      name const& t)                                                           \
    {                                                                          \
      ToStream(s, #name);                                                      \
      ToStream(s, "(");                                                        \
      ToStream(s, t.n0);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n1);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n2);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n3);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n4);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n5);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n6);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n7);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n8);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n9);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n10);                                                      \
      ToStream(s, ")");                                                        \
    }                                                                          \
                                                                               \
  /* AUTOCLASS22. */

#define AUTOCLASS24(name, base, T0, n0, T1, n1, T2, n2, T3, n3, T4, n4, T5, n5, T6, n6, T7, n7, T8, n8, T9, n9, T10, n10, T11, n11, T12, n12, T13, n13, T14, n14, T15, n15) \
  struct name : public base {                                                  \
    typedef name SelfType;                                                     \
    typedef base BaseType;                                                     \
    DefineMetadataInline(name)                                                 \
    T0 n0;                                                                     \
    T1 n1;                                                                     \
    T2 n2;                                                                     \
    T3 n3;                                                                     \
    T4 n4;                                                                     \
    T5 n5;                                                                     \
    T6 n6;                                                                     \
    T7 n7;                                                                     \
    T8 n8;                                                                     \
    T9 n9;                                                                     \
    T10 n10;                                                                   \
    T11 n11;                                                                   \
                                                                               \
    name(                                                                      \
        T0 const& n0,                                                          \
        T1 const& n1,                                                          \
        T2 const& n2,                                                          \
        T3 const& n3,                                                          \
        T4 const& n4,                                                          \
        T5 const& n5,                                                          \
        T6 const& n6,                                                          \
        T7 const& n7,                                                          \
        T8 const& n8,                                                          \
        T9 const& n9,                                                          \
        T10 const& n10,                                                        \
        T11 const& n11) :                                                      \
      n0(n0),                                                                  \
      n1(n1),                                                                  \
      n2(n2),                                                                  \
      n3(n3),                                                                  \
      n4(n4),                                                                  \
      n5(n5),                                                                  \
      n6(n6),                                                                  \
      n7(n7),                                                                  \
      n8(n8),                                                                  \
      n9(n9),                                                                  \
      n10(n10),                                                                \
      n11(n11)                                                                 \
    {                                                                          \
      Initializer(0);                                                          \
    }                                                                          \
                                                                               \
    void Initializer(void* ) {                                                 \
                                                                               \
    }                                                                          \
                                                                               \
    T0 Get##n0() const {                                                       \
      return n0;                                                               \
    }                                                                          \
                                                                               \
    T1 Get##n1() const {                                                       \
      return n1;                                                               \
    }                                                                          \
                                                                               \
    T2 Get##n2() const {                                                       \
      return n2;                                                               \
    }                                                                          \
                                                                               \
    T3 Get##n3() const {                                                       \
      return n3;                                                               \
    }                                                                          \
                                                                               \
    T4 Get##n4() const {                                                       \
      return n4;                                                               \
    }                                                                          \
                                                                               \
    T5 Get##n5() const {                                                       \
      return n5;                                                               \
    }                                                                          \
                                                                               \
    T6 Get##n6() const {                                                       \
      return n6;                                                               \
    }                                                                          \
                                                                               \
    T7 Get##n7() const {                                                       \
      return n7;                                                               \
    }                                                                          \
                                                                               \
    T8 Get##n8() const {                                                       \
      return n8;                                                               \
    }                                                                          \
                                                                               \
    T9 Get##n9() const {                                                       \
      return n9;                                                               \
    }                                                                          \
                                                                               \
    T10 Get##n10() const {                                                     \
      return n10;                                                              \
    }                                                                          \
                                                                               \
    T11 Get##n11() const {                                                     \
      return n11;                                                              \
    }                                                                          \
                                                                               \
    static void MapFields(                                                     \
      TypeT* type,                                                              \
      void* b,                                                                 \
      FieldMapper& m,                                                          \
      void* aux)                                                               \
    {                                                                          \
      SelfType* self = (SelfType*)b;                                           \
      static Type type0 = Type_Get(self->n0);                                 \
      static Type type1 = Type_Get(self->n1);                                 \
      static Type type2 = Type_Get(self->n2);                                 \
      static Type type3 = Type_Get(self->n3);                                 \
      static Type type4 = Type_Get(self->n4);                                 \
      static Type type5 = Type_Get(self->n5);                                 \
      static Type type6 = Type_Get(self->n6);                                 \
      static Type type7 = Type_Get(self->n7);                                 \
      static Type type8 = Type_Get(self->n8);                                 \
      static Type type9 = Type_Get(self->n9);                                 \
      static Type type10 = Type_Get(self->n10);                               \
      static Type type11 = Type_Get(self->n11);                               \
      m(&self->n0, #n0, type0, aux);                                           \
      m(&self->n1, #n1, type1, aux);                                           \
      m(&self->n2, #n2, type2, aux);                                           \
      m(&self->n3, #n3, type3, aux);                                           \
      m(&self->n4, #n4, type4, aux);                                           \
      m(&self->n5, #n5, type5, aux);                                           \
      m(&self->n6, #n6, type6, aux);                                           \
      m(&self->n7, #n7, type7, aux);                                           \
      m(&self->n8, #n8, type8, aux);                                           \
      m(&self->n9, #n9, type9, aux);                                           \
      m(&self->n10, #n10, type10, aux);                                        \
      m(&self->n11, #n11, type11, aux);                                        \
      base::MapFields(type, b, m, aux);                                        \
    }                                                                          \
                                                                               \
    template <class StreamT>                                                   \
    friend void _ToStream(                                                     \
      StreamT& s,                                                              \
      name const& t)                                                           \
    {                                                                          \
      ToStream(s, #name);                                                      \
      ToStream(s, "(");                                                        \
      ToStream(s, t.n0);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n1);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n2);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n3);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n4);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n5);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n6);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n7);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n8);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n9);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n10);                                                      \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n11);                                                      \
      ToStream(s, ")");                                                        \
    }                                                                          \
                                                                               \
  /* AUTOCLASS24. */

#define AUTOCLASS26(name, base, T0, n0, T1, n1, T2, n2, T3, n3, T4, n4, T5, n5, T6, n6, T7, n7, T8, n8, T9, n9, T10, n10, T11, n11, T12, n12, T13, n13, T14, n14, T15, n15) \
  struct name : public base {                                                  \
    typedef name SelfType;                                                     \
    typedef base BaseType;                                                     \
    DefineMetadataInline(name)                                                 \
    T0 n0;                                                                     \
    T1 n1;                                                                     \
    T2 n2;                                                                     \
    T3 n3;                                                                     \
    T4 n4;                                                                     \
    T5 n5;                                                                     \
    T6 n6;                                                                     \
    T7 n7;                                                                     \
    T8 n8;                                                                     \
    T9 n9;                                                                     \
    T10 n10;                                                                   \
    T11 n11;                                                                   \
    T12 n12;                                                                   \
                                                                               \
    name(                                                                      \
        T0 const& n0,                                                          \
        T1 const& n1,                                                          \
        T2 const& n2,                                                          \
        T3 const& n3,                                                          \
        T4 const& n4,                                                          \
        T5 const& n5,                                                          \
        T6 const& n6,                                                          \
        T7 const& n7,                                                          \
        T8 const& n8,                                                          \
        T9 const& n9,                                                          \
        T10 const& n10,                                                        \
        T11 const& n11,                                                        \
        T12 const& n12) :                                                      \
      n0(n0),                                                                  \
      n1(n1),                                                                  \
      n2(n2),                                                                  \
      n3(n3),                                                                  \
      n4(n4),                                                                  \
      n5(n5),                                                                  \
      n6(n6),                                                                  \
      n7(n7),                                                                  \
      n8(n8),                                                                  \
      n9(n9),                                                                  \
      n10(n10),                                                                \
      n11(n11),                                                                \
      n12(n12)                                                                 \
    {                                                                          \
      Initializer(0);                                                          \
    }                                                                          \
                                                                               \
    void Initializer(void* ) {                                                 \
                                                                               \
    }                                                                          \
                                                                               \
    T0 Get##n0() const {                                                       \
      return n0;                                                               \
    }                                                                          \
                                                                               \
    T1 Get##n1() const {                                                       \
      return n1;                                                               \
    }                                                                          \
                                                                               \
    T2 Get##n2() const {                                                       \
      return n2;                                                               \
    }                                                                          \
                                                                               \
    T3 Get##n3() const {                                                       \
      return n3;                                                               \
    }                                                                          \
                                                                               \
    T4 Get##n4() const {                                                       \
      return n4;                                                               \
    }                                                                          \
                                                                               \
    T5 Get##n5() const {                                                       \
      return n5;                                                               \
    }                                                                          \
                                                                               \
    T6 Get##n6() const {                                                       \
      return n6;                                                               \
    }                                                                          \
                                                                               \
    T7 Get##n7() const {                                                       \
      return n7;                                                               \
    }                                                                          \
                                                                               \
    T8 Get##n8() const {                                                       \
      return n8;                                                               \
    }                                                                          \
                                                                               \
    T9 Get##n9() const {                                                       \
      return n9;                                                               \
    }                                                                          \
                                                                               \
    T10 Get##n10() const {                                                     \
      return n10;                                                              \
    }                                                                          \
                                                                               \
    T11 Get##n11() const {                                                     \
      return n11;                                                              \
    }                                                                          \
                                                                               \
    T12 Get##n12() const {                                                     \
      return n12;                                                              \
    }                                                                          \
                                                                               \
    static void MapFields(                                                     \
      TypeT* type,                                                              \
      void* b,                                                                 \
      FieldMapper& m,                                                          \
      void* aux)                                                               \
    {                                                                          \
      SelfType* self = (SelfType*)b;                                           \
      static Type type0 = Type_Get(self->n0);                                 \
      static Type type1 = Type_Get(self->n1);                                 \
      static Type type2 = Type_Get(self->n2);                                 \
      static Type type3 = Type_Get(self->n3);                                 \
      static Type type4 = Type_Get(self->n4);                                 \
      static Type type5 = Type_Get(self->n5);                                 \
      static Type type6 = Type_Get(self->n6);                                 \
      static Type type7 = Type_Get(self->n7);                                 \
      static Type type8 = Type_Get(self->n8);                                 \
      static Type type9 = Type_Get(self->n9);                                 \
      static Type type10 = Type_Get(self->n10);                               \
      static Type type11 = Type_Get(self->n11);                               \
      static Type type12 = Type_Get(self->n12);                               \
      m(&self->n0, #n0, type0, aux);                                           \
      m(&self->n1, #n1, type1, aux);                                           \
      m(&self->n2, #n2, type2, aux);                                           \
      m(&self->n3, #n3, type3, aux);                                           \
      m(&self->n4, #n4, type4, aux);                                           \
      m(&self->n5, #n5, type5, aux);                                           \
      m(&self->n6, #n6, type6, aux);                                           \
      m(&self->n7, #n7, type7, aux);                                           \
      m(&self->n8, #n8, type8, aux);                                           \
      m(&self->n9, #n9, type9, aux);                                           \
      m(&self->n10, #n10, type10, aux);                                        \
      m(&self->n11, #n11, type11, aux);                                        \
      m(&self->n12, #n12, type12, aux);                                        \
      base::MapFields(type, b, m, aux);                                        \
    }                                                                          \
                                                                               \
    template <class StreamT>                                                   \
    friend void _ToStream(                                                     \
      StreamT& s,                                                              \
      name const& t)                                                           \
    {                                                                          \
      ToStream(s, #name);                                                      \
      ToStream(s, "(");                                                        \
      ToStream(s, t.n0);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n1);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n2);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n3);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n4);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n5);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n6);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n7);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n8);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n9);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n10);                                                      \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n11);                                                      \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n12);                                                      \
      ToStream(s, ")");                                                        \
    }                                                                          \
                                                                               \
  /* AUTOCLASS26. */

#define AUTOCLASS28(name, base, T0, n0, T1, n1, T2, n2, T3, n3, T4, n4, T5, n5, T6, n6, T7, n7, T8, n8, T9, n9, T10, n10, T11, n11, T12, n12, T13, n13, T14, n14, T15, n15) \
  struct name : public base {                                                  \
    typedef name SelfType;                                                     \
    typedef base BaseType;                                                     \
    DefineMetadataInline(name)                                                 \
    T0 n0;                                                                     \
    T1 n1;                                                                     \
    T2 n2;                                                                     \
    T3 n3;                                                                     \
    T4 n4;                                                                     \
    T5 n5;                                                                     \
    T6 n6;                                                                     \
    T7 n7;                                                                     \
    T8 n8;                                                                     \
    T9 n9;                                                                     \
    T10 n10;                                                                   \
    T11 n11;                                                                   \
    T12 n12;                                                                   \
    T13 n13;                                                                   \
                                                                               \
    name(                                                                      \
        T0 const& n0,                                                          \
        T1 const& n1,                                                          \
        T2 const& n2,                                                          \
        T3 const& n3,                                                          \
        T4 const& n4,                                                          \
        T5 const& n5,                                                          \
        T6 const& n6,                                                          \
        T7 const& n7,                                                          \
        T8 const& n8,                                                          \
        T9 const& n9,                                                          \
        T10 const& n10,                                                        \
        T11 const& n11,                                                        \
        T12 const& n12,                                                        \
        T13 const& n13) :                                                      \
      n0(n0),                                                                  \
      n1(n1),                                                                  \
      n2(n2),                                                                  \
      n3(n3),                                                                  \
      n4(n4),                                                                  \
      n5(n5),                                                                  \
      n6(n6),                                                                  \
      n7(n7),                                                                  \
      n8(n8),                                                                  \
      n9(n9),                                                                  \
      n10(n10),                                                                \
      n11(n11),                                                                \
      n12(n12),                                                                \
      n13(n13)                                                                 \
    {                                                                          \
      Initializer(0);                                                          \
    }                                                                          \
                                                                               \
    void Initializer(void* ) {                                                 \
                                                                               \
    }                                                                          \
                                                                               \
    T0 Get##n0() const {                                                       \
      return n0;                                                               \
    }                                                                          \
                                                                               \
    T1 Get##n1() const {                                                       \
      return n1;                                                               \
    }                                                                          \
                                                                               \
    T2 Get##n2() const {                                                       \
      return n2;                                                               \
    }                                                                          \
                                                                               \
    T3 Get##n3() const {                                                       \
      return n3;                                                               \
    }                                                                          \
                                                                               \
    T4 Get##n4() const {                                                       \
      return n4;                                                               \
    }                                                                          \
                                                                               \
    T5 Get##n5() const {                                                       \
      return n5;                                                               \
    }                                                                          \
                                                                               \
    T6 Get##n6() const {                                                       \
      return n6;                                                               \
    }                                                                          \
                                                                               \
    T7 Get##n7() const {                                                       \
      return n7;                                                               \
    }                                                                          \
                                                                               \
    T8 Get##n8() const {                                                       \
      return n8;                                                               \
    }                                                                          \
                                                                               \
    T9 Get##n9() const {                                                       \
      return n9;                                                               \
    }                                                                          \
                                                                               \
    T10 Get##n10() const {                                                     \
      return n10;                                                              \
    }                                                                          \
                                                                               \
    T11 Get##n11() const {                                                     \
      return n11;                                                              \
    }                                                                          \
                                                                               \
    T12 Get##n12() const {                                                     \
      return n12;                                                              \
    }                                                                          \
                                                                               \
    T13 Get##n13() const {                                                     \
      return n13;                                                              \
    }                                                                          \
                                                                               \
    static void MapFields(                                                     \
      TypeT* type,                                                              \
      void* b,                                                                 \
      FieldMapper& m,                                                          \
      void* aux)                                                               \
    {                                                                          \
      SelfType* self = (SelfType*)b;                                           \
      static Type type0 = Type_Get(self->n0);                                 \
      static Type type1 = Type_Get(self->n1);                                 \
      static Type type2 = Type_Get(self->n2);                                 \
      static Type type3 = Type_Get(self->n3);                                 \
      static Type type4 = Type_Get(self->n4);                                 \
      static Type type5 = Type_Get(self->n5);                                 \
      static Type type6 = Type_Get(self->n6);                                 \
      static Type type7 = Type_Get(self->n7);                                 \
      static Type type8 = Type_Get(self->n8);                                 \
      static Type type9 = Type_Get(self->n9);                                 \
      static Type type10 = Type_Get(self->n10);                               \
      static Type type11 = Type_Get(self->n11);                               \
      static Type type12 = Type_Get(self->n12);                               \
      static Type type13 = Type_Get(self->n13);                               \
      m(&self->n0, #n0, type0, aux);                                           \
      m(&self->n1, #n1, type1, aux);                                           \
      m(&self->n2, #n2, type2, aux);                                           \
      m(&self->n3, #n3, type3, aux);                                           \
      m(&self->n4, #n4, type4, aux);                                           \
      m(&self->n5, #n5, type5, aux);                                           \
      m(&self->n6, #n6, type6, aux);                                           \
      m(&self->n7, #n7, type7, aux);                                           \
      m(&self->n8, #n8, type8, aux);                                           \
      m(&self->n9, #n9, type9, aux);                                           \
      m(&self->n10, #n10, type10, aux);                                        \
      m(&self->n11, #n11, type11, aux);                                        \
      m(&self->n12, #n12, type12, aux);                                        \
      m(&self->n13, #n13, type13, aux);                                        \
      base::MapFields(type, b, m, aux);                                        \
    }                                                                          \
                                                                               \
    template <class StreamT>                                                   \
    friend void _ToStream(                                                     \
      StreamT& s,                                                              \
      name const& t)                                                           \
    {                                                                          \
      ToStream(s, #name);                                                      \
      ToStream(s, "(");                                                        \
      ToStream(s, t.n0);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n1);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n2);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n3);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n4);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n5);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n6);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n7);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n8);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n9);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n10);                                                      \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n11);                                                      \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n12);                                                      \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n13);                                                      \
      ToStream(s, ")");                                                        \
    }                                                                          \
                                                                               \
  /* AUTOCLASS28. */

#define AUTOCLASS30(name, base, T0, n0, T1, n1, T2, n2, T3, n3, T4, n4, T5, n5, T6, n6, T7, n7, T8, n8, T9, n9, T10, n10, T11, n11, T12, n12, T13, n13, T14, n14, T15, n15) \
  struct name : public base {                                                  \
    typedef name SelfType;                                                     \
    typedef base BaseType;                                                     \
    DefineMetadataInline(name)                                                 \
    T0 n0;                                                                     \
    T1 n1;                                                                     \
    T2 n2;                                                                     \
    T3 n3;                                                                     \
    T4 n4;                                                                     \
    T5 n5;                                                                     \
    T6 n6;                                                                     \
    T7 n7;                                                                     \
    T8 n8;                                                                     \
    T9 n9;                                                                     \
    T10 n10;                                                                   \
    T11 n11;                                                                   \
    T12 n12;                                                                   \
    T13 n13;                                                                   \
    T14 n14;                                                                   \
                                                                               \
    name(                                                                      \
        T0 const& n0,                                                          \
        T1 const& n1,                                                          \
        T2 const& n2,                                                          \
        T3 const& n3,                                                          \
        T4 const& n4,                                                          \
        T5 const& n5,                                                          \
        T6 const& n6,                                                          \
        T7 const& n7,                                                          \
        T8 const& n8,                                                          \
        T9 const& n9,                                                          \
        T10 const& n10,                                                        \
        T11 const& n11,                                                        \
        T12 const& n12,                                                        \
        T13 const& n13,                                                        \
        T14 const& n14) :                                                      \
      n0(n0),                                                                  \
      n1(n1),                                                                  \
      n2(n2),                                                                  \
      n3(n3),                                                                  \
      n4(n4),                                                                  \
      n5(n5),                                                                  \
      n6(n6),                                                                  \
      n7(n7),                                                                  \
      n8(n8),                                                                  \
      n9(n9),                                                                  \
      n10(n10),                                                                \
      n11(n11),                                                                \
      n12(n12),                                                                \
      n13(n13),                                                                \
      n14(n14)                                                                 \
    {                                                                          \
      Initializer(0);                                                          \
    }                                                                          \
                                                                               \
    void Initializer(void* ) {                                                 \
                                                                               \
    }                                                                          \
                                                                               \
    T0 Get##n0() const {                                                       \
      return n0;                                                               \
    }                                                                          \
                                                                               \
    T1 Get##n1() const {                                                       \
      return n1;                                                               \
    }                                                                          \
                                                                               \
    T2 Get##n2() const {                                                       \
      return n2;                                                               \
    }                                                                          \
                                                                               \
    T3 Get##n3() const {                                                       \
      return n3;                                                               \
    }                                                                          \
                                                                               \
    T4 Get##n4() const {                                                       \
      return n4;                                                               \
    }                                                                          \
                                                                               \
    T5 Get##n5() const {                                                       \
      return n5;                                                               \
    }                                                                          \
                                                                               \
    T6 Get##n6() const {                                                       \
      return n6;                                                               \
    }                                                                          \
                                                                               \
    T7 Get##n7() const {                                                       \
      return n7;                                                               \
    }                                                                          \
                                                                               \
    T8 Get##n8() const {                                                       \
      return n8;                                                               \
    }                                                                          \
                                                                               \
    T9 Get##n9() const {                                                       \
      return n9;                                                               \
    }                                                                          \
                                                                               \
    T10 Get##n10() const {                                                     \
      return n10;                                                              \
    }                                                                          \
                                                                               \
    T11 Get##n11() const {                                                     \
      return n11;                                                              \
    }                                                                          \
                                                                               \
    T12 Get##n12() const {                                                     \
      return n12;                                                              \
    }                                                                          \
                                                                               \
    T13 Get##n13() const {                                                     \
      return n13;                                                              \
    }                                                                          \
                                                                               \
    T14 Get##n14() const {                                                     \
      return n14;                                                              \
    }                                                                          \
                                                                               \
    static void MapFields(                                                     \
      TypeT* type,                                                              \
      void* b,                                                                 \
      FieldMapper& m,                                                          \
      void* aux)                                                               \
    {                                                                          \
      SelfType* self = (SelfType*)b;                                           \
      static Type type0 = Type_Get(self->n0);                                 \
      static Type type1 = Type_Get(self->n1);                                 \
      static Type type2 = Type_Get(self->n2);                                 \
      static Type type3 = Type_Get(self->n3);                                 \
      static Type type4 = Type_Get(self->n4);                                 \
      static Type type5 = Type_Get(self->n5);                                 \
      static Type type6 = Type_Get(self->n6);                                 \
      static Type type7 = Type_Get(self->n7);                                 \
      static Type type8 = Type_Get(self->n8);                                 \
      static Type type9 = Type_Get(self->n9);                                 \
      static Type type10 = Type_Get(self->n10);                               \
      static Type type11 = Type_Get(self->n11);                               \
      static Type type12 = Type_Get(self->n12);                               \
      static Type type13 = Type_Get(self->n13);                               \
      static Type type14 = Type_Get(self->n14);                               \
      m(&self->n0, #n0, type0, aux);                                           \
      m(&self->n1, #n1, type1, aux);                                           \
      m(&self->n2, #n2, type2, aux);                                           \
      m(&self->n3, #n3, type3, aux);                                           \
      m(&self->n4, #n4, type4, aux);                                           \
      m(&self->n5, #n5, type5, aux);                                           \
      m(&self->n6, #n6, type6, aux);                                           \
      m(&self->n7, #n7, type7, aux);                                           \
      m(&self->n8, #n8, type8, aux);                                           \
      m(&self->n9, #n9, type9, aux);                                           \
      m(&self->n10, #n10, type10, aux);                                        \
      m(&self->n11, #n11, type11, aux);                                        \
      m(&self->n12, #n12, type12, aux);                                        \
      m(&self->n13, #n13, type13, aux);                                        \
      m(&self->n14, #n14, type14, aux);                                        \
      base::MapFields(type, b, m, aux);                                        \
    }                                                                          \
                                                                               \
    template <class StreamT>                                                   \
    friend void _ToStream(                                                     \
      StreamT& s,                                                              \
      name const& t)                                                           \
    {                                                                          \
      ToStream(s, #name);                                                      \
      ToStream(s, "(");                                                        \
      ToStream(s, t.n0);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n1);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n2);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n3);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n4);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n5);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n6);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n7);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n8);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n9);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n10);                                                      \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n11);                                                      \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n12);                                                      \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n13);                                                      \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n14);                                                      \
      ToStream(s, ")");                                                        \
    }                                                                          \
                                                                               \
  /* AUTOCLASS30. */

#define AUTOCLASS32(name, base, T0, n0, T1, n1, T2, n2, T3, n3, T4, n4, T5, n5, T6, n6, T7, n7, T8, n8, T9, n9, T10, n10, T11, n11, T12, n12, T13, n13, T14, n14, T15, n15) \
  struct name : public base {                                                  \
    typedef name SelfType;                                                     \
    typedef base BaseType;                                                     \
    DefineMetadataInline(name)                                                 \
    T0 n0;                                                                     \
    T1 n1;                                                                     \
    T2 n2;                                                                     \
    T3 n3;                                                                     \
    T4 n4;                                                                     \
    T5 n5;                                                                     \
    T6 n6;                                                                     \
    T7 n7;                                                                     \
    T8 n8;                                                                     \
    T9 n9;                                                                     \
    T10 n10;                                                                   \
    T11 n11;                                                                   \
    T12 n12;                                                                   \
    T13 n13;                                                                   \
    T14 n14;                                                                   \
    T15 n15;                                                                   \
                                                                               \
    name(                                                                      \
        T0 const& n0,                                                          \
        T1 const& n1,                                                          \
        T2 const& n2,                                                          \
        T3 const& n3,                                                          \
        T4 const& n4,                                                          \
        T5 const& n5,                                                          \
        T6 const& n6,                                                          \
        T7 const& n7,                                                          \
        T8 const& n8,                                                          \
        T9 const& n9,                                                          \
        T10 const& n10,                                                        \
        T11 const& n11,                                                        \
        T12 const& n12,                                                        \
        T13 const& n13,                                                        \
        T14 const& n14,                                                        \
        T15 const& n15) :                                                      \
      n0(n0),                                                                  \
      n1(n1),                                                                  \
      n2(n2),                                                                  \
      n3(n3),                                                                  \
      n4(n4),                                                                  \
      n5(n5),                                                                  \
      n6(n6),                                                                  \
      n7(n7),                                                                  \
      n8(n8),                                                                  \
      n9(n9),                                                                  \
      n10(n10),                                                                \
      n11(n11),                                                                \
      n12(n12),                                                                \
      n13(n13),                                                                \
      n14(n14),                                                                \
      n15(n15)                                                                 \
    {                                                                          \
      Initializer(0);                                                          \
    }                                                                          \
                                                                               \
    void Initializer(void* ) {                                                 \
                                                                               \
    }                                                                          \
                                                                               \
    T0 Get##n0() const {                                                       \
      return n0;                                                               \
    }                                                                          \
                                                                               \
    T1 Get##n1() const {                                                       \
      return n1;                                                               \
    }                                                                          \
                                                                               \
    T2 Get##n2() const {                                                       \
      return n2;                                                               \
    }                                                                          \
                                                                               \
    T3 Get##n3() const {                                                       \
      return n3;                                                               \
    }                                                                          \
                                                                               \
    T4 Get##n4() const {                                                       \
      return n4;                                                               \
    }                                                                          \
                                                                               \
    T5 Get##n5() const {                                                       \
      return n5;                                                               \
    }                                                                          \
                                                                               \
    T6 Get##n6() const {                                                       \
      return n6;                                                               \
    }                                                                          \
                                                                               \
    T7 Get##n7() const {                                                       \
      return n7;                                                               \
    }                                                                          \
                                                                               \
    T8 Get##n8() const {                                                       \
      return n8;                                                               \
    }                                                                          \
                                                                               \
    T9 Get##n9() const {                                                       \
      return n9;                                                               \
    }                                                                          \
                                                                               \
    T10 Get##n10() const {                                                     \
      return n10;                                                              \
    }                                                                          \
                                                                               \
    T11 Get##n11() const {                                                     \
      return n11;                                                              \
    }                                                                          \
                                                                               \
    T12 Get##n12() const {                                                     \
      return n12;                                                              \
    }                                                                          \
                                                                               \
    T13 Get##n13() const {                                                     \
      return n13;                                                              \
    }                                                                          \
                                                                               \
    T14 Get##n14() const {                                                     \
      return n14;                                                              \
    }                                                                          \
                                                                               \
    T15 Get##n15() const {                                                     \
      return n15;                                                              \
    }                                                                          \
                                                                               \
    static void MapFields(                                                     \
      TypeT* type,                                                              \
      void* b,                                                                 \
      FieldMapper& m,                                                          \
      void* aux)                                                               \
    {                                                                          \
      SelfType* self = (SelfType*)b;                                           \
      static Type type0 = Type_Get(self->n0);                                 \
      static Type type1 = Type_Get(self->n1);                                 \
      static Type type2 = Type_Get(self->n2);                                 \
      static Type type3 = Type_Get(self->n3);                                 \
      static Type type4 = Type_Get(self->n4);                                 \
      static Type type5 = Type_Get(self->n5);                                 \
      static Type type6 = Type_Get(self->n6);                                 \
      static Type type7 = Type_Get(self->n7);                                 \
      static Type type8 = Type_Get(self->n8);                                 \
      static Type type9 = Type_Get(self->n9);                                 \
      static Type type10 = Type_Get(self->n10);                               \
      static Type type11 = Type_Get(self->n11);                               \
      static Type type12 = Type_Get(self->n12);                               \
      static Type type13 = Type_Get(self->n13);                               \
      static Type type14 = Type_Get(self->n14);                               \
      static Type type15 = Type_Get(self->n15);                               \
      m(&self->n0, #n0, type0, aux);                                           \
      m(&self->n1, #n1, type1, aux);                                           \
      m(&self->n2, #n2, type2, aux);                                           \
      m(&self->n3, #n3, type3, aux);                                           \
      m(&self->n4, #n4, type4, aux);                                           \
      m(&self->n5, #n5, type5, aux);                                           \
      m(&self->n6, #n6, type6, aux);                                           \
      m(&self->n7, #n7, type7, aux);                                           \
      m(&self->n8, #n8, type8, aux);                                           \
      m(&self->n9, #n9, type9, aux);                                           \
      m(&self->n10, #n10, type10, aux);                                        \
      m(&self->n11, #n11, type11, aux);                                        \
      m(&self->n12, #n12, type12, aux);                                        \
      m(&self->n13, #n13, type13, aux);                                        \
      m(&self->n14, #n14, type14, aux);                                        \
      m(&self->n15, #n15, type15, aux);                                        \
      base::MapFields(type, b, m, aux);                                        \
    }                                                                          \
                                                                               \
    template <class StreamT>                                                   \
    friend void _ToStream(                                                     \
      StreamT& s,                                                              \
      name const& t)                                                           \
    {                                                                          \
      ToStream(s, #name);                                                      \
      ToStream(s, "(");                                                        \
      ToStream(s, t.n0);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n1);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n2);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n3);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n4);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n5);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n6);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n7);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n8);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n9);                                                       \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n10);                                                      \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n11);                                                      \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n12);                                                      \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n13);                                                      \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n14);                                                      \
      ToStream(s, ", ");                                                       \
      ToStream(s, t.n15);                                                      \
      ToStream(s, ")");                                                        \
    }                                                                          \
                                                                               \
  /* AUTOCLASS32. */
