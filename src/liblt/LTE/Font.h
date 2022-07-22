#ifndef LTE_Font_h__
#define LTE_Font_h__

#include "DeclareFunction.h"
#include "Reference.h"
#include "String.h"

typedef Reference<struct FontT> Font;

struct FontT : public RefCounted {
  FontT() {}
  virtual ~FontT() {}

  virtual void Draw(
    String const& text,
    V2 const& position,
    float size,
    Color const& color,
    float alpha,
    bool additive) const = 0;

  virtual V2 GetTextSize(String const& text, float size) const = 0;
};

DeclareFunction(Font_Get, Font,
  String, path)

#endif
