#ifndef Audio_IO_h__
#define Audio_IO_h__

#include "Common.h"

namespace Audio {

  LT_API void WAV_Write(
    char const* outFile,
    Array<int> const& buf,
    int sampleRate,
    short channels);

}

#endif
