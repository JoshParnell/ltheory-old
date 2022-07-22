#include "SettingsEntry.h"

#include "LTE/Axis.h"
#include "LTE/Button.h"
#include "LTE/Color.h"
#include "LTE/Generic.h"

namespace {
  AutoClassDerived(SettingsAxis, SettingsEntry,
    String, name,
    Axis, value)
    GenericAxis function;

    DERIVED_TYPE_EX(SettingsAxis)

    SettingsAxis() {}

    void GetValue(void* buffer) {
      if (!function)
        function = ConstantPointer(&value);
      *((GenericAxis*)buffer) = function;
    }
  };

  DERIVED_IMPLEMENT(SettingsAxis)

  AutoClassDerived(SettingsBool, SettingsEntry,
    String, name,
    bool, value)
    GenericBool function;

    DERIVED_TYPE_EX(SettingsBool)

    SettingsBool() {}

    void GetValue(void* buffer) {
      if (!function)
        function = ConstantPointer(&value);
      *((GenericBool*)buffer) = function;
    }
  };

  DERIVED_IMPLEMENT(SettingsBool)

  AutoClassDerived(SettingsButton, SettingsEntry,
    String, name,
    Button, value)
    GenericButton function;

    DERIVED_TYPE_EX(SettingsButton)

    SettingsButton() {}

    void GetValue(void* buffer) {
      if (!function)
        function = value;
      *((GenericButton*)buffer) = function;
    }
  };

  DERIVED_IMPLEMENT(SettingsButton)

  AutoClassDerived(SettingsColor, SettingsEntry,
    String, name,
    Color, value)
    GenericColor function;

    DERIVED_TYPE_EX(SettingsColor)

    SettingsColor() {}

    void GetValue(void* buffer) {
      if (!function)
        function = ConstantPointer(&value);
      *((GenericColor*)buffer) = function;
    }
  };

  DERIVED_IMPLEMENT(SettingsColor)

  AutoClassDerived(SettingsFloat, SettingsEntry,
    String, name,
    float, value,
    float, minimum,
    float, maximum)
    GenericFloat function;

    DERIVED_TYPE_EX(SettingsFloat)

    SettingsFloat() {}

    void GetValue(void* buffer) {
      if (!function)
        function = ConstantPointer(&value);
      *((GenericFloat*)buffer) = function;
    }
  };

  DERIVED_IMPLEMENT(SettingsFloat)
}

SettingsEntry* SettingsEntry_Axis(String const& name, Axis const& defValue) {
  return new SettingsAxis(name, defValue);
}

SettingsEntry* SettingsEntry_Bool(String const& name, bool defValue) {
  return new SettingsBool(name, defValue);
}

SettingsEntry* SettingsEntry_Button(String const& name, Button const& defValue) {
  return new SettingsButton(name, defValue);
}

SettingsEntry* SettingsEntry_Color(String const& name, Color const& defValue) {
  return new SettingsColor(name, defValue);
}

SettingsEntry* SettingsEntry_Float(
  String const& name,
  float defValue,
  float minimum,
  float maximum)
{
  return new SettingsFloat(name, defValue, minimum, maximum);
}
