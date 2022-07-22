#ifndef AI_NLPPhrase_h__
#define AI_NLPPhrase_h__

#include "Types.h"
#include "LTE/Common.h"
#include "LTE/Vector.h"
#include "LTE/AutoPtr.h"
#include "LTE/String.h"

struct NLPOutputState {
  String output;
  bool newSentence;
  bool requiresSpace;

  NLPOutputState() :
    newSentence(true),
    requiresSpace(false)
    {}
};

struct NLPToken {
  virtual ~NLPToken() {}
  virtual void ToString(NLPOutputState& state) const = 0;
};

struct NLPPhrase {
  NLPContext& context;
  Vector<AutoPtr<NLPToken> > tokens;

  NLPPhrase(NLPContext& context) :
    context(context)
    {}

  NLPPhrase& operator<<(AutoPtr<NLPToken> token) {
    tokens.push(token);
    return *this;
  }

  LT_API String ToString() const;
};

LT_API AutoPtr<NLPToken> NLPTokenFullstop();
LT_API AutoPtr<NLPToken> NLPTokenNoun(const String& noun, bool proper);
LT_API AutoPtr<NLPToken> NLPTokenText(const String& text);

LT_API void NLPQualifierTime(NLPPhrase& phrase);

#endif
