#include "LTE/Function.h"
#include "LTE/StringTree.h"

TypeAlias(Reference<StringTreeT>, StringTree);

FreeFunction(StringTree, StringTree_Add,
  "Add 'child' to 'tree'",
  StringTree, tree,
  StringTree, child)
{
  ((StringTree&)tree)->children.push(child);
  return tree;
} FunctionAlias(StringTree_Add, Add);

FreeFunction(StringTree, StringTree_Child,
  "Get the child at 'index' in 'tree'",
  StringTree, tree,
  int, index)
{
  return tree->children[index];
} FunctionAlias(StringTree_Child, Child);

FreeFunction(int, StringTree_Children,
  "Return the number of children in 'tree'",
  StringTree, tree)
{
  return tree->children.size();
} FunctionAlias(StringTree_Children, Children);

FreeFunction(StringTree, StringTree_Create,
  "Create a new StringTree by parsing 'contents'",
  String, contents)
{
  return StringTree_Create(contents);
}

FreeFunction(String, StringTree_Value,
  "Return the value contained in 'tree'",
  StringTree, tree)
{
  return tree->value;
} FunctionAlias(StringTree_Value, Value);
