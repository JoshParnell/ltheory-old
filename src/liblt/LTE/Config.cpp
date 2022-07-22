#include "Config.h"
#include "Map.h"
#include "ProgramLog.h"
#include "OS.h"

#include <fstream>

namespace {
  String GetConfigFile() {
    const String configFile = OS_GetUserDataPath() + "config.txt";
    return configFile;
  }

  Map<String, String>& GetValueMap() {
    static Map<String, String> valueMap;
    static bool initialized = false;

    if (!initialized) {
      initialized = true;
      std::ifstream stream(GetConfigFile().c_str());
      if (stream) {
        String buffer;
        while (getline(stream, buffer)) {
          std::stringstream line(buffer);
          String left;
          String right;
          if (!getline(line, left, '='))
            continue;
          if (!getline(line, right, '='))
            continue;
          valueMap[String_Lower(left)] = right;
        }
      } else {
        Log_Warning(Stringize() | "Failed to read configuration file <"
                                | GetConfigFile().c_str() | ">");
      }
    }

    return valueMap;
  }
}

namespace LTE {
  bool Config_Bool(
    String const& name,
    String const& category,
    bool defaultValue)
  {
    String key = String_Lower(name);
    Map<String, String>& map = GetValueMap();
    if (map.contains(key)) {
      return map[key].substr(0, 4) == "true";
    } else {
      map[key] = defaultValue ? "true" : "false";
      Config_Flush();
      return defaultValue;
    }
  }

  float Config_Float(
    String const& name,
    String const& category,
    float defaultValue)
  {
    String key = String_Lower(name);
    Map<String, String>& map = GetValueMap();
    if (map.contains(key))
      return FromString<float>(map[key]);
    else {
      map[key] = ToString(defaultValue);
      Config_Flush();
      return defaultValue;
    }
  }

  void Config_Flush() {
    std::ofstream stream(GetConfigFile().c_str());
    Map<String, String>& map = GetValueMap();
    for (Map<String, String>::iterator it = map.begin();
         it != map.end(); ++it)
    {
      if (it != map.begin())
        stream << '\n';
      stream << it->first << '=' << it->second;
    }
  }

  int Config_Int(
    String const& name,
    String const& category,
    int defaultValue)
  {
    String key = String_Lower(name);
    Map<String, String>& map = GetValueMap();
    if (map.contains(key))
      return FromString<int>(map[key]);
    else {
      map[key] = ToString(defaultValue);
      Config_Flush();
      return defaultValue;
    }
  }

  void Config_Set(
    String const& name,
    String const& category,
    String& value)
  {
    Map<String, String>& map = GetValueMap();
    map[name] = value;
    Config_Flush();
  }
}
