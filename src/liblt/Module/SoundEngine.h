#ifndef Module_SoundEngine_h__
#define Module_SoundEngine_h__

#include "Common.h"
#include "Sound.h"
#include "LTE/Module.h"
#include "LTE/String.h"
#include "LTE/V3.h"
#include "Game/Common.h"

struct SoundEngine : public ModuleT {
  LT_API SoundEngine();
  LT_API virtual ~SoundEngine();

  LT_API void Pop();
  LT_API void Push();

  virtual Sound Play(Array<float> const& buffer) = 0;

  virtual Sound Play2D(String const& name, float volume, bool looped) = 0;

  virtual Sound Play3D(
    String const& name,
    Object const& carrier,
    V3 const& offset,
    float volume,
    float distanceDiv,
    bool looped) = 0;
};

LT_API SoundEngine* GetSoundEngine();
LT_API SoundEngine* SoundEngine_Fmod();
LT_API SoundEngine* SoundEngine_Null();

inline Sound Sound_Play2D(
  String const& name,
  float volume = 0.5f,
  bool looped = false)
{
  return GetSoundEngine()->Play2D(name, volume, looped);
}

inline Sound Sound_Play3D(
  String const& name,
  Object const& carrier,
  V3 const& offset,
  float volume = 0.5f,
  float distanceDiv = 1,
  bool looped = false)
{
  return GetSoundEngine()->Play3D(
    name, carrier, offset, volume, distanceDiv, looped);
}

#endif
