const uint kMaxFields = 16;

namespace CodeGen {
  void Generate_AutoClass(CodeBlock& block, uint fieldCount) {
    CodeBlock macroBlock;

    CodeBlock classBlock;
    classBlock << "typedef name SelfType;";
    classBlock << "typedef base BaseType;";
    classBlock << "DefineMetadataInline(name)";

    for (uint i = 0; i < fieldCount; ++i)
      classBlock << Variable(Stringize() | "T" | i, Stringize() | "n" | i);
    classBlock.NewLine();

    /* Constructors. */ {
      Vector<Variable> params;
      Vector<Initializer> inits;
      for (uint i = 0; i < fieldCount; ++i) {
        String varName = Stringize() | "n" | i;
        params.push(Variable(Stringize() | "T" | i | " const&", varName));
        inits.push(Initializer(varName, varName));
      }
      classBlock << Constructor("name", params, inits,
                                CodeBlock("Initializer(0);"));
    }

    classBlock << Function("Initializer", "void", Variable("void*", ""),
                           CodeBlock(" "));

    /* Getters. */
    for (uint i = 0; i < fieldCount; ++i)
      classBlock << Function(Stringize() | "Get##n" | i,
                             Stringize() | "T" | i,
                             CodeBlock(Stringize() | "return n" | i | ";")).Const();

    /* MapFields. */ {
      CodeBlock block;
      block << "SelfType* self = (SelfType*)b;";
      for (uint i = 0; i < fieldCount; ++i)
        block << "static Type type" << i
              << " = Type_Get(self->n" << i << ");";
      for (uint i = 0; i < fieldCount; ++i)
        block << "m(&self->n" << i << ", #n" << i << ", type" << i << ", aux);";
      block << "base::MapFields(b, m, aux);";

      Vector<Variable> vars;
      vars.push(Variable("TypeT*", "type"));
      vars.push(Variable("void*", "b"));
      vars.push(Variable("FieldMapper&", "m"));
      vars.push(Variable("void*", "aux"));
      classBlock << Function("MapFields", "static void", vars, block);
    }

    /* ToStream. */ {
      CodeBlock block;
      block << "ToStream(s, #name);";
      block << "ToStream(s, \"(\");";
      for (uint i = 0; i < fieldCount; ++i) {
        if (i)
          block << "ToStream(s, \", \");";
        block << "ToStream(s, t.n" << i << ");";
      }
      block << "ToStream(s, \")\");";
      classBlock << "template <class StreamT>";
      classBlock << Function("_ToStream",
                             "friend void",
                             Vector<Variable>(Variable("StreamT&", "s"),
                                              Variable("name const&", "t")),
                             block);
    }

    macroBlock << Class("name", "base", classBlock);
    macroBlock.lines.pop();
    macroBlock.lines.pop();
    
    Vector<String> macroParams;
    macroParams.push("name");
    macroParams.push("base");

    for (size_t i = 0; i < kMaxFields; ++i) {
      macroParams.push(Stringize() | "T" | i);
      macroParams.push(Stringize() | "n" | i);
    }

    block << Macro(Stringize() | "AUTOCLASS" | (2 * fieldCount),
                   macroParams,
                   macroBlock);
  }

  Reference<CodeBlock> AutoClasses() {
    CodeBlock* block = new CodeBlock;
    for (uint i = 1; i <= kMaxFields; ++i)
      Generate_AutoClass(*block, i);
    return block;
  }
}
