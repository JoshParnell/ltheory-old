#define LTE_CONSOLE
#include "LTE/LTE.h"

#include "Module/SoundEngine.h"

#include "Audio/Audio.h"
using namespace Audio;

double kFrequencyTable[] = {
  440.00, 466.16, 493.88, 523.25, 554.37, 587.33,
  622.25,  659.26, 698.46, 739.99, 783.99, 830.61,
};

int kMinorScale[] = { 0, 2, 3, 5, 7, 8, 10 };
int kMinorPentaScale[] = { 0, 3, 5, 7, 10 };

inline double noteFrequency(int index) {
  index -= 57;
  double octave = 1.;
  while (index < 0) {
    index += 12;
    octave /= 2.;
  }
  while (index >= 12) {
    index -= 12;
    octave *= 2.;
  }
  return kFrequencyTable[index] * octave;
}

inline double Quantize(double s, double step) {
  return Floor(s / step) * step;
}

inline double noise(double t) {
  return Fract(Sin(t) * 4437.37158237193);
}

inline double cnoise(double t) {
  double f = Floor(t);
  double i = t - f;
  return Mix(noise(f), noise(f + 1.), i);
}

inline double shape(double t) {
  double x = t * .686306;
  double a = 1 + Exp(-.75 * Sqrt(Abs(x)));
  return (Exp(x) - Exp(-x*a)) / (Exp(x) + Exp(-x));
}

struct Glass {
  double operator()(double f, double t) const {
    double s = 1. / f;
    return s * 4.0 * t * Exp(-4.0 * t * f);
  }
};

struct MySynth {
  double operator()(double f, double t) const {
    return (1. / (f * f)) * (Exp(-4. * t  * f) + Exp(-16. * t * f));
  }
};

template <class AmplitudeType>
struct AdditiveSynthType : public GeneratorT {
  AmplitudeType amplitude;

  AdditiveSynthType(AmplitudeType const& amplitude) :
    amplitude(amplitude)
    {}

  double Step(double i) const {
    return 2;
  }

  double Get(Note const& note, double t) const {
    double lt = kTau * note.frequency * t;
    double s = 0;
    double i = 1;
    for (double f = 1; f * note.frequency < 44100.0 / 2.0; f += Step(i++))
      s += Sin(lt * f) * amplitude(f, t);
    return s;
  }
};

template <class T>
GeneratorT* AdditiveSynth(const T& t) {
  return new AdditiveSynthType<T>(t);
}

struct Sine : public GeneratorT {
  double Get(Note const& note, double t) const {
    t *= note.frequency;
    return Sin(kTau * t);
  }
};

struct IdealSquare : public GeneratorT {
  double Get(Note const& note, double t) const {
    return Fract(t * note.frequency) > 0.5 ? 1.0 : -1.0;
  }
};

struct Wind : public GeneratorT {
  double Get(Note const& note, double t) const {
    return 2.0 * Fractal(ValueNoise1D, (float)(t * 440.0), 12, 1.8f) - 1.0;
  }
};

struct SoundStudio : public Program {
  SoundStudio() {
    window.width = 480;
    window.height = 480;
    window.title = "LT SoundStudio";
    Renderer_Initialize();
  }

  void Play(const int& note, UIClickData const& d) {
    #if 1
    Pattern p;
    p
      << Note(0, noteFrequency(note + 3), 1000000)
      << Note(0, noteFrequency(note + 10), 1000000)
      << Note(50000, noteFrequency(note + 0), 100000)
      << Note(100000, noteFrequency(note + 7), 100000)
      << Note(150000, noteFrequency(note + 10), 100000);

    Signal s = Signal_Instrument(AdditiveSynth(Glass()), p);
    s = Signal_Sum(s, Signal_Instrument(new Wind, Pattern(Note(0, 0, 1000000))));
    #else
    Signal s = Signal_Instrument(
      AdditiveSynth(MySynth()),
      Pattern(Note(0, noteFrequency(note), 100000)));


    #endif

    s = Signal_Compress(s, 1.0);

    for (int i = 0; i < 10; ++i)
      s = Signal_Sum(s, Signal_Delay(s, 5761 + rand() % 25000, 0.5, 0.5));

    AutoPtr<Array<float>> buf = Signal_Render(s, 8);

    Array<int> iBuf(buf->size());
    for (size_t i = 0; i < buf->size(); ++i)
      iBuf[i] = (int)(INT_MAX * (*buf)[i]);
    WAV_Write("data.wav", iBuf, 44100, 1);
    GetSoundEngine()->Play(*buf);
  }

  void Initialize() {
    window.setVerticalSyncEnabled(true);
    Mouse_SetVisible(false);

    modules.push(UIManager_Create());
    modules.push(SoundEngine_Fmod());

    Widget* studio = CreateUIGroupY()
      ->Add(CreateUILabel("SOUNDSTUDIO", 1, true))
      ->Add(CreateUIDivider());

    Widget* noteGrid = CreateUIGroupY();
    for (int i = 0; i < 6; ++i) {
      Widget* noteRow = CreateUIGroupX();
      for (int j = 0; j < 7; ++j) {
        int n = 20 + i * 12 + kMinorScale[j];
        noteRow->Add(CreateUIPanel()
          ->SetSize(48)
          ->MakeMouseListener(Bind(MemberFn(this, &SoundStudio::Play), n)));
      }
      noteGrid->Add(noteRow);
    }

    studio->Add(noteGrid);
    UIManager::Get().Add(CreateUIScrollPanel(studio));
    modules.push(&UIManager::Get());
  }

  void Update() {
    if (Keyboard_Pressed(Key_Q))
      exit = true;
  }
};

int main(int argc, char const* argv[]) {
  SoundStudio().Start();
  return 0;
}
