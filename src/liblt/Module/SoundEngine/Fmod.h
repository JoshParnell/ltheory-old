#ifndef Module_SoundEngine_Fmod_h__
#define Module_SoundEngine_Fmod_h__

#include "Module/SoundEngine.h"

struct SoundEvent {
  virtual ~SoundEvent() {}

  virtual void SetParameter(int index, float value) = 0;
  virtual void Start() = 0;
  virtual void Stop() = 0;
};

struct SoundEngineFmod : public SoundEngine {
  virtual SoundEvent* GetEvent(char const* guid) = 0;
  virtual void LoadProject(char const* name) = 0;
};

#endif
