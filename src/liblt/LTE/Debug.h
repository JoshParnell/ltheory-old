#ifndef LTE_Debug_h__
#define LTE_Debug_h__

#include "String.h"

#include <iostream>

#define DBG(level, output) { if (Debug_GetLevel() >= level) { dbg | output; } }

namespace LTE {
  LT_API int Debug_GetLevel();
  LT_API void Debug_SetLevel(int level);

  struct _Newline     {} static endl;
  struct _Marker      {} static marker;
  struct _Indent      {} static shiftr;
  struct _Unindent    {} static shiftl;
  struct _WaitKey     {} static waitkey;

  struct DebugPrint {
    int indentLevel;
    int markerCount;
    bool newLine;
    bool silent;

    DebugPrint() :
      indentLevel(0),
      markerCount(0),
      newLine(true),
      silent(false)
      {}

    template <class T>
    DebugPrint& operator|(T const& t) {
      if (silent) return *this;

      if (newLine) {
        newLine = false;
        for (int i = 0; i < indentLevel; ++i)
          std::cout << "  ";
      }
      
      ToStream(std::cout, t);
      std::cout << std::flush;
      return *this;
    }

    DebugPrint& operator|(_Newline const& n) {
      if (silent) return *this;
      std::cout << std::endl;
      newLine = true;
      return *this;
    }

    DebugPrint& operator|(_Indent const& i) {
      if (silent) return *this;
      indentLevel++;
      return *this;
    }

    DebugPrint& operator|(_Marker const& m) {
      if (silent) return *this;
      markerCount++;
      std::cout << "[Marker #" << markerCount << "]" << std::flush;
      return *this;
    }

    DebugPrint& operator|(_Unindent const& i) {
      if (silent) return *this;
      indentLevel--;
      return *this;
    }

    DebugPrint& operator|(_WaitKey const& w) {
      if (silent) return *this;
      getchar();
      return *this;
    }

    friend void StreamIndent(DebugPrint& s, int level) {
      if (!s.silent)
        s.indentLevel += level;
    }
  };

  struct DebugPrintNull {
    bool silent;

    template <class T>
    DebugPrintNull& operator|(T const& t) {
      return *this;
    }
  };

#ifdef NO_OUTPUT
  static DebugPrintNull dbg;
#else
  static DebugPrint dbg;
#endif

  struct ScopedIndent {
    ScopedIndent() {
      dbg | shiftr;
    }

    ~ScopedIndent() {
      dbg | shiftl;
    }

    void Dummy() {
      dbg | marker | endl | waitkey;
    }
  };

  struct ScopedSuppress {
    ScopedSuppress() {
      dbg.silent = true;
    }

    ~ScopedSuppress() {
      dbg.silent = false;
    }
  };
}

#endif
