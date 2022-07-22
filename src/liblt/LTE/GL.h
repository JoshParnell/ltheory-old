/* --------------------------------------------------------------------------
 *  EasyGL
 *    ~ Josh Parnell, June 2012
 *
 *      Designed to be a super-thin wrapper over OGL calls, EasyGL allows
 *      easier access to all OpenGL functionality by encapsulating functions
 *      in the anonymous global structure GL and definining numerous
 *      enumerations to ensure that the correct enumeration type is ALWAYS
 *      passed to any given OGL function.
 *
 *      EasyGL defines several new "explicit" typedefs to make GL more
 *      type-safe. Using these new types will ensure that you don't pass invalid
 *      object types to GL functions.
 *
 *      Improves documentation of OGL calls by making certain parameter
 *      names more descriptive, less misleading, and by adding comments to
 *      every function (copied from www.opengl.org) (admittedly, not very
 *      helpful - but you can thank Khronos for that one).
 *
 *      Finally, includes a bit of extra functionality in the anonymous global
 *      structure GLU to make common, multiple-GL-call operations easier.
 *
 * -------------------------------------------------------------------------*/

#ifndef _EasyGL_JP_
#define _EasyGL_JP_

#define GLEW_STATIC

#include "Glew/GL/glew.h"
#include "BuildMode.h"

#include "GLEnum.h"
#include "GLType.h"
#include <string>

#ifdef BUILD_STRICT
  #include "Common.h"
  #include "ProgramLog.h"
  #define DEBUG_GL_ERRORS {                                                    \
    GL_Error::Enum error = GL_GetError();                                      \
    if (error != GL_Error::None) {                                             \
      Log_Error(std::string("OpenGL Error: ") + GL_Error::String(error));      \
      error("OpenGL Error");                                                   \
    }}
#else
  #define DEBUG_GL_ERRORS
#endif

/* Check for errors. */
inline GL_Error::Enum GL_GetError() {
  return (GL_Error::Enum)glGetError();
}

/* Select active texture unit. */
inline void GL_ActiveTexture(unsigned int unit) {
  glActiveTexture(GL_TEXTURE0 + unit);
  DEBUG_GL_ERRORS;
}

/* Attach a shader object to a program object. */
inline void GL_AttachShader(GL_Program program, GL_Shader shader) {
  glAttachShader((unsigned int)program, (unsigned int)shader);
  DEBUG_GL_ERRORS;
}

/* Delimit the vertices of a primitive or a group of like primitives. */
inline void GL_Begin(GL_DrawMode::Enum mode) {
  DEBUG_GL_ERRORS;
  glBegin(mode);
}

/* Associates a generic vertex attribute index with a named attribute variable */
inline void GL_BindAttribLocation(
  GL_Program program,
  unsigned int index,
  char const* name)
{
  glBindAttribLocation((unsigned int)program, index, name);
  DEBUG_GL_ERRORS;
}

/* Bind a named buffer object. */
inline void GL_BindBuffer(GL_BufferTarget::Enum target, GL_Buffer buffer) {
  glBindBuffer(target, (unsigned int)buffer);
  DEBUG_GL_ERRORS;
}

inline void GL_BindFragDataLocation(
  GL_Program program,
  unsigned int index,
  char const* name)
{
  glBindFragDataLocation((unsigned int)program, index, name);
  DEBUG_GL_ERRORS;
}

/* Bind a named framebuffer object. */
inline void GL_BindFramebuffer(
  GL_FramebufferTarget::Enum target,
  GL_Framebuffer buffer)
{
  glBindFramebuffer(target, (unsigned int)buffer);
  DEBUG_GL_ERRORS;
}

/* Bind a named renderbuffer object. */
inline void GL_BindRenderbuffer(
  GL_RenderbufferTarget::Enum target,
  GL_Renderbuffer renderBuffer)
{
  glBindRenderbuffer(target, (unsigned int)renderBuffer);
  DEBUG_GL_ERRORS;
}

/* Bind a named texture to a texturing target. */
inline void GL_BindTexture(
  GL_TextureTargetBindable::Enum target,
  GL_Texture texture)
{
  glBindTexture(target, (unsigned int)texture);
  DEBUG_GL_ERRORS;
}

/* Specify the blending function to be used for the source and destination
   surfaces. */
inline void GL_BlendFunc(
  GL_BlendFunction::Enum src,
  GL_BlendFunction::Enum dst)
{
  glBlendFunc(src, dst);
  DEBUG_GL_ERRORS;
}

/* Specify the blending function to be used for the source and destination
   surfaces, separating RGB and alpha equations. */
inline void GL_BlendFuncSeparate(
  GL_BlendFunction::Enum srcRGB,
  GL_BlendFunction::Enum dstRGB,
  GL_BlendFunction::Enum srcAlpha,
  GL_BlendFunction::Enum dstAlpha)
{
  glBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
  DEBUG_GL_ERRORS;
}

/* Creates and initalizes a buffer object's data store. */
inline void GL_BufferData(
  GL_BufferTarget::Enum target,
  int bufferSize,
  void const* initialData,
  GL_BufferUsage::Enum usage)
{
  glBufferData(target, bufferSize, initialData, usage);
  DEBUG_GL_ERRORS;
}

inline void GL_CheckFramebuffer() {
#ifdef BUILD_STRICT
  GL_FramebufferStatus::Enum status = 
    (GL_FramebufferStatus::Enum)glCheckFramebufferStatus(GL_FramebufferTarget::Draw);
  DEBUG_GL_ERRORS;
  if (status != GL_FramebufferStatus::Complete) {
    Log_Error(std::string("OpenGL Error: ")
        + GL_FramebufferStatus::String(status));
    error("OpenGL Error");
  }
#endif
}

/* Clear buffers to preset values. */
inline void GL_Clear(GL_BufferBit::Mask buffers) {
  GL_CheckFramebuffer();
  glClear(buffers);
  DEBUG_GL_ERRORS;
}

/* Specify clear values for the color buffers. */
inline void GL_ClearColor(float r, float g, float b, float a = 0.f) {
  glClearColor(r, g, b, a);
  DEBUG_GL_ERRORS;
}

/* Specify the clear value for the depth buffer. */
inline void GL_ClearDepth(float depth) {
  GL_CheckFramebuffer();
  glClearDepth(depth);
  DEBUG_GL_ERRORS;
}

/* Set the current color. */
inline void GL_Color(float r, float g, float b) {
  glColor3f(r, g, b);
}

/* Set the current color. */
inline void GL_Color(float r, float g, float b, float a) {
  glColor4f(r, g, b, a);
}

/* Compile a shader object. */
inline void GL_CompileShader(GL_Shader shader) {
  glCompileShader((unsigned int)shader);
  DEBUG_GL_ERRORS;
}

inline void GL_CopyTexImage2D(
  GL_TextureTarget::Enum target,
  int level,
  GL_TextureFormat::Enum internalFormat,
  int x,
  int y,
  int width,
  int height,
  int border)
{
  glCopyTexImage2D(target, level, internalFormat, x, y, width, height, border);
  DEBUG_GL_ERRORS;
}

/* Create a program object. */
inline GL_Program GL_CreateProgram() {
  GL_Program p = GL_Program(glCreateProgram());
  DEBUG_GL_ERRORS;
  return p;
}

/* Create a shader object. */
inline GL_Shader GL_CreateShader(GL_ShaderType::Enum type) {
  GL_Shader s = GL_Shader(glCreateShader(type));
  DEBUG_GL_ERRORS;
  return s;
}

/* Specify whether front- or back-facing facets can be culled. */
inline void GL_CullFace(GL_CullMode::Enum mode) {
  glCullFace(mode);
  DEBUG_GL_ERRORS;
}

/* NONSTANDARD
   Delete a single named buffer object. */
inline void GL_DeleteBuffer(GL_Buffer buffer) {
  glDeleteBuffers(1, (unsigned int*)&buffer);
  DEBUG_GL_ERRORS;
}

/* Delete named buffer objects. */
inline void GL_DeleteBuffers(int count, GL_Buffer* bufferArray) {
  glDeleteBuffers(count, (unsigned int*)bufferArray);
  DEBUG_GL_ERRORS;
}

/* NONSTANDARD
   Delete a single named buffer object. */
inline void GL_DeleteFramebuffer(GL_Framebuffer buffer) {
  glDeleteFramebuffers(1, (unsigned int*)&buffer);
  DEBUG_GL_ERRORS;
}

/* Delete a program object. */
inline void GL_DeleteProgram(GL_Program program) {
  glDeleteProgram((unsigned int)program);
  DEBUG_GL_ERRORS;
}

/* Delete a shader object. */
inline void GL_DeleteShader(GL_Shader shader) {
  glDeleteShader((unsigned int)shader);
  DEBUG_GL_ERRORS;
}

/* NONSTANDARD
   Delete a single named texture. */
inline void GL_DeleteTexture(GL_Texture texture) {
  glDeleteTextures(1, (unsigned int*)&texture);
  DEBUG_GL_ERRORS;
}

/* Delete named textures. */
inline void GL_DeleteTextures(int count, GL_Texture* textureArray) {
  glDeleteTextures(count, (unsigned int*)textureArray);
  DEBUG_GL_ERRORS;
}

/* Enable or disable writing to the depth buffer. */
inline void GL_DepthMask(bool depthWritable) {
  glDepthMask(depthWritable);
  DEBUG_GL_ERRORS;
}

/* Disable specific OpenGL capabilities. */
inline void GL_Disable(GL_Capability::Enum cap) {
  glDisable(cap);
  DEBUG_GL_ERRORS;
}

/* Disable the usage of a specific generic vertex attribute array. */
inline void GL_DisableVertexAttribArray(unsigned int index) {
  glDisableVertexAttribArray(index);
  DEBUG_GL_ERRORS;
}

/* Render primitives from array data. */
inline void GL_DrawElements(
  GL_DrawMode::Enum mode,
  int indexCount,
  GL_IndexFormat::Enum indexFormat,
  void const* indexData)
{
  GL_CheckFramebuffer();
  glDrawElements(mode, indexCount, indexFormat, indexData);
  DEBUG_GL_ERRORS;
}

/* Enable specific OpenGL capabilities. */
inline void GL_Enable(GL_Capability::Enum cap) {
  glEnable(cap);
  DEBUG_GL_ERRORS;
}

/* Enable the usage of a specific generic vertex attribute array. */
inline void GL_EnableVertexAttribArray(unsigned int index) {
  glEnableVertexAttribArray(index);
  DEBUG_GL_ERRORS;
}

/* Delimit the vertices of a primitive or a group of like primitives. */
inline void GL_End() {
  glEnd();
  DEBUG_GL_ERRORS;
}

/* Block until GL execution is complete. */
inline void GL_Finish() {
  glFinish();
  DEBUG_GL_ERRORS;
}

/* Attach a level of a texture object as a logical buffer to the currently
   bound framebuffer object. */
inline void GL_FramebufferTexture2D(
  GL_FramebufferTarget::Enum framebufferTarget,
  GL_FramebufferAttachment::Enum attachment,
  GL_TextureTarget::Enum textureTarget,
  GL_Texture texture,
  int level)
{
  glFramebufferTexture2D(framebufferTarget, attachment, textureTarget,
                         texture, level);
  DEBUG_GL_ERRORS;
}

/* Define front- and back-facing polygons. */
inline void GL_FrontFace(GL_FaceOrientation::Enum mode) {
  glFrontFace(mode);
  DEBUG_GL_ERRORS;
}

/* NONSTANDARD
   Generate a single buffer object. */
inline GL_Buffer GL_GenBuffer() {
  GL_Buffer buf;
  glGenBuffers(1, (unsigned int*)&buf);
  DEBUG_GL_ERRORS;
  return buf;
}

/* Generate buffer object names. */
inline void GL_GenBuffers(int count, GL_Buffer* bufferArray) {
  glGenBuffers(count, (unsigned int*)bufferArray);
  DEBUG_GL_ERRORS;
}

/* NONSTANDARD
   Generate a single render buffer object. */
inline GL_Renderbuffer GL_GenRenderBuffer() {
  GL_Renderbuffer buf;
  glGenRenderbuffers(1, (unsigned int*)&buf);
  DEBUG_GL_ERRORS;
  return buf;
}

/* Generate render buffer object names. */
inline void GL_GenRenderBuffers(
  int count,
  GL_Renderbuffer* rBufferArray)
{
  glGenRenderbuffers(count, (unsigned int*)rBufferArray);
  DEBUG_GL_ERRORS;
}

/* Generate mipmaps for a specified texture target. */
inline void GL_GenerateMipmap(GL_TextureTarget::Enum target) {
  glGenerateMipmap(target);
  DEBUG_GL_ERRORS;
}

/* NONSTANDARD
   Generate a single framebuffer object. */
inline GL_Framebuffer GL_GenFramebuffer() {
  GL_Framebuffer buf;
  glGenFramebuffers(1, (unsigned int*)&buf);
  DEBUG_GL_ERRORS;
  return buf;
}

/* Generate framebuffer object names. */
inline void GL_GenFramebuffers(int count, GL_Framebuffer* fboArray) {
  glGenFramebuffers(count, (unsigned int*)fboArray);
  DEBUG_GL_ERRORS;
}

/* NONSTANDARD
   Generate a single texture name. */
inline GL_Texture GL_GenTexture() {
  GL_Texture tex;
  glGenTextures(1, (unsigned int*)&tex);
  DEBUG_GL_ERRORS;
  return tex;
}

/* Generate texture names. */
inline void GL_GenTextures(int count, GL_Texture* textureArray) {
  glGenTextures(count, (unsigned int*)textureArray);
  DEBUG_GL_ERRORS;
}

/* Returns information about an active uniform variable for the specified
   program object. */
inline void GL_GetActiveUniform(
  GL_Program program,
  unsigned int index,
  int& size,
  GLenum& type,
  std::string& name)
{
  char buf[256];
  glGetActiveUniform((unsigned int)program, index, sizeof(buf) - 1, NULL,
                     &size, &type, buf);
  name = buf;
  DEBUG_GL_ERRORS;
}

/* NONSTANDARD
   Return the currently-bounded program. */
inline GL_Program GL_GetCurrentProgram() {
  int program;
  glGetIntegerv(GL_CURRENT_PROGRAM, &program);
  return (GL_Program)(unsigned int)program;
}

/* Checks the completeness status of a framebuffer. */
inline GL_FramebufferStatus::Enum GL_GetFramebufferStatus(GL_Framebuffer fbo) {
  return (GL_FramebufferStatus::Enum)glCheckFramebufferStatus((unsigned int)fbo);
}

/* Returns an integer program property. */
inline int GL_GetProgramI(GL_Program program, GL_ProgramProperty::Enum property) {
  int i;
  glGetProgramiv((unsigned int)program, property, &i);
  DEBUG_GL_ERRORS;
  return i;
}

/* NONSTANDARD
   Check the validation status of the given program. */
inline bool GL_GetProgramValidationStatus(GL_Program program) {
  int b;
  glGetProgramiv(program, GL_VALIDATE_STATUS, &b);
  return b == GL_TRUE;
}

/* Return an integer shader propery. */
inline int GL_GetShaderI(GL_Shader shader, GL_ShaderProperty::Enum property) {
  int i;
  glGetShaderiv((unsigned int)shader, property, &i);
  DEBUG_GL_ERRORS;
  return i;
}

/* MODIFIED STANDARD
   Retrieves the log associated with a program. Original specification
   is cumbersome. */
inline void GL_GetProgramInfoLog(GL_Program program, std::string& log) {
  int logSize = GL_GetProgramI(program, GL_ProgramProperty::InfoLogLength);
  char* logBuffer = new char[logSize];
  glGetProgramInfoLog((unsigned int)program, logSize, NULL, logBuffer);
  log = logBuffer;
  delete[] logBuffer;
  DEBUG_GL_ERRORS;
}

/* MODIFIED STANDARD
   Retrieves the log associated with a shader. Original specification
   is cumbersome. */
inline void GL_GetShaderInfoLog(GL_Shader shader, std::string& log) {
  int logSize = GL_GetShaderI(shader, GL_ShaderProperty::InfoLogLength);
  char* logBuffer = new char[logSize];
  glGetShaderInfoLog((unsigned int)shader, logSize, NULL, logBuffer);
  log = logBuffer;
  delete[] logBuffer;
  DEBUG_GL_ERRORS;
}

/* Obtain the image data associated with the given LOD level of a texture
 * object. */
inline void GL_GetTexImage(
  GL_TextureTarget::Enum target,
  int level,
  GL_PixelFormat::Enum pixelFormat,
  GL_DataFormat::Enum dataFormat,
  void* buffer)
{
  glGetTexImage(target, level, pixelFormat, dataFormat, buffer);
  DEBUG_GL_ERRORS;
}

/* Return texture parameter values for a specific level of detail. */
inline int GL_GetTexLevelParameterI(
  GL_TextureTarget::Enum target,
  int level,
  GL_TextureParameterRead::Enum parameter)
{
  int i;
  glGetTexLevelParameteriv(target, level, parameter, &i);
  DEBUG_GL_ERRORS;
  return i;
}

/* NONSTANDARD
   Return the height of the texture bound to the given target. */
inline int GL_GetTexHeight(GL_TextureTarget::Enum target) {
  return GL_GetTexLevelParameterI(
    GL_TextureTarget::T2D, 0, GL_TextureParameterRead::Height);
}

/* NONSTANDARD
   Return the width of the texture bound to the given target. */
inline int GL_GetTexWidth(GL_TextureTarget::Enum target) {
  return GL_GetTexLevelParameterI(
    GL_TextureTarget::T2D, 0, GL_TextureParameterRead::Width);
}

/* Return the location of a uniform variable. */
inline int GL_GetUniformLocation(GL_Program program, const char* name) {
  int location = glGetUniformLocation((unsigned int)program, name);
  DEBUG_GL_ERRORS;
  return location;
}

/* Link a program object. */
inline void GL_LinkProgram(GL_Program program) {
  glLinkProgram((unsigned int)program);
  DEBUG_GL_ERRORS;
}

/* Replace the current matrix with the identity matrix. */
inline void GL_LoadIdentity() {
  glLoadIdentity();
  DEBUG_GL_ERRORS;
}

/* Replace the current matrix with the specified matrix. */
inline void GL_LoadMatrix(const float* m) {
  glLoadMatrixf(m);
  DEBUG_GL_ERRORS;
}

/* Specify which matrix is the current matrix. */
inline void GL_MatrixMode(GL_Matrix::Enum mode) {
  glMatrixMode(mode);
  DEBUG_GL_ERRORS;
}

/* Multiply the current matrix with the given matrix. */
inline void GL_MultMatrix(const float* m) {
  glMultMatrixf(m);
  DEBUG_GL_ERRORS;
}

/* Define an array of normals. */
inline void GL_NormalPointer(
  GL_VertexFormat::Enum format,
  int sizeofVertex,
  void const* normalData)
{
  glNormalPointer(format, sizeofVertex, normalData);
  DEBUG_GL_ERRORS;
}

/* Multiply the current matrix with an orthographic matrix. */
inline void GL_Ortho(
  float left,
  float right,
  float bottom,
  float top,
  float nearPlane,
  float farPlane)
{
  glOrtho(left, right, bottom, top, nearPlane, farPlane);
  DEBUG_GL_ERRORS;
}

/* Select a polygon rasterization mode. */
inline void GL_PolygonMode(GL_FaceType::Enum face, GL_FillMode::Enum fillMode) {
  glPolygonMode(face, fillMode);
  DEBUG_GL_ERRORS;
}

/* Push the current matrix onto the matrix stack. */
inline void GL_PushMatrix() {
  glPushMatrix();
  DEBUG_GL_ERRORS;
}

/* Pop a matrix off of the stack, and load it as the current matrix. */
inline void GL_PopMatrix() {
  glPopMatrix();
  DEBUG_GL_ERRORS;
}

/* Copy the current framebuffer's pixel data into the given buffer. */
inline void GL_ReadPixels(
  int x,
  int y,
  int width,
  int height,
  GL_PixelFormat::Enum pixelFormat,
  GL_DataFormat::Enum dataFormat,
  void* data)
{
  glReadPixels(x, y, width, height, pixelFormat, dataFormat, data);
  DEBUG_GL_ERRORS;
}

/* Define the scissor box. */
inline void GL_Scissor(int x, int y, int width, int height) {
  glScissor(x, y, width, height);
  DEBUG_GL_ERRORS;
}

/* MODIFIED STANDARD
   Replaces the source code in a shader object.
   Modified for easier use. Original function specification is silly. */
inline void GL_ShaderSource(GL_Shader shader, std::string const& source) {
  const int size = (int)source.size();
  const char* src = source.c_str();
  glShaderSource((unsigned int)shader, 1, &src, &size);
  DEBUG_GL_ERRORS;
}

/* Set the current texture coordinates. */
inline void GL_TexCoord(float u) {
  glTexCoord1f(u);
}

/* Set the current texture coordinates. */
inline void GL_TexCoord(float u, float v) {
  glTexCoord2f(u, v);
}

/* Set the current texture coordinates. */
inline void GL_TexCoord(float u, float v, float w) {
  glTexCoord3f(u, v, w);
}

/* Define an array of texture coordinates. */
inline void GL_TexCoordPointer(
  int componentsPerCoord,
  GL_VertexFormat::Enum format,
  size_t sizeofVertex,
  void const* data)
{
  glTexCoordPointer(componentsPerCoord, format, (int)sizeofVertex, data);
  DEBUG_GL_ERRORS;
}

/* Specify a two-dimensional texture image. */
inline void GL_TexImage2D(
  GL_TextureTarget::Enum target,
  int level,
  GL_TextureFormat::Enum internalFormat,
  size_t width,
  size_t height,
  GL_PixelFormat::Enum pixelFormat,
  GL_DataFormat::Enum dataFormat,
  void const* uploadData)
{
  glTexImage2D(target, level, internalFormat,
    (int)width, (int)height, 0,
    pixelFormat, dataFormat, uploadData);
  DEBUG_GL_ERRORS;
}

/* Specify a three-dimensional texture image. */
inline void GL_TexImage3D(
  GL_TextureTarget::Enum target,
  int level,
  GL_TextureFormat::Enum internalFormat,
  size_t width,
  size_t height,
  size_t depth,
  GL_PixelFormat::Enum pixelFormat,
  GL_DataFormat::Enum dataFormat,
  void const* uploadData)
{
  glTexImage3D(target, level, internalFormat,
    (int)width, (int)height, (int)depth, 0,
    pixelFormat, dataFormat, uploadData);
  DEBUG_GL_ERRORS;
}

/* NONSTANDARD
   Specify the texture coordinate wrapping mode for the given texture
   target and the given texture coordinate. */
inline void GL_TexWrapMode(
  GL_TextureTarget::Enum target,
  GL_TextureCoordinate::Enum coordinate,
  GL_TextureWrapMode::Enum mode)
{
  GLenum pName;
  if (coordinate == GL_TextureCoordinate::S)
    pName = GL_TEXTURE_WRAP_S;
  else if (coordinate == GL_TextureCoordinate::T)
    pName = GL_TEXTURE_WRAP_T;
  else
    pName = GL_TEXTURE_WRAP_R;
  glTexParameteri(target, pName, mode);
  DEBUG_GL_ERRORS;
}

/* Specify a rectangular subregion of a two-dimensional texture image. */
inline void GL_TexSubImage2D(
  GL_TextureTarget::Enum target,
  int level,
  int xOffset,
  int yOffset,
  size_t width,
  size_t height,
  GL_PixelFormat::Enum pixelFormat,
  GL_DataFormat::Enum dataFormat,
  void const* uploadData)
{
  glTexSubImage2D(target, level, xOffset, yOffset, (int)width, (int)height,
                  pixelFormat, dataFormat, uploadData);
  DEBUG_GL_ERRORS;
}

/* Specify a box-shaped subregion of a three-dimensional texture image. */
inline void GL_TexSubImage3D(
  GL_TextureTarget::Enum target,
  int level,
  int xOffset,
  int yOffset,
  int zOffset,
  size_t width,
  size_t height,
  size_t depth,
  GL_PixelFormat::Enum pixelFormat,
  GL_DataFormat::Enum dataFormat,
  void const* uploadData)
{
  glTexSubImage3D(target, level, xOffset, yOffset, zOffset,
    (int)width, (int)height, (int)depth, pixelFormat, dataFormat, uploadData);
  DEBUG_GL_ERRORS;
}

/* NONSTANDARD
   Specifies the index of the lowest defined mipmap level. */
inline void GL_TexBaseLevel(GL_TextureTarget::Enum target, int level) {
  glTexParameteri(target, GL_TextureParameter::BaseLevel, level);
  DEBUG_GL_ERRORS;
}

/* NONSTANDARD
   Set the magnification filter mode of the given texture. */
inline void GL_TexMagFilter(
  GL_TextureTarget::Enum target,
  GL_TextureFilter::Enum filter)
{
  glTexParameteri(target, GL_TextureParameter::MagFilter, filter);
  DEBUG_GL_ERRORS;
}

/* NONSTANDARD
   Sets the maximum anisotropy value of the given texture. */
inline void GL_TexMaxAnisotropy(
  GL_TextureTarget::Enum target,
  float anisotropy)
{
  glTexParameterf(target, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
  DEBUG_GL_ERRORS;
}

/* NONSTANDARD
   Set the minification filter mode of the given texture. */
inline void GL_TexMinFilter(
  GL_TextureTarget::Enum target,
  GL_TextureFilterMip::Enum filter)
{
  glTexParameteri(target, GL_TextureParameter::MinFilter, filter);
  DEBUG_GL_ERRORS;
}

/* Set texture parameters. */
inline void GL_TexParameter(
  GL_TextureTarget::Enum target,
  GL_TextureParameter::Enum param,
  int paramValue)
{
  glTexParameteri(target, param, paramValue);
  DEBUG_GL_ERRORS;
}

/* Set texture parameters. */
inline void GL_TexParameter(
  GL_TextureTarget::Enum target,
  GL_TextureParameter::Enum param,
  float paramValue)
{
  glTexParameterf(target, param, paramValue);
  DEBUG_GL_ERRORS;
}

/* Specify the value of a uniform variable for the current program. */
inline void GL_Uniform(int location, float f1) {
  glUniform1f(location, f1);
  DEBUG_GL_ERRORS;
}

/* Specify the value of a uniform variable for the current program. */
inline void GL_Uniform(int location, float f1, float f2) {
  glUniform2f(location, f1, f2);
  DEBUG_GL_ERRORS;
}

/* Specify the value of a uniform variable for the current program. */
inline void GL_Uniform(int location, float f1, float f2, float f3) {
  glUniform3f(location, f1, f2, f3);
  DEBUG_GL_ERRORS;
}

/* Specify the value of a uniform variable for the current program. */
inline void GL_Uniform(int location, float f1, float f2, float f3, float f4) {
  glUniform4f(location, f1, f2, f3, f4);
  DEBUG_GL_ERRORS;
}

/* Specify the value of a uniform variable for the current program. */
inline void GL_Uniform(int location, int i1) {
  glUniform1i(location, i1);
  DEBUG_GL_ERRORS;
}

/* Specify the value of a uniform variable for the current program. */
inline void GL_Uniform(int location, int i1, int i2) {
  glUniform2i(location, i1, i2);
  DEBUG_GL_ERRORS;
}

/* Specify the value of a uniform variable for the current program. */
inline void GL_Uniform(int location, int i1, int i2, int i3) {
  glUniform3i(location, i1, i2, i3);
  DEBUG_GL_ERRORS;
}

/* Specify the value of a uniform variable for the current program. */
inline void GL_Uniform(int location, int i1, int i2, int i3, int i4) {
  glUniform4i(location, i1, i2, i3, i4);
  DEBUG_GL_ERRORS;
}

/* Specify the value of a uniform variable for the current program.
   NONSTANDARD : Required name change in order to avoid overload conflict.
*/
inline void GL_UniformArray1(int location, size_t arraySize, float const* fv) {
  glUniform1fv(location, (int)arraySize, fv);
  DEBUG_GL_ERRORS;
}

/* Specify the value of a uniform variable for the current program.
   NONSTANDARD : Required name change in order to avoid overload conflict.
*/
inline void GL_UniformArray2(int location, size_t arraySize, float const* fv) {
  glUniform2fv(location, (int)arraySize, fv);
  DEBUG_GL_ERRORS;
}

/* Specify the value of a uniform variable for the current program.
   NONSTANDARD : Required name change in order to avoid overload conflict.
*/
inline void GL_UniformArray3(int location, size_t arraySize, float const* fv) {
  glUniform3fv(location, (int)arraySize, fv);
  DEBUG_GL_ERRORS;
}

/* Specify the value of a uniform variable for the current program.
   NONSTANDARD : Required name change in order to avoid overload conflict.
*/
inline void GL_UniformArray4(int location, size_t arraySize, float const* fv) {
  glUniform4fv(location, (int)arraySize, fv);
  DEBUG_GL_ERRORS;
}

inline void GL_UniformMatrix2(int location, float const* m) {
  glUniformMatrix2fv(location, 1, false, m);
  DEBUG_GL_ERRORS;
}

inline void GL_UniformMatrix3(int location, float const* m) {
  glUniformMatrix3fv(location, 1, false, m);
  DEBUG_GL_ERRORS;
}

inline void GL_UniformMatrix4(int location, float const* m) {
  glUniformMatrix4fv(location, 1, false, m);
  DEBUG_GL_ERRORS;
}

/* Install a program object as part of current rendering state.
    NOTE : UseProgram(NullProgram) activates the fixed-function pipeline. */
inline void GL_UseProgram(GL_Program program) {
  glUseProgram((unsigned int)program);
  DEBUG_GL_ERRORS;
}

inline void GL_ValidateProgram(GL_Program program) {
  glValidateProgram((unsigned int)program);
  DEBUG_GL_ERRORS;
}

/* Specify a vertex of float components. */
inline void GL_Vertex(float x, float y) {
  glVertex2f(x, y);
}

/* Specify a vertex of float components. */
inline void GL_Vertex(float x, float y, float z) {
  glVertex3f(x, y, z);
}

inline void GL_VertexAttrib(unsigned int index, float v0) {
  glVertexAttrib1f(index, v0);
}

inline void GL_VertexAttrib(unsigned int index, float v0, float v1) {
  glVertexAttrib2f(index, v0, v1);
}

inline void GL_VertexAttrib(unsigned int index, float v0, float v1, float v2) {
  glVertexAttrib3f(index, v0, v1, v2);
}

inline void GL_VertexAttrib(unsigned int index, float v0, float v1, float v2, float v3) {
  glVertexAttrib4f(index, v0, v1, v2, v3);
}

inline void GL_VertexAttribPointer(
  unsigned int index,
  int valuesPerVertex,
  GL_DataFormat::Enum dataType,
  bool normalized,
  size_t sizeOfVertex,
  void const* data)
{
  glVertexAttribPointer(index, valuesPerVertex, dataType, normalized,
                        (int)sizeOfVertex, data);
  DEBUG_GL_ERRORS;
}

/* Define an array of vertex data. */
inline void GL_VertexPointer(
  int componentsPerVertex,
  GL_VertexFormat::Enum vf,
  size_t sizeOfVertex,
  void const* data)
{
  glVertexPointer(componentsPerVertex, vf, (int)sizeOfVertex, data);
  DEBUG_GL_ERRORS;
}

/* Set the viewport. */
inline void GL_Viewport(int x, int y, int width, int height) {
  glViewport(x, y, width, height);
  DEBUG_GL_ERRORS;
}

namespace GLU {
  /* Loads the identity matrix into both the ModelView and Projection
     matrix stacks. */
  inline void ClearMatrices() {
    GL_MatrixMode(GL_Matrix::ModelView);
    GL_LoadIdentity();
    GL_MatrixMode(GL_Matrix::Projection);
    GL_LoadIdentity();
  }

  /* Create and initialize a 2D texture of the given specifications.
     NOTE : The min and mag filters are automatically set to linear.
     NOTE : The texture unit 0's binding is stomped on.*/
  inline GL_Texture CreateTexture2D(
    unsigned int width,
    unsigned int height,
    GL_TextureFormat::Enum internalFormat)
  {
    GL_Texture texture;
    GL_GenTextures(1, &texture);
    GL_BindTexture(GL_TextureTargetBindable::T2D, texture);

    /* Note that these are necessary to make the textures "complete,"
       since we won't be generating mip-maps. */
    GL_TexMagFilter(GL_TextureTarget::T2D, GL_TextureFilter::Linear);
    GL_TexMinFilter(GL_TextureTarget::T2D, GL_TextureFilterMip::LinearMipLinear);
    return texture;
  }

  /* Draws a quad spanning [minX, minY] to [maxX, maxY].
     NOTE : Does NOT modify the matrix stacks, so matrices must be set to
            identity first if the quad is to be drawn untransformed. */
  inline void DrawQuad(
    float minX, float minY,
    float maxX, float maxY,
    float z = 0.0f)
  {
    GL_Begin(GL_DrawMode::Quads);
    GL_TexCoord(0.0f, 1.0f);
    GL_Vertex(minX, minY, z);
    GL_TexCoord(1.0f, 1.0f);
    GL_Vertex(maxX, minY, z);
    GL_TexCoord(1.0f, 0.0f);
    GL_Vertex(maxX, maxY, z);
    GL_TexCoord(0.0f, 0.0f);
    GL_Vertex(minX, maxY, z);
    GL_End();
  }

  /* Pushes both the ModelView and Projection matrix stacks. */
  inline void PushMatrices() {
    GL_MatrixMode(GL_Matrix::ModelView);
    GL_PushMatrix();
    GL_MatrixMode(GL_Matrix::Projection);
    GL_PushMatrix();
  }

  /* Pops both the ModelView and Projection matrix stacks. */
  inline void PopMatrices() {
    GL_MatrixMode(GL_Matrix::ModelView);
    GL_PopMatrix();
    GL_MatrixMode(GL_Matrix::Projection);
    GL_PopMatrix();
  }

  /* Loads the given matrices into the ModelView and Projection stacks,
     respectively. */
  inline void SetMatrices(float const* modelView, float const* projection) {
    GL_MatrixMode(GL_Matrix::ModelView);
    GL_LoadMatrix(modelView);
    GL_MatrixMode(GL_Matrix::Projection);
    GL_LoadMatrix(projection);
  }
}

#endif
