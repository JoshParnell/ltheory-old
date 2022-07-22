#ifndef LTE_SDF_h__
#define LTE_SDF_h__

#include "BaseType.h"
#include "Reference.h"
#include "String.h"
#include "V3.h"

struct SDFT : public RefCounted {
  BASE_TYPE(SDFT)

  virtual float Evaluate(V3 const& p) const = 0;

  virtual Bound3 GetBound() const = 0;

  virtual String GetCode() const {
    return GetCode("p");
  }

  virtual String GetCode(String const& p) const {
    return "0";
  }

  LT_API virtual V3 Gradient(V3 const& p) const;

  LT_API SDF Add(SDF const& other);
  
  LT_API SDF Intersect(SDF const& other, float sharpness = -1);
  
  LT_API SDF Subtract(SDF const& other, float sharpness = -1);
  
  LT_API SDF Union(SDF const& other, float sharpness = -1);

  LT_API SDF Expand(V3 const& amount);
  
  LT_API SDF ExpandRadial(V3 const& axis, float amount);
  
  LT_API SDF Mirror(V3 const& origin, V3 const& normal);

  LT_API SDF MirrorRadial(
    V3 const& origin,
    V3 const& normal,
    float angleSize);

  LT_API SDF Multiply(float value);
  
  LT_API SDF PinchAxis(V3 const& axis);
  
  LT_API SDF PinchY(V3 const& axis);
  
  LT_API SDF Repeat(V3 const& frequency, V3 const& spacing = 0);
  
  LT_API SDF Rotate(V3 const& ypr);
  
  LT_API SDF Scale(V3 const& s);
  
  LT_API SDF Translate(V3 const& t);

  FIELDS {}
};

#endif
