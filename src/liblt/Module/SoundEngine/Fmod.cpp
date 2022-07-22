#include "Fmod.h"

#include "Component/Orientation.h"

#include "Game/Camera.h"
#include "Game/Object.h"

#include "LTE/Array.h"
#include "LTE/AutoPtr.h"
#include "LTE/Location.h"
#include "LTE/Map.h"
#include "LTE/Math.h"
#include "LTE/Matrix.h"
#include "LTE/Pointer.h"
#include "LTE/Pool.h"
#include "LTE/ProgramLog.h"
#include "LTE/StackFrame.h"
#include "LTE/Thread.h"
#include "LTE/V3.h"
#include "LTE/Vector.h"

#include "FMOD/fmod.hpp"
#include "FMOD/fmod_event.hpp"

const float kDistanceScale = 50.0f;
const float kVelocityScale = 1.0f / 50.0f;

const size_t kMaxSounds = 128;
const size_t kMaxVirtualVoices = 1024;
const String kDefaultSoundPath = "sound/";

typedef FMOD_RESULT ResultT;

#define ERROR_X                                                                \
  X(FMOD_OK)                                                                   \
  X(FMOD_ERR_ALREADYLOCKED)                                                    \
  X(FMOD_ERR_BADCOMMAND)                                                       \
  X(FMOD_ERR_CDDA_DRIVERS)                                                     \
  X(FMOD_ERR_CDDA_INIT)                                                        \
  X(FMOD_ERR_CDDA_INVALID_DEVICE)                                              \
  X(FMOD_ERR_CDDA_NOAUDIO)                                                     \
  X(FMOD_ERR_CDDA_NODEVICES)                                                   \
  X(FMOD_ERR_CDDA_NODISC)                                                      \
  X(FMOD_ERR_CDDA_READ)                                                        \
  X(FMOD_ERR_CHANNEL_ALLOC)                                                    \
  X(FMOD_ERR_CHANNEL_STOLEN)                                                   \
  X(FMOD_ERR_COM)                                                              \
  X(FMOD_ERR_DMA)                                                              \
  X(FMOD_ERR_DSP_CONNECTION)                                                   \
  X(FMOD_ERR_DSP_FORMAT)                                                       \
  X(FMOD_ERR_DSP_NOTFOUND)                                                     \
  X(FMOD_ERR_DSP_RUNNING)                                                      \
  X(FMOD_ERR_DSP_TOOMANYCONNECTIONS)                                           \
  X(FMOD_ERR_FILE_BAD)                                                         \
  X(FMOD_ERR_FILE_COULDNOTSEEK)                                                \
  X(FMOD_ERR_FILE_DISKEJECTED)                                                 \
  X(FMOD_ERR_FILE_EOF)                                                         \
  X(FMOD_ERR_FILE_NOTFOUND)                                                    \
  X(FMOD_ERR_FILE_UNWANTED)                                                    \
  X(FMOD_ERR_FORMAT)                                                           \
  X(FMOD_ERR_HTTP)                                                             \
  X(FMOD_ERR_HTTP_ACCESS)                                                      \
  X(FMOD_ERR_HTTP_PROXY_AUTH)                                                  \
  X(FMOD_ERR_HTTP_SERVER_ERROR)                                                \
  X(FMOD_ERR_HTTP_TIMEOUT)                                                     \
  X(FMOD_ERR_INITIALIZATION)                                                   \
  X(FMOD_ERR_INITIALIZED)                                                      \
  X(FMOD_ERR_INTERNAL)                                                         \
  X(FMOD_ERR_INVALID_ADDRESS)                                                  \
  X(FMOD_ERR_INVALID_FLOAT)                                                    \
  X(FMOD_ERR_INVALID_HANDLE)                                                   \
  X(FMOD_ERR_INVALID_PARAM)                                                    \
  X(FMOD_ERR_INVALID_POSITION)                                                 \
  X(FMOD_ERR_INVALID_SPEAKER)                                                  \
  X(FMOD_ERR_INVALID_SYNCPOINT)                                                \
  X(FMOD_ERR_INVALID_VECTOR)                                                   \
  X(FMOD_ERR_MAXAUDIBLE)                                                       \
  X(FMOD_ERR_MEMORY)                                                           \
  X(FMOD_ERR_MEMORY_CANTPOINT)                                                 \
  X(FMOD_ERR_MEMORY_SRAM)                                                      \
  X(FMOD_ERR_NEEDS2D)                                                          \
  X(FMOD_ERR_NEEDS3D)                                                          \
  X(FMOD_ERR_NEEDSHARDWARE)                                                    \
  X(FMOD_ERR_NEEDSSOFTWARE)                                                    \
  X(FMOD_ERR_NET_CONNECT)                                                      \
  X(FMOD_ERR_NET_SOCKET_ERROR)                                                 \
  X(FMOD_ERR_NET_URL)                                                          \
  X(FMOD_ERR_NET_WOULD_BLOCK)                                                  \
  X(FMOD_ERR_NOTREADY)                                                         \
  X(FMOD_ERR_OUTPUT_ALLOCATED)                                                 \
  X(FMOD_ERR_OUTPUT_CREATEBUFFER)                                              \
  X(FMOD_ERR_OUTPUT_DRIVERCALL)                                                \
  X(FMOD_ERR_OUTPUT_ENUMERATION)                                               \
  X(FMOD_ERR_OUTPUT_FORMAT)                                                    \
  X(FMOD_ERR_OUTPUT_INIT)                                                      \
  X(FMOD_ERR_OUTPUT_NOHARDWARE)                                                \
  X(FMOD_ERR_OUTPUT_NOSOFTWARE)                                                \
  X(FMOD_ERR_PAN)                                                              \
  X(FMOD_ERR_PLUGIN)                                                           \
  X(FMOD_ERR_PLUGIN_INSTANCES)                                                 \
  X(FMOD_ERR_PLUGIN_MISSING)                                                   \
  X(FMOD_ERR_PLUGIN_RESOURCE)                                                  \
  X(FMOD_ERR_PRELOADED)                                                        \
  X(FMOD_ERR_PROGRAMMERSOUND)                                                  \
  X(FMOD_ERR_RECORD)                                                           \
  X(FMOD_ERR_REVERB_INSTANCE)                                                  \
  X(FMOD_ERR_SUBSOUND_ALLOCATED)                                               \
  X(FMOD_ERR_SUBSOUND_CANTMOVE)                                                \
  X(FMOD_ERR_SUBSOUND_MODE)                                                    \
  X(FMOD_ERR_SUBSOUNDS)                                                        \
  X(FMOD_ERR_TAGNOTFOUND)                                                      \
  X(FMOD_ERR_TOOMANYCHANNELS)                                                  \
  X(FMOD_ERR_UNIMPLEMENTED)                                                    \
  X(FMOD_ERR_UNINITIALIZED)                                                    \
  X(FMOD_ERR_UNSUPPORTED)                                                      \
  X(FMOD_ERR_UPDATE)                                                           \
  X(FMOD_ERR_VERSION)                                                          \
  X(FMOD_ERR_EVENT_FAILED)                                                     \
  X(FMOD_ERR_EVENT_INFOONLY)                                                   \
  X(FMOD_ERR_EVENT_INTERNAL)                                                   \
  X(FMOD_ERR_EVENT_MAXSTREAMS)                                                 \
  X(FMOD_ERR_EVENT_MISMATCH)                                                   \
  X(FMOD_ERR_EVENT_NAMECONFLICT)                                               \
  X(FMOD_ERR_EVENT_NOTFOUND)                                                   \
  X(FMOD_ERR_EVENT_NEEDSSIMPLE)                                                \
  X(FMOD_ERR_EVENT_GUIDCONFLICT)                                               \
  X(FMOD_ERR_EVENT_ALREADY_LOADED)                                             \
  X(FMOD_ERR_MUSIC_UNINITIALIZED)                                              \
  X(FMOD_ERR_MUSIC_NOTFOUND)                                                   \
  X(FMOD_ERR_MUSIC_NOCALLBACK)

namespace {
  void CheckError(ResultT result) {
    if (result != FMOD_OK) {
#define X(x) if (result == x) error(#x);
      ERROR_X
#undef X
    }
  }

  struct SoundImpl : public SoundT {
    FMOD::Sound* source;
    mutable FMOD::Channel* sound;
    float volume;
    float pan;
    float pitch;
    bool deleted;
    bool looped;
    bool playing;

    SoundImpl(FMOD::Sound* source) :
      source(source),
      volume(1),
      pan(0),
      pitch(1),
      deleted(false),
      looped(false),
      playing(false)
      {}

    ~SoundImpl() {
      if (sound)
        sound->setPaused(true);
    }

    void Delete() {
      deleted = true;
    }

    bool IsFinished() const {
      if (!sound)
        return true;

      bool ip;
      ResultT result = sound->isPlaying(&ip);
      if (result == FMOD_OK)
        return !ip;
      else {
        sound = nullptr;
        return true;
      }
    }

    bool IsLooped() const {
      return looped;
    }

    float GetDuration() const {
      if (!sound)
        return 0;

      uint length;
      ResultT result = source->getLength(&length, FMOD_TIMEUNIT_MS);
      if (result == FMOD_OK)
        return (float)length;
      else {
        sound = nullptr;
        return 0;
      }
    }

    float GetPan() const {
      return pan;
    }

    float GetPitch() const {
      return pitch;
    }

    float GetVolume() const {
      return volume;
    }

    void SetCursor(float position) {
      if (!sound) return;

      uint length;
      ResultT result = source->getLength(&length, FMOD_TIMEUNIT_MS);
      if (result == FMOD_OK)
        sound->setPosition(((uint)position % length), FMOD_TIMEUNIT_MS);
      else
        sound = nullptr;
    }

    void SetPan(float pan) {
      pan = Clamp(pan, -1.0f, 1.0f);
      this->pan = pan;
      if (!sound) return;

      ResultT result = sound->setPan(pan);
      if (result != FMOD_OK)
        sound = nullptr;
    }

    void SetPitch(float pitch) {
      this->pitch = pitch;
      if (!sound) return;

      ResultT result = sound->setFrequency(44100.0f * pitch);
      if (result != FMOD_OK)
        sound = nullptr;
    }

    void SetPlaying(bool playing) {
      this->playing = playing;
      if (!sound) return;

      ResultT result = sound->setPaused(!playing);
      if (result != FMOD_OK)
        sound = nullptr;
    }

    void SetVolume(float volume) {
      volume = Saturate(volume);
      this->volume = volume;
      if (!sound) return;

      ResultT result = sound->setVolume(volume);
      if (result != FMOD_OK)
        sound = nullptr;
    }
  };

  struct SoundEventImpl : public SoundEvent {
    FMOD::Event* event;

    SoundEventImpl(FMOD::Event* event) :
      event(event)
      {}

    ~SoundEventImpl() {
      event->release();
    }

    void SetParameter(int index, float value) {
      FMOD::EventParameter* param;
      CheckError(event->getParameterByIndex(index, &param));
      CheckError(param->setValue(value));
    }

    void Start() {
      CheckError(event->start());
    }

    void Stop() {
      CheckError(event->stop());
    }
  };

  struct Sound2DInstance {
    Reference<SoundImpl> sound;
    typedef Sound2DInstance SelfType;

    POOLED_TYPE
  };

  struct Sound3DInstance {
    Reference<SoundImpl> sound;
    Object carrier;
    V3 offset;

    typedef Sound3DInstance SelfType;
    POOLED_TYPE

    void Update(V3D const& camPos) {
      V3D position = carrier
        ? (V3D)carrier->GetTransform().TransformPoint(offset)
        : (V3D)offset;

      V3 relative = (V3)(position - camPos);
      V3 velocity = carrier
        ? kVelocityScale * carrier->GetVelocity()
        : 0;

      CheckError(sound->sound->set3DAttributes(
        (FMOD_VECTOR const*)&relative,
        (FMOD_VECTOR const*)&velocity));
    }
  };

  struct SoundEngineFmodImpl : public SoundEngineFmod {
    FMOD::EventSystem* eventSystem;
    FMOD::System* engine;
    V3D camPos;

    Map<String, FMOD::Sound*> sources;

    Vector<Sound2DInstance*> sounds2D;
    Vector<Sound3DInstance*> sounds3D;

    SoundEngineFmodImpl() :
      camPos(0)
    {
      CheckError(FMOD::EventSystem_Create(&eventSystem));
      CheckError(eventSystem->getSystemObject(&engine));
      CheckError(engine->setDSPBufferSize(2048, 4));
      CheckError(engine->setSoftwareChannels(kMaxSounds));

      CheckError(eventSystem->init(
        kMaxVirtualVoices,
        FMOD_INIT_HRTF_LOWPASS & FMOD_INIT_VOL0_BECOMES_VIRTUAL,
        0,
        FMOD_EVENT_INIT_USE_GUIDS
      ));

      CheckError(eventSystem->setMediaPath("../lt/resource/music/"));
    }

    ~SoundEngineFmodImpl() {
      eventSystem->release();
    }

    SoundEvent* GetEvent(char const* guid) {
      FMOD::Event* event;
      CheckError(eventSystem->getEventByGUIDString(guid, 0, &event));
      return new SoundEventImpl(event);
    }

    char const* GetName() const {
      return "SoundEngine (FMOD)";
    }

    void LoadProject(char const* name) {
      FMOD::EventProject* project;
      CheckError(eventSystem->load(name, nullptr, &project));
    }

    void Update() {
      SFRAME("Sound Engine");

      FRAME("Listener Update") {
        /* Update the sound engine's listener location. */
        Camera const& camera = Camera_Get();
        if (camera) {
          camPos = camera->GetPos();
          V3 camLook = camera->GetLook();
          V3 camUp = camera->GetUp();

          V3 listenerPos = 0;
          V3 camVelocity = camera->GetTarget()
            ? kVelocityScale * camera->GetTarget()->GetVelocity()
            : V3(0);

          engine->set3DListenerAttributes(0,
            (FMOD_VECTOR const*)&listenerPos,
            (FMOD_VECTOR const*)&camVelocity,
            (FMOD_VECTOR const*)&camLook,
            (FMOD_VECTOR const*)&camUp);
        }
      }

      FRAME("2D Sound Update") {
        for (int i = 0; i < (int)sounds2D.size(); ++i) {
          Sound2DInstance* info = sounds2D[i];
          if (info->sound->IsFinished() || info->sound->deleted) {
            info->sound->deleted = false;
            delete info;
            sounds2D.removeIndex(i);
            i--;
            continue;
          }
        }
      }

      FRAME("3D Sound Update") {
        for (int i = 0; i < (int)sounds3D.size(); ++i) {
          Sound3DInstance* info = sounds3D[i];
          if ((info->carrier && info->carrier->IsDeleted())
              || info->sound->IsFinished()
              || info->sound->deleted)
          {
            info->sound->deleted = false;
            delete info;
            sounds3D.removeIndex(i);
            i--;
            continue;
          }

          info->Update(camPos);
        }
      }

      FRAME("Engine Update") {
        CheckError(engine->update());
      }
    }

    SoundImpl* CreateSound(
      String const& file,
      bool spatialized,
      bool looped)
    {
      String path = kDefaultSoundPath + file;
      if (!sources.contains(path)) {
        AutoPtr< Array<uchar> > arr = Location_Resource(path)->Read();
        if (!arr)
          Log_Critical("Failed to read sound " + file);
        FMOD::Sound* source;
        FMOD_CREATESOUNDEXINFO info;
        memset(&info, 0, sizeof(info));
        info.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
        info.length = arr->size();

        CheckError(engine->createSound(
          (char const*)arr->data(),
          FMOD_OPENMEMORY | FMOD_SOFTWARE |
          (looped ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF) |
          (spatialized ? FMOD_3D : FMOD_2D),
          &info,
          &source));
        sources[path] = source;
      }

      FMOD::Sound* source = sources[path];
      SoundImpl* s = new SoundImpl(source);

      CheckError(engine->playSound(
        FMOD_CHANNEL_FREE,
        source,
        true,
        &s->sound));

      CheckError(s->sound->setLoopCount(looped ? -1 : 0));
      s->looped = looped;
      return s;
    }

    Sound Play(Array<float> const& buffer) {
      FMOD::Sound* source;
      FMOD_CREATESOUNDEXINFO createSoundInfo;
      memset(&createSoundInfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
      createSoundInfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
      createSoundInfo.length = buffer.size() * sizeof(float);
      createSoundInfo.numchannels = 1;
      createSoundInfo.defaultfrequency = 44100;
      createSoundInfo.format = FMOD_SOUND_FORMAT_PCMFLOAT;

      CheckError(engine->createSound(
        (const char*)buffer.data(),
        FMOD_OPENMEMORY | FMOD_OPENRAW | FMOD_SOFTWARE,
        &createSoundInfo,
        &source));

      Sound2DInstance* info = new Sound2DInstance;
      SoundImpl* s = new SoundImpl(source);

      CheckError(engine->playSound(FMOD_CHANNEL_FREE, source, true, &s->sound));

      info->sound = s;
      info->sound->SetPlaying(true);
      sounds2D << info;
      return s;
    }

    Sound Play2D(String const& name, float volume, bool looped) {
      Sound2DInstance* info = new Sound2DInstance;
      info->sound = CreateSound(name, false, looped);
      info->sound->SetVolume(volume);
      info->sound->SetPlaying(true);
      sounds2D << info;
      return info->sound;
    }

    Sound Play3D(
      String const& name,
      Object const& carrier,
      V3 const& offset,
      float volume,
      float distanceDiv,
      bool looped)
    {
      Sound3DInstance* info = new Sound3DInstance;
      info->carrier = carrier;
      info->offset = offset;
      info->sound = CreateSound(name, true, looped);
      info->sound->SetVolume(volume);
      CheckError(info->sound->sound->set3DMinMaxDistance(
        kDistanceScale * distanceDiv, 100000));
      info->Update(camPos);
      info->sound->SetPlaying(true);
      sounds3D << info;
      return info->sound;
    }
  };
}

SoundEngine* SoundEngine_Fmod() {
  return new SoundEngineFmodImpl;
}
