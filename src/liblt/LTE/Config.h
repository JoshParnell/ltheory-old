#ifndef LTE_Config_h__
#define LTE_Config_h__

#include "String.h"

namespace LTE {

  LT_API bool Config_Bool(
    String const& name,
    String const& category,
    bool defaultValue);

  LT_API float Config_Float(
    String const& name,
    String const& category,
    float defaultValue);

  LT_API void Config_Flush();

  LT_API int Config_Int(
    String const& name,
    String const& category,
    int defaultValue);

  LT_API void Config_Set(
    String const& name,
    String const& category,
    String const& value);

}

#endif
