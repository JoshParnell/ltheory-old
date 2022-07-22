#include "MusicEngine.h"
#include "MusicEngine/LtheoryTest01.h"
#include "SoundEngine/Fmod.h"

#include "LTE/Generic.h"
#include "LTE/Map.h"
#include "LTE/Pointer.h"
#include "LTE/Vector.h"

namespace {
  Pointer<MusicEngine> gActive;

  struct MusicEngineImpl : public MusicEngine {
    GenericFloat tension;
    GenericFloat volume;
    Pointer<SoundEvent> playing;
    Reference<SoundEngineFmod> soundEngine;

    MusicEngineImpl() {
      LTE_ASSERT(!gActive);
      gActive = this;
      soundEngine = (SoundEngineFmod*)GetSoundEngine();
      soundEngine->LoadProject("LtheoryTest01.fev");
    }

    ~MusicEngineImpl() {
      gActive = nullptr;
    }

    char const* GetName() const {
      return "MusicEngine";
    }

    MusicEngine* SetTension(GenericFloat const& tension) {
      this->tension = tension;
      return this;
    }

    MusicEngine* SetVolume(GenericFloat const& volume) {
      this->volume = volume;
      return this;
    }

    void Update() {
      if (!playing) {
        playing = soundEngine->GetEvent(
          EVENTGUID_LTHEORYTEST01_TESTEX_BT_01_SPACE01_MUSIC);
        playing->Start();
      }

      if (playing) {
        float t = tension ? tension() : 1.0f;
        float v = volume ? volume() : 1.0f;
        playing->SetParameter(
          EVENTPARAMETER_LTHEORYTEST01_TESTEX_BT_01_SPACE01_MUSIC_SPACE01_EX_BT_END,
          t);

        playing->SetParameter(
          EVENTPARAMETER_LTHEORYTEST01_TESTEX_BT_01_SPACE01_MUSIC_VOLUME__CLOSING_IN_,
          v);
      }
    }
  };
}

MusicEngine* CreateMusicEngine() {
  return new MusicEngineImpl;
}

MusicEngine* GetMusicEngine() {
  LTE_ASSERT(gActive);
  return gActive;
}
