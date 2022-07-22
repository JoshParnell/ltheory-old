#include "NLPPhrase.h"
#include "NLPContext.h"

String NLPPhrase::ToString() const {
  NLPOutputState state;
  for (size_t i = 0; i < tokens.size(); ++i)
    tokens[i]->ToString(state);
  return state.output;
}

AutoPtr<NLPToken> NLPTokenFullstop() {
  struct NLPTokenFullstopImpl : public NLPToken {
    void ToString(NLPOutputState& state) const {
      state.output += '.';
      state.newSentence = true;
      state.requiresSpace = true;
    }
  };
  return new NLPTokenFullstopImpl;
}

AutoPtr<NLPToken> NLPTokenNoun(const String& noun, bool proper) {
  struct NLPTokenNounImpl : public NLPToken {
    String noun;
    bool proper;

    NLPTokenNounImpl(const String& noun, bool proper) :
      noun(noun),
      proper(proper)
    {
      if (proper)
        this->noun.front() = (char)toupper(this->noun.front());
    }

    void ToString(NLPOutputState& state) const {
      if (state.requiresSpace == true)
        state.output += ' ';
      char first = (char)tolower(noun.front() );
      if (first == 'a' || first == 'e' || first == 'i' || first == 'o' ||
          first == 'u' || first == 'y')
        state.output += "an ";
      else
        state.output += "a ";
      state.output += noun;
      state.requiresSpace = true;
    }
  };
  return new NLPTokenNounImpl(noun, proper);
}

AutoPtr<NLPToken> NLPTokenText(const String& text) {
  struct NLPTokenTextImpl : public NLPToken {
    String text;

    NLPTokenTextImpl(const String& text) :
      text(text)
      {}

    void ToString(NLPOutputState& state) const {
      String buf = text;
      if (state.requiresSpace == true)
        state.output += ' ';

      if (state.newSentence && buf.size()) {
        buf.front() = (char)toupper(buf.front());
        state.newSentence = false;
      }
      state.output += buf;
      state.requiresSpace = true;
    }
  };
  return new NLPTokenTextImpl(text);
}

void NLPQualifierTime(NLPPhrase& phrase) {
  int choice = rand() % 3;
  if (choice == 0)
    phrase << NLPTokenText("for a long time");
  if (choice == 1)
    phrase << NLPTokenText("for ages");
  if (choice == 2)
    phrase << NLPTokenText("for as long as")
           << NLPTokenText(phrase.context.selfPronoun)
           << NLPTokenText("can remember");
}

