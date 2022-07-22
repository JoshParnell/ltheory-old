#include "StringTree.h" 
#include "Location.h"
#include "HashMap.h"
#include "Map.h"
#include "ProgramLog.h"
#include "Tokenizer.h"

#include "Debug.h"

StringTree StringTree_Create(Tokenizer& tokenizer, int& lineNumber) {
  String const kWhitespace = " \t\n\r";
  String const kTab = " ";

  String lineString = tokenizer.ReadLine();
  uint indent = Tokenizer::GetIndent(lineString, kTab);

  Tokenizer line(lineString);

  line.ReadSubset(kWhitespace);
  String value = line.ReadToken(kWhitespace, "");
  if (!value.size())
    return nullptr;

  StringTree tree = new StringTreeT;
  tree->value = value;
  tree->line = lineNumber;

  /* Inline children. */
  while (line.HasMore()) {
    StringTree child = StringTree_Create(
      line.ReadToken(kWhitespace, "", "(", ")"), lineNumber);
    if (child)
      tree->children.push(child);
  }

  /* Indented children. */
  while (tokenizer.HasMore()) {
    size_t cursor = tokenizer.GetCursor();
    String thisLine = tokenizer.ReadLine();
    tokenizer.SetCursor(cursor);

    uint lineIndent = Tokenizer::GetIndent(thisLine, kTab);
    if (lineIndent <= indent && !thisLine.containsOnly(kWhitespace))
      break;

    StringTree child = StringTree_Create(tokenizer, lineNumber);
    if (child)
      tree->children.push(child);
  }

  return tree;
}

StringTree StringTree_Create(String const& source, int& line) {
  Tokenizer tokenizer(source);
  return StringTree_Create(tokenizer, line);
}

StringTree StringTree_Load(Location const& location) {
  return StringTree_Create(location->ReadAscii());
}
