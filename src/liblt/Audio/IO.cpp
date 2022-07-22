#include "IO.h"

#include "LTE/Array.h"

#include <fstream>

namespace Audio {
  template <class T>
  void WriteData(std::ofstream& stream, const T& t) {
    stream.write((char const*)&t, sizeof(T));
  }

  template <class SampleType>
  void WAV_Write_Template(
    char const* outFile,
    Array<SampleType> const& buf,
    int sampleRate,
    short channels)
  {
    size_t bufSize = buf.size() * sizeof(SampleType);
    std::ofstream stream(outFile, std::ios::binary);
    stream.write("RIFF", 4);
    WriteData<int>(stream, 36 + bufSize);
    stream.write("WAVE", 4);

    stream.write("fmt ", 4);
    WriteData<int>(stream, 16);

    /* Format (1 = PCM). */
    WriteData<short>(stream, 1);

    /* Channels. */
    WriteData<short>(stream, channels);

    /* Sample rate. */
    WriteData<int>(stream, sampleRate);

    /* Byte rate. */
    WriteData<int>(stream, sampleRate * channels * sizeof(SampleType));

    /* Frame size. */
    WriteData<short>(stream, channels * sizeof(SampleType));

    /* Bits per sample. */
    WriteData<short>(stream, 8 * sizeof(SampleType));

    stream.write("data", 4);
    stream.write((char const*)&bufSize, 4);
    stream.write((char const*)buf.data(), bufSize);
  }

  void WAV_Write(
    char const* outFile,
    Array<int> const& buf,
    int sampleRate,
    short channels)
  {
    WAV_Write_Template(outFile, buf, sampleRate, channels);
  }
}
