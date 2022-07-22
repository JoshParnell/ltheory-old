#ifndef Game_Settings_h__
#define Game_Settings_h__

#include "Module/Settings.h"

inline GenericFloat Setting_WidgetFadeDistance() {
  return Settings_Float("Interface/HUD/Widget Fade Distance",
    1000.0f, 100000.0f, 10000.0f);
}

#endif
