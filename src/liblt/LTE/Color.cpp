#include "Color.h"

namespace {
  inline float hueToComponent(float p, float q, float t) {
    if (t < 0.0f) t += 1.0f;
    if (t > 1.0f) t -= 1.0f;
    if (t < 1.0f / 6.0f) return p + (q - p) * 6.0f * t;
    if (t < 1.0f / 2.0f) return q;
    if (t < 2.0f / 3.0f) return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;
    return p;
  }
}

DefineConversion(int_to_color, int, Color) {
  dest = (Color)src;
}

DefineConversion(float_to_color, float, Color) {
  dest = (Color)src;
}

DefineConversion(double_to_color, double, Color) {
  dest = (Color)src;
}

DefineConversion(V3F_to_color, V3F, Color) {
  dest = (Color)src;
}

DefineConversion(color_to_V3F, Color, V3F) {
  dest = (V3F)src;
}

DefineFunction(Desaturate) {
  return Mix(args.color, Color(Luminance(args.color)), args.amount);
}

DefineFunction(Luminance) {
  return Dot(args.color, Color(0.2126f, 0.7152f, 0.0722f));
}

DefineFunction(ToHSL) {
  float r = args.color.x;
  float g = args.color.y;
  float b = args.color.z;
  float M = Max(r, Max(g, b));
  float m = Min(r, Min(g, b));
  float mSum = M + m;
  float mDiff = M - m;
  float h, s, l = mSum / 2.0f;
  s = l > 0.5f ? mDiff / (2.0f - mSum) : mDiff / mSum;
  if (r == M)       h = (g - b) / mDiff + (g < b ? 6.0f : 0.0f);
  else if (g == M)  h = (b - r) / mDiff + 2.0f;
  else              h = (r - g) / mDiff + 4.0f;
  return Color(h / 6.0f, s, l);
}

DefineFunction(ToRGB) {
  float h = args.color.x;
  float s = args.color.y;
  float l = args.color.z;

  float q = l < 0.5f ? l * (1.0f + s) : l + s - l * s;
  float p = 2.0f * l - q;
  float r = hueToComponent(p, q, h + 1.0f / 3.0f);
  float g = hueToComponent(p, q, h);
  float b = hueToComponent(p, q, h - 1.0f / 3.0f);
  return Color(r, g, b);
}

DefineFunction(Color_FromWavelength) {
  float w = args.wavelength;
  V3 c = 0;
  if (w >= 380.0f && w < 440.0f) {
      c.x = (440.0f - w) / (440.0f - 380.0f);
      c.z = 1.0f;
  } else if (w >= 440.0f && w < 490.0f) {
      c.y = (w - 440.0f) / (490.0f - 440.0f);
      c.z = 1.0f;
  } else if (w >= 490.0f && w < 510.0f) {
      c.y = 1.0f;
      c.z = -(w - 510.0f) / (510.0f - 490.0f);
  } else if (w >= 510.0f && w < 580.0f) {
      c.x = (w - 510.0f) / (580.0f - 510.0f);
      c.y = 1.0f;
  } else if (w >= 580.0f && w < 645.0f) {
      c.x = 1.0f;
      c.y = -(w - 645.0f) / (645.0f - 580.0f);
  } else if (w >= 645.0f && w < 781.0f)
      c.x = 1.0f;

  if (w >= 380.0f && w < 400.0f)
      c *= (w - 380.0f) / (400.0f - 380.0f);
  else if (w >= 400.0f && w < 740.0f)
      c *= 1.0f;
  else if (w >= 740.0f && w < 780.0f)
      c *= (780.0f - w) / (780.0f - 740.0f);
  else
      c *= 0.0f;
  return c;
}
