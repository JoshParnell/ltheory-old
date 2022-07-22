#ifndef LTE_Common_h__
#define LTE_Common_h__

#include "../Common.h"

/* Container. */
template <class T1> struct Array;
template <class T1> struct AutoPtr;
template <class T1> struct Distribution;
template <class T1> struct List;
template <class T1> struct Pointer;
template <class T1> struct Reference;
template <class T1> struct RingBuffer;
template <class T1> struct Vector;

template <class T1, class T2> struct Map;
template <class T1, class T2> struct Tuple2;
template <class T1, class T2> struct VectorMap;
template <class T1, class T2, class T3> struct Tuple3;

template <class T1, char const* (*)(int)> struct Enum;
template <class T1, int MaxElements> struct Stack;

typedef Reference<struct ListNPT> ListNP;

/* Engine. */
typedef Reference<struct StringListT> StringList;
typedef Reference<struct StringTreeT> StringTree;

/* Math. */
struct Capsule;
struct Cylinder;
struct Segment;
struct SpatialPartition;
struct Sphere;
struct Transform;
struct View;

typedef Reference<struct CollisionMeshT> CollisionMesh;
typedef Reference<struct RNGT> RNG;
typedef Reference<struct SDFT> SDF;
typedef Reference<struct WarpT> Warp;

/* Math - Parametric. */
template <class T1> struct V2T;
typedef V2T<float> V2;
typedef V2T<float> V2F;
typedef V2T<double> V2D;
typedef V2T<int> V2I;
typedef V2T<uint> V2U;

template <class T1> struct V3T;
typedef V3T<float> V3;
typedef V3T<float> V3F;
typedef V3T<double> V3D;
typedef V3T<int> V3I;
typedef V3T<uint> V3U;

typedef double DistanceT;
typedef V3T<DistanceT> PointT;

template <class T1> struct V4T;
typedef V4T<float> V4;
typedef V4T<float> V4F;
typedef V4T<double> V4D;
typedef V4T<int> V4I;
typedef V4T<uint> V4U;

template <class T1, class T2> struct BoundT;
typedef BoundT<V3, V3>  Bound3;
typedef BoundT<V3F, V3> Bound3F;
typedef BoundT<V3D, V3> Bound3D;

template <class T1> struct MatrixT;
typedef MatrixT<float> Matrix;
typedef MatrixT<float> MatrixF;
typedef MatrixT<double> MatrixD;

template <class T1> struct PlaneT;
typedef PlaneT<float> Plane;
typedef PlaneT<float> PlaneF;
typedef PlaneT<double> PlaneD;

template <class T1, class T2> struct RayT;
typedef RayT<V3, V3> Ray;
typedef RayT<V3F, V3> RayF;
typedef RayT<V3D, V3> RayD;

/* Graphics. */
struct Color;
struct Vertex;

typedef Reference<struct CubeMapT> CubeMap;
typedef Reference<struct FontT> Font;
typedef Reference<struct GeometryT> Geometry;
typedef Reference<struct MeshT> Mesh;
typedef Reference<struct ModelT> Model;
typedef Reference<struct ParticleSystemT> ParticleSystem;
typedef Reference<struct PlateMeshT> PlateMesh;
typedef Reference<struct RenderableT> Renderable;
typedef Reference<struct RenderPassT> RenderPass;
typedef Reference<struct RenderStyleT> RenderStyle;
typedef Reference<struct ShaderT> Shader;
typedef Reference<struct ShaderInstanceT> ShaderInstance;
typedef Reference<struct ViewportT> Viewport;
typedef Reference<struct Texture2DT> Texture2D;
typedef Reference<struct Texture3DT> Texture3D;

/* Program. */
struct Program;
struct Timer;

typedef Reference<struct LockT> Lock;
typedef Reference<struct ModuleT> Module;
typedef Reference<struct ThreadT> Thread;
typedef Reference<struct WindowT> Window;

/* Reflection. */
struct ConversionType;
struct Field;
struct FieldMapper;
struct FieldType;
struct FunctionDesc;
struct Type;
struct TypeT;

typedef Reference<struct FunctionT> Function;
typedef Reference<struct PackageT> Package;

/* Misc. */
struct DrawState;
typedef Reference<struct ResourceMapT> ResourceMap;

namespace LTE {
  /* Control. */
  typedef Reference<struct AxisT> Axis;
  typedef Reference<struct ButtonT> Button;
  struct Joystick;

  /* Engine. */
  struct Data;
  struct DataRef;
  struct DataStack;
  struct Diff;
  struct Grammar;
  struct Patch;
  struct String;

  typedef Reference<struct ExpressionT> Expression;
  typedef Reference<struct JobT> Job;
  typedef Reference<struct LocationT> Location;
  typedef Reference<struct ScriptT> Script;
  typedef Reference<struct ScriptFunctionT> ScriptFunction;
  typedef Reference<struct ScriptTypeT> ScriptType;

  /* Function. */
  template <class ValueType, class ArgType> struct Generic;

  typedef uint HashT;
  #define HASHT_MAX UINT_MAX

  typedef Generic<bool, void> GenericBool;
  typedef Generic<Color, void> GenericColor;
  typedef Generic<int, void> GenericInt;
  typedef Generic<float, void> GenericFloat;
  typedef Generic<void, void> GenericVoid;
  typedef Generic<Axis, void> GenericAxis;
  typedef Generic<Button, void> GenericButton;
  typedef Generic<V2, void> GenericV2;
  typedef Generic<V3, void> GenericV3;
  typedef Generic<V4, void> GenericV4;

  /* Enums. */
  namespace CubeFace {
    enum Enum {
      PositiveX,
      NegativeX,
      PositiveY,
      NegativeY,
      PositiveZ,
      NegativeZ,
      SIZE
    };

    typedef unsigned char BitField;
    const unsigned char PositiveXBit = 0x01;
    const unsigned char NegativeXBit = 0x02;
    const unsigned char PositiveYBit = 0x04;
    const unsigned char NegativeYBit = 0x08;
    const unsigned char PositiveZBit = 0x10;
    const unsigned char NegativeZBit = 0x20;
  }

  #define KEY_X                                                                \
    X(A) X(B) X(C) X(D) X(E) X(F) X(G) X(H) X(I) X(J) X(K) X(L) X(M) X(N) X(O) \
    X(P) X(Q) X(R) X(S) X(T) X(U) X(V) X(W) X(X) X(Y) X(Z)                     \
    X(N0) X(N1) X(N2) X(N3) X(N4) X(N5) X(N6) X(N7) X(N8) X(N9)                \
    X(NP0) X(NP1) X(NP2) X(NP3) X(NP4) X(NP5) X(NP6) X(NP7) X(NP8) X(NP9)      \
    X(F1) X(F2) X(F3) X(F4) X(F5) X(F6) X(F7) X(F8) X(F9) X(F10) X(F11) X(F12) \
    X(F13) X(F14) X(F15)                                                       \
    X(Add)                                                                     \
    X(BackSpace)                                                               \
    X(BackSlash)                                                               \
    X(Comma)                                                                   \
    X(Dash)                                                                    \
    X(Delete)                                                                  \
    X(Divide)                                                                  \
    X(Down)                                                                    \
    X(End)                                                                     \
    X(Equal)                                                                   \
    X(Escape)                                                                  \
    X(Home)                                                                    \
    X(Insert)                                                                  \
    X(LBracket)                                                                \
    X(Left)                                                                    \
    X(Menu)                                                                    \
    X(Multiply)                                                                \
    X(PageDown)                                                                \
    X(PageUp)                                                                  \
    X(Pause)                                                                   \
    X(Period)                                                                  \
    X(Quote)                                                                   \
    X(RBracket)                                                                \
    X(Return)                                                                  \
    X(Right)                                                                   \
    X(SemiColon)                                                               \
    X(Slash)                                                                   \
    X(Space)                                                                   \
    X(Subtract)                                                                \
    X(Tab)                                                                     \
    X(Tilde)                                                                   \
    X(Up)                                                                      \
    X(LAlt)                                                                    \
    X(RAlt)                                                                    \
    X(LControl)                                                                \
    X(RControl)                                                                \
    X(LShift)                                                                  \
    X(RShift)                                                                  \
    X(LSystem)                                                                 \
    X(RSystem)

  #define XLIST KEY_X
  #define XTYPE Key
  #include "LTE/XEnum.h"
  #undef XTYPE
  #undef XLIST

  #define MOUSE_BUTTON_X                                                       \
    X(Left)                                                                    \
    X(Middle)                                                                  \
    X(Right)                                                                   \
    X(X1)                                                                      \
    X(X2)

  #define XLIST MOUSE_BUTTON_X
  #define XTYPE MouseButton
  #include "LTE/XEnum.h"
  #undef XTYPE
  #undef XLIST

  #define JOYSTICK_AXIS_X                                                      \
    X(X)                                                                       \
    X(Y)                                                                       \
    X(Z)                                                                       \
    X(R)                                                                       \
    X(U)                                                                       \
    X(V)                                                                       \
    X(PovX)                                                                    \
    X(PovY)

  #define XLIST JOYSTICK_AXIS_X
  #define XTYPE JoystickAxis
  #include "LTE/XEnum.h"
  #undef XTYPE
  #undef XLIST
}

#define offset_of(type, member) ((volatile void const*)&((type*)0)->member)

#define MACRO_IDENTITY(x) x

#define DeclareMetadata(T)                                                     \
  LT_API friend Type _Type_Get(T const& t);

#define FIELDS                                                                 \
  static void MapFields(                                                       \
    TypeT* type,                                                               \
    void* addr,                                                                \
    FieldMapper& m,                                                            \
    void* aux)

#define PRIMITIVE_TYPE_X                                                       \
  X(bool)                                                                      \
  X(char)                                                                      \
  X(signed char)                                                               \
  X(unsigned char)                                                             \
  X(signed short)                                                              \
  X(unsigned short)                                                            \
  X(signed int)                                                                \
  X(unsigned int)                                                              \
  X(signed long)                                                               \
  X(unsigned long)                                                             \
  X(signed long long)                                                          \
  X(unsigned long long)                                                        \
  X(float)                                                                     \
  X(double)

template <class T>
void Swap(T& one, T& two) {
  T temp = one;
  one = two;
  two = temp;
}

#define X(x) template <class StreamT>                                          \
  inline void _ToStream(StreamT& s, x const& t) {                              \
    s << t; }
PRIMITIVE_TYPE_X
X(char const*)
#undef X

template <class StreamT, class T>
void _ToStream(StreamT& s, T* const& t) {
  s << (void*)t;
}

#if 1
template <class StreamT, class T>
void _ToStream(StreamT& s, T const& t) {
  s << "Unknown Type";
}
#endif

template <class StreamT>
inline void ToStream(StreamT& s, char const* str) {
  s << str;
}

template <class StreamT, class T>
void ToStream(StreamT& s, T const& t) {
  _ToStream(s, t);
}

template <class T>
struct NullBase {
  FIELDS {}
};

struct NoBase {};

#include "StdMath.h"

using namespace LTE;

#endif
