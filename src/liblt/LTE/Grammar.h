#ifndef LTE_Grammar_h__
#define LTE_Grammar_h__

#include "String.h"
#include "Vector.h"

namespace LTE {
  struct GrammarPhrase {
    Vector<String> tags;
    Vector<String> tokens;
    float weight;

    GrammarPhrase(float weight) :
      weight(weight)
      {}

    bool Matches(Vector<String> const& tags) const {
      for (size_t i = 0; i < this->tags.size(); ++i)
        if (!tags.contains(this->tags[i]))
          return false;
      return true;
    }
  };

  struct GrammarRule {
    virtual ~GrammarRule() {}

    virtual GrammarRule* Add(GrammarPhrase* phrase) = 0;

    virtual GrammarPhrase* Select(
      Vector<String> const& tags,
      RNG const& rg) const = 0;
  };

  struct Grammar {
    virtual ~Grammar() {}

    virtual Grammar* Add(GrammarRule* rule, String const& name) = 0;

    LT_API static Grammar* Load(Location const& location);

    virtual String Generate(
      RNG const& rg,
      String const& token,
      String const& tags = "") = 0;
  };

  LT_API Grammar* Grammar_Create();
  LT_API GrammarRule* Grammar_CreateRule();
  LT_API Grammar* Grammar_Get();
}

#endif
