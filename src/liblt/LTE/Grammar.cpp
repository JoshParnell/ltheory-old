#include "Grammar.h"
#include "AutoPtr.h"
#include "Location.h"
#include "Pointer.h"
#include "ProgramLog.h"
#include "RNG.h"
#include "Vector.h"
#include "VectorMap.h"

#include <sstream>

namespace {
  bool IsToken(String const& str) {
    static String symbols("abcdefghijklmnopqrstuvwxyz01234546789");
    for (size_t i = 0; i < str.size(); ++i)
      if (symbols.contains((char)tolower(str[i])))
        return true;
    return false;
  }

  String RemoveLeadingWS(String str) {
    static String whitespace(" \t");
    while (str.size() && whitespace.contains(str.front()))
      str.erase(str.begin());
    return str;
  }
}

namespace LTE {
  struct GrammarRuleImpl : public GrammarRule {
    Vector< AutoPtr<GrammarPhrase> > entries;

    GrammarRule* Add(GrammarPhrase* phrase) {
      entries.push(phrase);
      return this;
    }

    GrammarPhrase* Select(
      Vector<String> const& tags,
      RNG const& rg) const
    {
      float totalWeight = 0;
      Vector<uint> choices;
      Vector<float> cumulative;

      for (size_t i = 0; i < entries.size(); ++i) {
        if (entries[i]->Matches(tags)) {
          totalWeight += entries[i]->weight;
          choices.append(i);
          cumulative.append(totalWeight);
        }
      }

      if (!choices.size()) {
        Log_Error(Stringize("No matching production for tags ") | tags);
        return nullptr;
      }

      float chosenWeight = rg->GetFloat(0, totalWeight);
      for (size_t i = 0; i < choices.size(); ++i)
        if (chosenWeight <= cumulative[i])
          return entries[choices[i]];
      return entries.back();
    }
  };

  struct GrammarImpl : public Grammar {
    VectorMap< String, AutoPtr<GrammarRule> > rules;

    Grammar* Add(GrammarRule* rule, String const& name) {
      rules[name] = rule;
      if (((GrammarRuleImpl*)rule)->entries.empty())
        Log_Warning("Empty grammar rule <" + name + ">");
      return this;
    }

    String Generate(
      RNG const& rg,
      String const& token,
      String const& tags)
    {
      Vector<String> tagVector;
      String_Split(tagVector, tags, ',');
      for (size_t i = 0; i < tagVector.size(); ++i)
        tagVector[i] = String_Lower(tagVector[i]);
      return Parse(token, tagVector, rg);
    }

    String Parse(
      String const& tok,
      Vector<String> const& tags,
      RNG const& rg)
    {
      if (!tok.size())
        return "";

      if (tok.front() == '$') {
        GrammarRule* rule = rules[tok.substr(1)];
        if (!rule) {
          Log_Error("No match for grammar rule <" + tok + ">");
          return "";
        }

        String buffer;
        GrammarPhrase* phrase = rule->Select(tags, rg);
        if (phrase)
          for (size_t i = 0; i < phrase->tokens.size(); ++i)
            buffer += Parse(phrase->tokens[i], tags, rg);
        else
          Log_Error("No phrase selected for rule <" + tok + ">");
        return buffer;
      } else
        return tok;
    }
  };

  Grammar* Grammar::Load(Location const& location) {
    Grammar* grammar = Grammar_Create();

    std::stringstream stream(location->ReadAscii());
    String ruleName;
    Pointer<GrammarRule> currentRule;

    String buffer;

    while (getline(stream, buffer)) {
      if (!currentRule) {
        if (IsToken(buffer)) {
          ruleName = RemoveLeadingWS(buffer);
          currentRule = Grammar_CreateRule();
        }
      } else {
        if (IsToken(buffer)) {
          String thisRule = RemoveLeadingWS(buffer);
          Vector<String> tokens;
          String_Split(tokens, thisRule, '|');

          if (tokens.size() < 2) {
            Log_Error(Stringize() |
              "Rule <" | ruleName | "> contains a malformed production");
            continue;
          }

          float weight = FromString<float>(tokens[0]);
          GrammarPhrase* thisPhrase = new GrammarPhrase(weight);

          for (size_t i = 1; i < tokens.size(); ++i) {
            if (!tokens[i].size())
              continue;
            if (tokens[i].front() == '#')
              thisPhrase->tags.push(String_Lower(tokens[i].substr(1)));
            else
              thisPhrase->tokens.push(tokens[i]);
          }
          currentRule->Add(thisPhrase);
        } else {
          grammar->Add(currentRule, ruleName);
          currentRule = nullptr;
        }
      }
    }

    if (currentRule)
      grammar->Add(currentRule, ruleName);

    return grammar;
  }

  Grammar* Grammar_Get() {
    static AutoPtr<Grammar> g;
    return g ? g : g = Grammar::Load(
      Location_Resource("gamedata/grammar/default.txt"));
  }

  Grammar* Grammar_Create() {
    return new GrammarImpl;
  }

  GrammarRule* Grammar_CreateRule() {
    return new GrammarRuleImpl;
  }
}
