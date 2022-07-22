#include "Icons.h"

#include "LTE/Math.h"

#include "UI/Glyphs.h"

DefineFunction(Icon_Crosshair) {
  static Icon icon = Icon_Create()
    ->Add(Glyph_Arc(0, 0.5f, 0.1f, 1, 1, 0, 1))
    ->Add(Glyph_Box(V2(-0.5f,  0.0f), V2(0.25f, 0.05f), 1, 1))
    ->Add(Glyph_Box(V2( 0.5f,  0.0f), V2(0.25f, 0.05f), 1, 1))
    ->Add(Glyph_Box(V2( 0.0f, -0.5f), V2(0.05f, 0.25f), 1, 1))
    ->Add(Glyph_Box(V2( 0.0f,  0.5f), V2(0.05f, 0.25f), 1, 1));
  return icon;
}

DefineFunction(Icon_Database) {
  static Icon icon;
  if (!icon) {
    icon = Icon_Create();
    for (uint i = 0; i < 8; ++i) {
      float y = 2.0f * ((float)(i + 1) / (8 + 1)) - 1.0f;
      float w = Rand();
      icon->Add(Glyph_Box(V2(w - 1.0f, y), V2(0.5f * w, 1.0f / 15.0f), 1, 1));
      icon->Add(Glyph_Box(V2(w, y), V2(0.5f * (1.0f - w), 1.0f / 15.0f), 1, 1));
    }
  }
  return icon;
}

DefineFunction(Icon_Disc) {
  static Icon icon = Icon_Create()
    ->Add(Glyph_Arc(0, 0.6f, 0.2f, 1, 1, 0, 1))
    ->Add(Glyph_Arc(0, 0.25f, 0.05f, 1, 1, 0, 1));
  return icon;
}

DefineFunction(Icon_Dock) {
  const Icon icon = Icon_Create()
    ->Add(Glyph_LineFade(V2(0,  0.5f), V2(-1.0f,  0.0f), 1, 1))
    ->Add(Glyph_LineFade(V2(0,  0.5f), V2( 1.0f,  0.0f), 1, 1))
    ->Add(Glyph_LineFade(V2(0,  0.5f), V2( 0.0f, -1.0f), 1, 1))
    ->Add(Glyph_Box(V2(0, 1), V2(1, 0.1f), 1, 1));
  return icon;
}

DefineFunction(Icon_Gear) {
  Icon icon = Icon_Create();
  icon->Add(Glyph_Arc(0, 0.5f, args.width, 1, 1, 0, 1));

  float r2 = 0.5f + args.width + args.spokeWidth;
  float as = 0.5f * (1.0f - args.spacing) / (float)args.spokes;
  
  for (uint i = 0; i < args.spokes; ++i) {
    float t = (float)i / (float)args.spokes;
    icon->Add(Glyph_Arc(0, r2, args.spokeWidth, 1, 1, t, as));
  }

  return icon;
}

DefineFunction(Icon_Refresh) {
  static Icon icon = Icon_Create()
    ->Add(Glyph_Arc(0, 0.8f, 0.01f, 1, 1, 0.0f, 0.15))
    ->Add(Glyph_Arc(0, 0.8f, 0.01f, 1, 1, 0.5f, 0.15))
    ->Add(Glyph_LineFade(0.8f * Polar(kTau * 0.15f), 1.0f * Polar(kTau * 0.1f), 1, 1))
    ->Add(Glyph_LineFade(0.8f * Polar(kTau * 0.15f), 0.6f * Polar(kTau * 0.1f), 1, 1))
    ->Add(Glyph_LineFade(0.8f * Polar(kTau * 0.65f), 1.0f * Polar(kTau * 0.60f), 1, 1))
    ->Add(Glyph_LineFade(0.8f * Polar(kTau * 0.65f), 0.6f * Polar(kTau * 0.60f), 1, 1));
  return icon;
}

DefineFunction(Icon_Task_Mine) {
  static Icon icon;
  if (!icon) {
    icon = Icon_Create()
      ->Add(Glyph_Arc(V2(0,  1.0f), 0.5f, 0.01f, 1, 1, 0.25f, 0.25f))
      ->Add(Glyph_Box(V2(0, -0.2f), V2(0.04f, 0.7f), 1, 1));

    for (uint i = 0; i < 6; ++i) {
      float t = (float)i / 6.0f;
      V2 dir = Polar(kTau * t);
      icon->Add(Glyph_Line(V2(0, 0.5f) + 0.1f * dir, V2(0, 0.5f) + 0.3f * dir, 1, 1));
    }
  }
  return icon;
}

DefineFunction(Icon_Task_Produce) {
  static Icon icon;
  if (!icon) {
    icon = Icon_Create()
      ->Add(Icon_Gear(0.1f, 5, 0.1f, 0.6f), V2(-0.5f, 0.5f), 0.4f)
      ->Add(Icon_Gear(0.1f, 7, 0.1f, 0.5f), V2( 0.5f, 0.5f), 0.8f)
      ->Add(Icon_Gear(0.1f, 6, 0.1f, 0.5f), V2( 0.1f,-0.5f), 0.5f);
  }
  return icon;
}

DefineFunction(Icon_Task_Research) {
  static Icon icon;
  if (!icon) {
    icon = Icon_Create()
      ->Add(Glyph_Arc(0, 0.1f, 0.1f, 1, 1, 0, 1));
    for (uint i = 0; i < 7; ++i) {
      float t = kTau * ((float)i + 0.25f * noise((float)i)) / 7.0f;
      float r = 0.7f + 0.3f * noise(t);
      float rs = 0.15f * noise(t + 0.137f);
      icon->Add(Glyph_Line(0.2f * Polar(t), (r - rs) * Polar(t), 1, 1));
      icon->Add(Glyph_Arc(r * Polar(t), rs, 0, 1, 1, 0, 1));
    }
  }
  return icon;
}
