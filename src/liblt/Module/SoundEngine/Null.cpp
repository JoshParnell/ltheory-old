#include "Module/SoundEngine.h"

namespace {
  struct SoundNullImpl : public SoundT {
    void Delete() {}

    bool IsFinished() const {
      return true;
    }

    bool IsLooped() const {
      return false;
    }

    float GetDuration() const {
      return 1;
    }

    float GetPan() const {
      return 0;
    }

    float GetPitch() const {
      return 1;
    }

    float GetVolume() const {
      return 1;
    }

    void SetCursor(float position) {}
    void SetPan(float pan) {}
    void SetPitch(float pitch) {}
    void SetPlaying(bool playing) {}
    void SetVolume(float volume) {}
  };

  struct SoundEngineNullImpl : public SoundEngine {
    char const* GetName() const {
      return "SoundEngine (NULL)";
    }

    Sound Play(Array<float> const& buffer) {
      return new SoundNullImpl;
    }

    Sound Play2D(String const& filename, float volume, bool looped) {
      return new SoundNullImpl;
    }

    Sound Play3D(
      String const& s,
      Object const& o,
      V3 const& v,
      float f,
      float distanceDiv,
      bool looped)
    {
      return new SoundNullImpl;
    }

    void Update() {}
  };
}

SoundEngine* SoundEngine_Null() {
  return new SoundEngineNullImpl;
}
