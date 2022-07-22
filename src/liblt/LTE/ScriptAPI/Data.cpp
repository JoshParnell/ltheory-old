#include "LTE/Data.h"
#include "LTE/Function.h"
#include "LTE/Location.h"
#include "LTE/Serializer.h"

FreeFunction(bool, Data_IsType,
  "Return whether 'data' is of type 'type'",
  Data, data,
  String, type)
{
  return data.type && data.type->name == type;
} FunctionAlias(Data_IsType, IsType);

FreeFunctionNoParams(Data, Data_None,
  "Return an empty piece of data")
{
  return Data();
}

FreeFunction(bool, Data_NotEmpty,
  "Return whether 'data' contains something",
  Data, data)
{
  return data.type;
} FunctionAlias(Data_NotEmpty, NotEmpty);

FreeFunction(bool, Data_IsNotNull,
  "Return whether 'data' is an object rather than a null pointer",
  Data, data)
{
  if (!data.type)
    return false;
  return !data.type->GetPointeeType() || *(void**)data.data;
} FunctionAlias(Data_IsNotNull, IsNotNull);

FreeFunction(bool, Data_IsNull,
  "Return whether 'data' is a null pointer",
  Data, data)
{
  if (!data.type)
    return true;
  return data.type->GetPointeeType() && *(void**)data.data == nullptr;
} FunctionAlias(Data_IsNull, IsNull);

FreeFunction(Data, Data_LoadFrom,
  "Load binary data from 'path'",
  String, path)
{
  Data data;
  LoadFrom(data, Location_File(path));
  return data;
} FunctionAlias(Data_LoadFrom, LoadFrom);

VoidFreeFunction(Data_SaveTo,
  "Save 'data' in binary form to 'path'",
  Data, data,
  String, path)
{
  SaveTo((Data&)data, Location_File(path), 0);
} FunctionAlias(Data_SaveTo, SaveTo);
