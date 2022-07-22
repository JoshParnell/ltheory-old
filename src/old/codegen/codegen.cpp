#include "LTE/LTE.h"

#include "CodeBlock.h"
#include "Constructor.h"
#include "Class.h"
#include "Function.h"
#include "FunctionCall.h"
#include "Macro.h"
#include "Variable.h"

#include "GenerateAutoClass.h"
#include "GenerateFunction.h"

int main(int argc, char const* argv[]) {
  CodeGen::AutoClasses()->SaveTo("AutoClass.h");
  CodeGen::Functions()->SaveTo("Functions.h");
  return 0;
}
