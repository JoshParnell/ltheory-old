#include "../Expressions.h"

#include "LTE/Environment.h"
#include "LTE/ProgramLog.h"
#include "LTE/Script.h"
#include "LTE/StringList.h"

#include "LTE/Debug.h"

#include <sstream>

namespace {
  inline void ScriptType_Construct(TypeT* type, void* buf) {
    ScriptType const& self = type->GetAux().Convert<ScriptType>();
    char* buffer = (char*)buf;
    for (size_t i = 0; i < self->fields.size(); ++i) {
      void* fieldPtr = buffer + self->fields[i].offset;
      self->fields[i].type->Construct(fieldPtr);
      if (self->initializers[i])
        self->initializers[i]->Evaluate(fieldPtr);
    }
  }

  inline void ScriptType_Destruct(TypeT* type, void* buf) {
    ScriptType const& self = type->GetAux().Convert<ScriptType>();
    char* buffer = (char*)buf;
    for (size_t i = 0; i < self->fields.size(); ++i)
      self->fields[i].type->Destruct(buffer + self->fields[i].offset);
  }

  inline void* ScriptType_Allocate(TypeT* type) {
    ScriptType const& self = type->GetAux().Convert<ScriptType>();
    char* buffer = (char*)malloc(self->size);
    ScriptType_Construct(type, buffer);
    return buffer;
  }

  inline void ScriptType_Deallocate(TypeT* type, void* buf) {
    ScriptType_Destruct(type, buf);
    free(buf);
  }

  inline void ScriptType_Assign(TypeT* type, void const* src, void* dst) {
    ScriptType const& self = type->GetAux().Convert<ScriptType>();
    char const* srcBuf = (char const*)src;
    char* dstBuf = (char*)dst;
    for (size_t i = 0; i < self->fields.size(); ++i)
      self->fields[i].type->Assign(
        srcBuf + self->fields[i].offset,
        dstBuf + self->fields[i].offset);
  }

  inline void ScriptType_Map(
    TypeT* type,
    void* buf,
    FieldMapper& m,
    void* aux)
  {
    ScriptType const& self = type->GetAux().Convert<ScriptType>();
    char* buffer = (char*)buf;
    for (size_t i = 0; i < self->fields.size(); ++i)
      m(buffer + self->fields[i].offset,
        self->fields[i].name,
        self->fields[i].type,
        aux);
  }

  inline void ScriptType_ToString(
    TypeT* type,
    void* ptr,
    String* string)
  {
    ScriptType const& self = type->GetAux().Convert<ScriptType>();
    std::stringstream stream;
    stream << type->name << " @ " << ptr << "\n";

    Vector<String> split;
    for (size_t i = 0; i < self->fields.size(); ++i) {
      stream << "  " << self->fields[i].name << " : ";

      String result = self->fields[i].type->ToString(
        (char*)ptr + self->fields[i].offset);

      String_Split(split, result, '\n');
      if (split.size()) {
        stream << split[0] << '\n';
        for (size_t j = 1; j < split.size(); ++j)
          stream << "  " << split[j] << '\n';
      } else {
        stream << '\n';
      }
    }
    *string = stream.str();
  }
}

namespace LTE {
  Expression Expression_Type(
    StringList const& list,
    CompileEnvironment& env)
  {
    if (list->GetSize() < 2) {
      Log_Message(Stringize() | "type -- list does not have 1 argument");
      return nullptr;
    }

    String const& name = list->Get(1)->GetValue();
    ScriptType type = env.script->types[name];

    if (type) {
      Log_Message(Stringize()
        | "type -- type '" | name | "' is already defined");
      return nullptr;
    }

    type = new ScriptTypeT;
    type->name = name;
    size_t alignment = 1;

    /* TODO : Unify with outer parsing. */
    /* TODO : Function declaration / definition separation. */

    for (size_t i = 2; i < list->GetSize(); ++i) {
      StringList const& sub = list->Get(i);
      String const& value = sub->Get(0)->GetValue();

      if (value == "#") {}

      else if (value == "function") {}

      else {
        if (sub->GetSize() < 2 ||
            sub->GetSize() > 3)
        {
          Log_Message(Stringize() |
            "type -- field does not have 1 or 2 arguments");
          return nullptr;
        }

        StringList const& typeName = sub->Get(0);
        Type fieldType = env.script->ResolveType(typeName);

        if (!fieldType) {
          Log_Message(Stringize() |
            "type -- field type '" | typeName->GetString() | "' not found");
          return nullptr;
        }

        String const& fieldName = sub->Get(1)->GetValue();
        for (size_t j = 0; j < type->fields.size(); ++j) {
          if (fieldName == type->fields[j].name) {
            Log_Message(Stringize() |
              "type -- field name '" | fieldName | "' already used");
            return nullptr;
          }
        }
        
        Expression initializer;
        if (sub->GetSize() == 3) {
          CompileEnvironment subEnv;
          subEnv.script = env.script;

          initializer = Expression_Compile(sub->Get(2), subEnv);
          if (!initializer) {
            Log_Message(Stringize()
              | "type -- initializer for field '" | fieldName
              | "' did not compile");
            return nullptr;
          }

          initializer = Expression_Conversion(initializer, fieldType);
          if (!initializer) {
            Log_Message(Stringize()
              | "type -- initializer for field '" | fieldName
              | "' could not be converted to type '" | fieldType->name | "'");
            return nullptr;
          }
        }

        type->initializers.push(initializer);

        /* Field padding. */ {
          size_t padding = type->size % fieldType->alignment;
          type->size += padding;
        }

        type->fields.push(Field(fieldName, fieldType, type->size));
        type->size += fieldType->size;
        alignment = Max(alignment, fieldType->alignment);
      }
    }

    /* Final padding. */
    if (type->size)
      type->size += type->size % alignment;

    /* TODO : Type leakage. */
    Type hardType = Type_Create(type->name, type->size);
    hardType->GetAux() = type;
    hardType->GetFields() = type->fields;
    hardType->alignment = alignment;
    hardType->allocate = ScriptType_Allocate;
    hardType->assign = ScriptType_Assign;
    hardType->construct = ScriptType_Construct;
    hardType->deallocate = ScriptType_Deallocate;
    hardType->destruct = ScriptType_Destruct;
    hardType->mapper = ScriptType_Map;
    hardType->toString = ScriptType_ToString;
    type->type = hardType;

    env.script->types[name] = type;
    env.context.push(type);

    for (size_t i = 2; i < list->GetSize(); ++i) {
      StringList const& sub = list->Get(i);
      if (sub->Get(0)->GetValue() == "function")
        Expression_Function(sub, env);
    }

    env.context.pop();
    return nullptr;
  }
}
