#ifndef UI_Common_h__
#define UI_Common_h__

#include "LTE/Common.h"

struct CompositorT;
struct GlyphState;
struct GlyphT;
struct IconT;
struct InterfaceT;
struct WidgetT;
struct WidgetComponentT;

typedef Reference<CompositorT> Compositor;
typedef Reference<GlyphT> Glyph;
typedef Reference<IconT> Icon;
typedef Reference<InterfaceT> Interface;
typedef Reference<WidgetT> Widget;
typedef Reference<WidgetComponentT> WidgetComponent;

#define WIDGET_FOCUS_X                                                         \
  X(Key)                                                                       \
  X(Mouse)                                                                     \
  X(Scroll)

#define XTYPE FocusType
#define XLIST WIDGET_FOCUS_X
#include "LTE/XEnum.h"
#undef XTYPE
#undef XLIST

#endif
