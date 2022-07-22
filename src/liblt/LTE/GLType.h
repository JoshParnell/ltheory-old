#ifndef GLType_h__
#define GLType_h__

template <class T, int N>
struct ExplicitTypedef {
  T data;
  ExplicitTypedef() {}
  explicit ExplicitTypedef(T data) : data(data) {}
  operator T() const {return data;}
};

typedef ExplicitTypedef<unsigned int, 0> GL_Buffer;
typedef ExplicitTypedef<unsigned int, 1> GL_Framebuffer;
typedef ExplicitTypedef<unsigned int, 2> GL_Program;
typedef ExplicitTypedef<unsigned int, 3> GL_Renderbuffer;
typedef ExplicitTypedef<unsigned int, 4> GL_Shader;
typedef ExplicitTypedef<unsigned int, 5> GL_Texture;

static const GL_Buffer       GL_NullBuffer(0);
static const GL_Framebuffer  GL_NullFramebuffer(0);
static const GL_Program      GL_NullProgram(0);
static const GL_Renderbuffer GL_NullRenderbuffer(0);
static const GL_Shader       GL_NullShader(0);
static const GL_Texture      GL_NullTexture(0);

#endif
