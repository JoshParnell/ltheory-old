#ifndef LTE_Color_h__
#define LTE_Color_h__

#include "DeclareFunction.h"
#include "V3.h"

struct Color : public V3F {
  typedef V3T<float> BaseType;

  Color() : V3F(0) {}
  Color(float rgb) : V3F(rgb) {}
  Color(float r, float g, float b) : V3F(r, g, b) {}
  Color(V3F const& rgb) : V3F(rgb) {}

  DefineMetadataInline(Color)
};

DeclareFunction(Desaturate, Color,
  Color, color,
  float, amount)

DeclareFunction(Luminance, float,
  Color, color)

DeclareFunction(ToHSL, Color,
  Color, color)

DeclareFunction(ToRGB, Color,
  Color, color)

DeclareFunction(Color_FromWavelength, Color,
  float, wavelength)

const Color Color_Black = Color(0.0f);
const Color Color_White = Color(1.0f);

#endif
