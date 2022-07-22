#include "LTE/Function.h"
#include "LTE/Renderer.h"
#include "LTE/ShaderInstance.h"
#include "LTE/Texture2D.h"

#include "LTE/Transform.h"
#include "LTE/Viewport.h"

#include "UI/WidgetRenderer.h"

TypeAlias(Reference<Texture2DT>, Texture2D);

VoidFreeFunction(Texture2D_BeginDrawTo,
  "Start using 'texture' as the destination for drawing operations",
  Texture2D, texture)
{
  texture->Bind(0);
  Renderer_SetWorldTransform(Transform_Identity());
  Renderer_SetViewTransform(Transform_Identity());
  Viewport_Push(Viewport_Create(0, V2(texture->GetWidth(), texture->GetHeight()), 1, false));
  Viewport_Get()->LoadMatrix();
} FunctionAlias(Texture2D_BeginDrawTo, BeginDrawTo);

VoidFreeFunction(DrawClear,
  "Clear all pixels of the current drawing surface to have 'color' and 'alpha'",
  Color, color,
  float, alpha)
{
  Renderer_PushScissorOff();
  Renderer_Clear(V4(color.x, color.y, color.z, alpha));
  Renderer_PopScissor();
}

FreeFunction(Texture2D, Texture2D_Create,
  "Create a new 2-dimensional texture",
  int, width,
  int, height)
{
  return Texture_Create(width, height);
}

FreeFunction(Texture2D, Texture2D_CreateHDR,
  "Create a new 2-dimensional, high-dynamic-range texture",
  int, width,
  int, height)
{
  return Texture_Create(width, height, GL_TextureFormat::RGBA32F);
}

VoidFreeFunction(Texture2D_EndDrawTo,
  "Stop using 'texture' as the destination for drawing operations",
  Texture2D, texture)
{
  WidgetRenderer_Flush();
  Viewport_Pop();
  texture->Unbind();
} FunctionAlias(Texture2D_EndDrawTo, EndDrawTo);

VoidFreeFunction(Texture2D_GenerateFromShader,
  "Fill 'texture' by executing 'shader'",
  Texture2D, texture,
  Shader, shader)
{
  Texture_Generate(texture, shader);
} FunctionAlias(Texture2D_GenerateFromShader, GenerateFromShader);

VoidFreeFunction(Texture2D_GenerateFromShaderInstance,
  "Fill 'texture' by executing 'shaderInstance'",
  Texture2D, texture,
  ShaderInstance, shaderInstance)
{
  shaderInstance->Begin();
  Texture_Generate(texture, shaderInstance->GetShader());
  shaderInstance->End();
} FunctionAlias(Texture2D_GenerateFromShaderInstance, GenerateFromShader);

VoidFreeFunction(Texture2D_GenerateMipmap,
  "Automatically calculate all non-zero mipmap levels of 'texture'",
  Texture2D, texture)
{
  texture->GenerateMipmap();
} FunctionAlias(Texture2D_GenerateMipmap, GenerateMipmap);

FreeFunction(int, Texture2D_GetHeight,
  "Return the height of 'texture'",
  Texture2D, texture)
{
  return texture->GetHeight();
} FunctionAlias(Texture2D_GetHeight, GetHeight);

FreeFunction(int, Texture2D_GetWidth,
  "Return the width of 'texture'",
  Texture2D, texture)
{
  return texture->GetWidth();
} FunctionAlias(Texture2D_GetWidth, GetWidth);

VoidFreeFunction(Texture2D_SaveTo,
  "Save 'texture' to a standard image format (bmp, jpg, or png) at 'path'",
  Texture2D, texture,
  String, path)
{
  texture->SaveTo(path);
} FunctionAlias(Texture2D_SaveTo, SaveTo);
