const size_t kMaxFunctionParams = 8;

namespace CodeGen {
  void Generate_Function(
    CodeBlock& output,
    size_t paramCount,
    bool member,
    bool voidfn)
  {
    CodeBlock declBlock;
    CodeBlock callBlock;
    CodeBlock metaBlock;

    Vector<Variable> functionParams;
    /* Decl. */ {
      for (size_t i = 0; i < paramCount; ++i)
        functionParams.push(Variable(
          Stringize() | "T" | i | " const&",
          Stringize() | "N" | i));

      declBlock << Function("Name##_Impl", "LT_API Prefix ReturnType", functionParams);
      declBlock.lines.pop();
      declBlock.lines.back().pop();
      declBlock.lines.back() += " Postfix;";
      declBlock.NewLine();
    }

    /* Caller. */ {
      /* Call Function. */ {
        CodeBlock block;
        if (!voidfn)
          block << "*(ReturnType*)out = ";
        if (member)
          block << "((SelfType*)in[0])->";

        /* Call. */ {
          Vector<String> args;
          for (size_t i = 0; i < paramCount; ++i) {
            size_t index = member ? i + 1 : i;
            args.push(Stringize() | "*(T" | i | "*)in[" | index | "]");
          }
          block << FunctionCall("Name##_Impl", args);
        }

        Vector<Variable> vars;
        vars.push(Variable("void**", "in"));
        vars.push(Variable("void*", "out"));

        callBlock << Function(
          "Name##_Call",
          member ? "static void" : "inline void",
          vars, block);
      }
    }

    /* Metadata */ {
      CodeBlock functionBlock;
      functionBlock << "static FunctionType* fn = nullptr;";
      functionBlock << "if (!fn) {";

      CodeBlock initBlock;
      initBlock << "fn = new FunctionType(#Name);";
      initBlock << "fn->call = Name##_Call;";
      initBlock << "fn->description = Description;";

      size_t params = member ? paramCount + 1 : paramCount;
      initBlock << "fn->paramCount = " << params << ";";

      if (!params)
        initBlock << "fn->params = nullptr;";
      else
        initBlock << "fn->params = new Parameter[" << params << "];";

      if (member)
        initBlock
          << "Mutable(fn->params[0]) = "
          << "Parameter(\"object\", nullptr);";

      for (size_t i = 0; i < paramCount; ++i) {
        size_t index = member ? i + 1 : i;
        initBlock
          << "Mutable(fn->params[" << index << "]) = "
          << "Parameter(#N" << i << ", Type_Get<T" << i << ">());";
      }

      initBlock << "fn->returnType = Type_Get<ReturnType>();";
      initBlock.Indent();

      functionBlock << initBlock;
      functionBlock << "}";
      functionBlock << "return fn;";

      metaBlock << "template <int unused>";
      metaBlock << Function(
        "Name##_GetMetadata",
        Stringize() | (member ? "static " : "") | "FunctionType const*",
        functionBlock);
    }

    if (!member) {
      metaBlock
        << "static FunctionType const* Name##_Metadata "
        << "= Name##_GetMetadata<0>();";
    }

    String fnName = member ? "MemberFunction" : "FreeFunction";
    if (voidfn)
      fnName = "Void" + fnName;

    Vector<String> params;
    params.push("Prefix");
    params.push("Postfix");
    params.push("ReturnType");
    params.push("Name");
    params.push("Description");
    for (size_t i = 0; i < kMaxFunctionParams; ++i) {
      params.push(Stringize() | "T" | i);
      params.push(Stringize() | "N" | i);
    }

    /* Inline macro. */ {
      CodeBlock inlineBlock;
      if (member) {
        inlineBlock << callBlock;
        inlineBlock << metaBlock;
      } else {
        inlineBlock << declBlock;
        inlineBlock << callBlock;
        inlineBlock << metaBlock;
      }

      inlineBlock << Function("Name##_Impl", "Prefix ReturnType", functionParams);
      inlineBlock.lines.pop();
      inlineBlock.lines.back().pop();
      inlineBlock.lines.back() += " Postfix ";

      output << Macro(Stringize() | fnName | paramCount, params, inlineBlock);
    }
  }

  void Generate_FunctionMacros(
    CodeBlock& output,
    bool isVoid,
    bool isMember,
    bool isConst,
    bool isVirtual,
    bool noParams)
  {
    String name = isMember ? "MemberFunction" : "FreeFunction";
    if (isVoid)
      name = "Void" + name;
    String callName = name;

    if (isVirtual)
      name = "Virtual" + name;
    if (isConst)
      name = "Const" + name;
    if (noParams)
      name += "NoParams";

    String wrapperName = "_" + name;

    /* Helper Fn. */ {
      Vector<String> args;
      args.push("RT");
      args.push("Name");
      args.push("Desc");

      for (uint i = 0; i < kMaxFunctionParams; ++i) {
        args.push(Stringize() | "T" | i);
        args.push(Stringize() | "N" | i);
      }
      args.push("x");
      args.push("...");

      Vector<String> callArgs;
      callArgs.push(isVirtual ? "virtual" : "");
      callArgs.push(isConst ? "const" : "");
      callArgs.push("RT");
      callArgs.push("Name");
      callArgs.push("Desc");

      for (uint i = 0; i < kMaxFunctionParams; ++i) {
        callArgs.push(Stringize() | "T" | i);
        callArgs.push(Stringize() | "N" | i);
      }

      CodeBlock block;
      block << "MACRO_IDENTITY(";
      block << FunctionCall(callName + "##x", callArgs).Macro();
      block << ")";
      block.Join().Join();

      output << Macro(wrapperName, args, block);
    }

    /* Main Fn. */ {
      Vector<String> helperArgs;
      helperArgs.push(isVoid ? "void " : "RT");
      helperArgs.push("Name");
      helperArgs.push("Desc");

      if (!noParams)
        helperArgs.push("__VA_ARGS__");

      for (uint i = 0; i <= kMaxFunctionParams; ++i) {
        String number = ToString(kMaxFunctionParams - i);
        helperArgs.push(number);
        helperArgs.push(number);
      }

      CodeBlock block;
      block << "MACRO_IDENTITY(";
      block << FunctionCall(wrapperName, helperArgs).Macro();
      block << ")";
      block.Join().Join();

      Vector<String> args;
      if (!isVoid)
        args.push("RT");
      args.push("Name");
      args.push("Desc");

      if (!noParams)
        args.push("...");
      output << Macro(name, args, block);
    }
  }

  Reference<CodeBlock> Functions() {
    CodeBlock* block = new CodeBlock;
    for (size_t i = 0; i <= kMaxFunctionParams; ++i) {
      Generate_Function(*block, i, false, false);
      Generate_Function(*block, i, true, false);
      Generate_Function(*block, i, false, true);
      Generate_Function(*block, i, true, true);
    }

    Generate_FunctionMacros(*block, 0, 0, 0, 0, 0);
    Generate_FunctionMacros(*block, 1, 0, 0, 0, 0);
    Generate_FunctionMacros(*block, 0, 1, 0, 0, 0);
    Generate_FunctionMacros(*block, 1, 1, 0, 0, 0);
    Generate_FunctionMacros(*block, 0, 1, 1, 0, 0);
    Generate_FunctionMacros(*block, 1, 1, 1, 0, 0);
    Generate_FunctionMacros(*block, 0, 1, 0, 1, 0);
    Generate_FunctionMacros(*block, 1, 1, 0, 1, 0);
    Generate_FunctionMacros(*block, 0, 1, 1, 1, 0);
    Generate_FunctionMacros(*block, 1, 1, 1, 1, 0);
    Generate_FunctionMacros(*block, 0, 0, 0, 0, 1);
    Generate_FunctionMacros(*block, 1, 0, 0, 0, 1);
    Generate_FunctionMacros(*block, 0, 1, 0, 0, 1);
    Generate_FunctionMacros(*block, 1, 1, 0, 0, 1);
    Generate_FunctionMacros(*block, 0, 1, 1, 0, 1);
    Generate_FunctionMacros(*block, 1, 1, 1, 0, 1);
    Generate_FunctionMacros(*block, 0, 1, 0, 1, 1);
    Generate_FunctionMacros(*block, 1, 1, 0, 1, 1);
    Generate_FunctionMacros(*block, 0, 1, 1, 1, 1);
    Generate_FunctionMacros(*block, 1, 1, 1, 1, 1);
    return block;
  }
}
