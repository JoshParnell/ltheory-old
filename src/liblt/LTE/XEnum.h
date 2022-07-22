#define ENUM_NAME_2(x) x##_enum
#define ENUM_NAME(x) ENUM_NAME_2(x)
#define ELEMENT_UNAME_2(x, y) x##__##y
#define ELEMENT_UNAME(x, y) ELEMENT_UNAME_2(x, y)
#define ELEMENT_NAME_2(x, y) x##_##y
#define ELEMENT_NAME(x, y) ELEMENT_NAME_2(x, y)
#define STRING_NAME_2(x) x##_String
#define STRING_NAME(x) STRING_NAME_2(x)

enum ENUM_NAME(XTYPE) {
  #define X(x) ELEMENT_UNAME(XTYPE, x),
  #define Y(x, y) ELEMENT_UNAME(XTYPE, x),
  #define Z(x, y, z) ELEMENT_UNAME(XTYPE, y),
  XLIST
  #undef X
  #undef Y
  #undef Z
  ELEMENT_UNAME(XTYPE, SIZE)
};

#define X(x) const int ELEMENT_NAME(XTYPE, x) = (int)ELEMENT_UNAME(XTYPE, x);
#define Y(x, y) const int ELEMENT_NAME(XTYPE, x) = (int)ELEMENT_UNAME(XTYPE, x);
#define Z(x, y, z) const int ELEMENT_NAME(XTYPE, y) = (int)ELEMENT_UNAME(XTYPE, y);
XLIST
#undef X
#undef Y
#undef Z
const int ELEMENT_NAME(XTYPE, SIZE) = (int)ELEMENT_UNAME(XTYPE, SIZE);

char const* const STRING_NAME(XTYPE)[] = {
  #define X(x) #x,
  #define Y(x, y) y,
  #define Z(x, y, z) z,
  XLIST
  #undef X
  #undef Y
  #undef Z
};

typedef int XTYPE;

#undef ENUM_NAME_2
#undef ENUM_NAME
#undef ELEMENT_UNAME_2
#undef ELEMENT_UNAME
#undef ELEMENT_NAME_2
#undef ELEMENT_NAME
#undef STRING_NAME_2
#undef STRING_NAME
