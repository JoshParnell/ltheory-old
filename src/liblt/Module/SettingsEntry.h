#ifndef Module_SettingsEntry_h__
#define Module_SettingsEntry_h__

#include "LTE/BaseType.h"
#include "LTE/String.h"
#include "UI/Common.h"
#include "UI/Widget.h"

struct SettingsEntry {
  BASE_TYPE(SettingsEntry)

  virtual Widget GetWidget() {
    return nullptr;
  }

  virtual void GetValue(void* buffer) = 0;

  FIELDS {}
};

LT_API SettingsEntry* SettingsEntry_Axis(String const& name, Axis const& defValue);

LT_API SettingsEntry* SettingsEntry_Bool(String const& name, bool defValue);

LT_API SettingsEntry* SettingsEntry_Button(String const& name, Button const& defValue);

LT_API SettingsEntry* SettingsEntry_Color(String const& name, Color const& defValue);

LT_API SettingsEntry* SettingsEntry_Float(
  String const& name,
  float defValue,
  float minimum,
  float maximum);

#endif
