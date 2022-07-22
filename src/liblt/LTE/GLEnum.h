#ifndef GLEnum_h__
#define GLEnum_h__

#define GLEW_STATIC
#include "Common.h"
#include "Glew/GL/glew.h"

namespace GL_BlendFunction {
  enum Enum {
    ConstantAlpha              = GL_CONSTANT_ALPHA,
    ConstantColor              = GL_CONSTANT_COLOR,
    DestAlpha                  = GL_DST_ALPHA,
    DestColor                  = GL_DST_COLOR,
    One                        = GL_ONE,
    OneMinusConstantAlpha      = GL_ONE_MINUS_CONSTANT_ALPHA,
    OneMinusConstantColor      = GL_ONE_MINUS_CONSTANT_COLOR,
    OneMinusDestAlpha          = GL_ONE_MINUS_DST_ALPHA,
    OneMinusDestColor          = GL_ONE_MINUS_DST_COLOR,
    OneMinusSourceAlpha        = GL_ONE_MINUS_SRC_ALPHA,
    OneMinusSourceColor        = GL_ONE_MINUS_SRC_COLOR,
    SourceAlpha                = GL_SRC_ALPHA,
    SourceColor                = GL_SRC_COLOR,
    Zero                       = GL_ZERO
  };
}

namespace GL_BufferBit {
  typedef unsigned int Mask;
  const Mask Accum             = GL_ACCUM_BUFFER_BIT;
  const Mask Color             = GL_COLOR_BUFFER_BIT;
  const Mask Depth             = GL_DEPTH_BUFFER_BIT;
  const Mask Stencil           = GL_STENCIL_BUFFER_BIT;
}

namespace GL_BufferTarget {
  enum Enum {
    Array                      = GL_ARRAY_BUFFER,
    ElementArray               = GL_ELEMENT_ARRAY_BUFFER,
    PixelPack                  = GL_PIXEL_PACK_BUFFER,
    PixelUnpack                = GL_PIXEL_UNPACK_BUFFER
  };
}

namespace GL_BufferUsage {
  enum Enum {
    DynamicCopy                = GL_DYNAMIC_COPY,
    DynamicDraw                = GL_DYNAMIC_DRAW,
    DynamicRead                = GL_DYNAMIC_READ,
    StaticDraw                 = GL_STATIC_DRAW,
    StaticRead                 = GL_STATIC_READ,
    StreamDraw                 = GL_STREAM_DRAW,
    StreamRead                 = GL_STREAM_READ
  };
}

namespace GL_Capability {
  enum Enum {
    Blend                      = GL_BLEND,
    CubemapSeamless            = GL_TEXTURE_CUBE_MAP_SEAMLESS,
    CullFace                   = GL_CULL_FACE,
    DepthTest                  = GL_DEPTH_TEST,
    MultiSample                = GL_MULTISAMPLE,
    ScissorTest                = GL_SCISSOR_TEST,
    StencilTest                = GL_STENCIL_TEST,
    TextureCoordArray          = GL_TEXTURE_COORD_ARRAY
  };
}

namespace GL_CullMode {
  enum Enum {
    Back                       = GL_BACK,
    Both                       = GL_FRONT_AND_BACK,
    Front                      = GL_FRONT
  };
}

namespace GL_DataFormat {
  enum Enum {
    Byte                       = GL_BYTE,
    Half                       = GL_HALF_FLOAT,
    Float                      = GL_FLOAT,
    Int                        = GL_INT,
    Short                      = GL_SHORT,
    UnsignedByte               = GL_UNSIGNED_BYTE,
    UnsignedInt                = GL_UNSIGNED_INT,
    UnsignedShort              = GL_UNSIGNED_SHORT
  };
}

namespace GL_DrawMode {
  enum Enum {
    Lines                      = GL_LINES,
    LineLoop                   = GL_LINE_LOOP,
    LineStrip                  = GL_LINE_STRIP,
    Polygon                    = GL_POLYGON,
    Points                     = GL_POINTS,
    Triangles                  = GL_TRIANGLES,
    TriangleFan                = GL_TRIANGLE_FAN,
    TriangleStrip              = GL_TRIANGLE_STRIP,
    Quads                      = GL_QUADS,
    QuadStrip                  = GL_QUAD_STRIP
  };
}

namespace GL_FaceOrientation {
  enum Enum {
    CCW                        = GL_CCW,
    CW                         = GL_CW
  };
}

namespace GL_FaceType {
  enum Enum {
    Back                       = GL_BACK,
    Front                      = GL_FRONT,
    FrontAndBack               = GL_FRONT_AND_BACK
  };
}

namespace GL_FillMode {
  enum Enum {
    Point                      = GL_POINT,
    Line                       = GL_LINE,
    Fill                       = GL_FILL
  };
}

namespace GL_FramebufferAttachment {
  enum Enum {
    ColorAttachment0           = GL_COLOR_ATTACHMENT0,
    ColorAttachment1           = GL_COLOR_ATTACHMENT1,
    ColorAttachment2           = GL_COLOR_ATTACHMENT2,
    ColorAttachment3           = GL_COLOR_ATTACHMENT3,
    ColorAttachment4           = GL_COLOR_ATTACHMENT4,
    ColorAttachment5           = GL_COLOR_ATTACHMENT5,
    ColorAttachment6           = GL_COLOR_ATTACHMENT6,
    ColorAttachment7           = GL_COLOR_ATTACHMENT7,
    ColorAttachment8           = GL_COLOR_ATTACHMENT8,
    ColorAttachment9           = GL_COLOR_ATTACHMENT9,
    DepthAttachment            = GL_DEPTH_ATTACHMENT,
    DepthStencilAttachment     = GL_DEPTH_STENCIL_ATTACHMENT,
    StencilAttachment          = GL_STENCIL_ATTACHMENT
  };
}

namespace GL_FramebufferStatus {
  enum Enum {
    Complete                   = GL_FRAMEBUFFER_COMPLETE,
    DrawBuffer                 = GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER,
    Error                      = 0,
    IncompleteAttachment       = GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT ,
    LayerTargets               = GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS,
    MissingAttachment          = GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT,
    Mutisample                 = GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE,
    ReadBuffer                 = GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER,
    Undefined                  = GL_FRAMEBUFFER_UNDEFINED,
    Unsupported                = GL_FRAMEBUFFER_UNSUPPORTED
  };

  inline const char* String(Enum e) {
    return
      e == Complete ? "Complete Framebuffer" :
      e == DrawBuffer ? "Incomplete Draw Buffer" :
      e == Error ? "Error Checking Framebuffer Status" :
      e == IncompleteAttachment ? "Incomplete Attachment" :
      e == LayerTargets ? "Incomplete Layered Targets" :
      e == MissingAttachment ? "Missing Attachments" :
      e == Mutisample ? "Incomplete Multisample" :
      e == ReadBuffer ? "Incomplete Read Buffer" :
      e == Undefined ? "Undefined Framebuffer" :
      e == Unsupported ? "Unsupported Framebuffer" :
      "Unknown Framebuffer Status";
  }
}

namespace GL_FramebufferTarget {
  enum Enum {
    Draw                       = GL_DRAW_FRAMEBUFFER,
    DrawAndRead                = GL_FRAMEBUFFER,
    Read                       = GL_READ_FRAMEBUFFER
  };
}

namespace GL_Error {
  enum Enum {
    None                       = GL_NO_ERROR,
    InvalidEnum                = GL_INVALID_ENUM,
    InvalidValue               = GL_INVALID_VALUE,
    InvalidOperation           = GL_INVALID_OPERATION,
    InvalidFramebufferOperation= GL_INVALID_FRAMEBUFFER_OPERATION,
    OutOfMemory                = GL_OUT_OF_MEMORY,
    StackUnderflow             = GL_STACK_UNDERFLOW,
    StackOverflow              = GL_STACK_OVERFLOW
  };

  inline const char* String(Enum e) {
    return
      e == None ? "No Error" :
      e == InvalidEnum ? "Invalid Enum" :
      e == InvalidValue ? "Invalid Value" :
      e == InvalidOperation ? "Invalid Operation" :
      e == InvalidFramebufferOperation ? "Invalid Framebuffer Operation" :
      e == OutOfMemory ? "Out Of Memory" :
      e == StackUnderflow ? "Stack Underflow" :
      e == StackOverflow ? "Stack Overflow" :
      "Unknown Error";
  }
}

namespace GL_IndexFormat {
  enum Enum {
    Byte                       = GL_UNSIGNED_BYTE,
    Short                      = GL_UNSIGNED_SHORT,
    Int                        = GL_UNSIGNED_INT
  };
}

namespace GL_Matrix {
  enum Enum {
    ModelView                  = GL_MODELVIEW,
    Projection                 = GL_PROJECTION,
    Texture                    = GL_TEXTURE
  };
}

namespace GL_PixelFormat {
  enum Enum {
    Alpha                      = GL_ALPHA,
    BGR                        = GL_BGR,
    BGRA                       = GL_BGRA,
    Blue                       = GL_BLUE,
    ColorIndex                 = GL_COLOR_INDEX,
    DepthComponent             = GL_DEPTH_COMPONENT,
    DepthStencil               = GL_DEPTH_STENCIL,
    Green                      = GL_GREEN,
    Red                        = GL_RED,
    RG                         = GL_RG,
    RGB                        = GL_RGB,
    RGBA                       = GL_RGBA
  };
}

namespace GL_ProgramProperty {
  enum Enum {
    ActiveUniforms             = GL_ACTIVE_UNIFORMS,
    AttachedShaders            = GL_ATTACHED_SHADERS,
    InfoLogLength              = GL_INFO_LOG_LENGTH,
    LinkStatus                 = GL_LINK_STATUS,
    ValidateStatus             = GL_VALIDATE_STATUS
  };
}

namespace GL_RenderbufferTarget {
  enum Enum {
    Renderbuffer               = GL_RENDERBUFFER
  };
}

namespace GL_ShaderProperty {
  enum Enum {
    CompileStatus              = GL_COMPILE_STATUS,
    InfoLogLength              = GL_INFO_LOG_LENGTH,
    SourceLength               = GL_SHADER_SOURCE_LENGTH
  };
}

namespace GL_ShaderType {
  enum Enum {
    Fragment                   = GL_FRAGMENT_SHADER,
    Vertex                     = GL_VERTEX_SHADER
  };
}

namespace GL_TextureCoordinate {
  enum Enum {
    S,
    T,
    R
  };
}

namespace GL_TextureFormat {
  enum Enum {
    DepthComponent16           = GL_DEPTH_COMPONENT16,
    DepthComponent24           = GL_DEPTH_COMPONENT24,
    DepthComponent32F          = GL_DEPTH_COMPONENT32F,
    R8                         = GL_R8,
    R8I                        = GL_R8I,
    R8UI                       = GL_R8UI,
    R16                        = GL_R16,
    R16F                       = GL_R16F,
    R16I                       = GL_R16I,
    R16UI                      = GL_R16UI,
    R32F                       = GL_R32F,
    R32I                       = GL_R32I,
    R32UI                      = GL_R32UI,
    RG8                        = GL_RG8,
    RG16                       = GL_RG16,
    RG16F                      = GL_RG16F,
    RGBA8                      = GL_RGBA8,
    RGBA8I                     = GL_RGBA8I,
    RGBA8UI                    = GL_RGBA8UI,
    RGB10_A2                   = GL_RGB10_A2,
    RGBA12                     = GL_RGBA12,
    RGBA16                     = GL_RGBA16,
    RGBA16F                    = GL_RGBA16F,
    RGBA16I                    = GL_RGBA16I,
    RGBA16UI                   = GL_RGBA16UI,
    RGBA32F                    = GL_RGBA32F,
    RGBA32I                    = GL_RGBA32I
  };

  inline GL_DataFormat::Enum DataFormat(Enum format) {
    switch (format) {
    case R8:        return GL_DataFormat::UnsignedByte;
    case R16F:      return GL_DataFormat::Half;
    case R32F:      return GL_DataFormat::Float;
    case RG8:       return GL_DataFormat::UnsignedByte;
    case RGBA16F:   return GL_DataFormat::Half;
    case RGBA8:     return GL_DataFormat::UnsignedByte;
    case RGBA32F:   return GL_DataFormat::Float;
    default:
      error("Unsupported Texture Format");
      return GL_DataFormat::UnsignedByte;
    }
  }

  inline GL_PixelFormat::Enum PixelFormat(Enum format) {
    switch (format) {
    case R8:        return GL_PixelFormat::Red;
    case R8I:       return GL_PixelFormat::Red;
    case R8UI:      return GL_PixelFormat::Red;
    case R16:       return GL_PixelFormat::Red;
    case R16F:      return GL_PixelFormat::Red;
    case R16I:      return GL_PixelFormat::Red;
    case R16UI:     return GL_PixelFormat::Red;
    case R32F:      return GL_PixelFormat::Red;
    case R32I:      return GL_PixelFormat::Red;
    case R32UI:     return GL_PixelFormat::Red;
    case RG8:       return GL_PixelFormat::RG;
    case RG16:      return GL_PixelFormat::RG;
    case RG16F:     return GL_PixelFormat::RG;
    case RGBA8:     return GL_PixelFormat::RGBA;
    case RGBA8I:    return GL_PixelFormat::RGBA;
    case RGBA8UI:   return GL_PixelFormat::RGBA;
    case RGB10_A2:  return GL_PixelFormat::RGBA;
    case RGBA12:    return GL_PixelFormat::RGBA;
    case RGBA16:    return GL_PixelFormat::RGBA;
    case RGBA16F:   return GL_PixelFormat::RGBA;
    case RGBA16I:   return GL_PixelFormat::RGBA;
    case RGBA16UI:  return GL_PixelFormat::RGBA;
    case RGBA32F:   return GL_PixelFormat::RGBA;
    case RGBA32I:   return GL_PixelFormat::RGBA;
    default:
      error("Unsupported Texture Format");
      return GL_PixelFormat::Red;
    }
  }

  inline size_t Size(Enum format) {
    switch (format) {
    case R8:        return 1;
    case R8I:       return 1;
    case R8UI:      return 1;
    case R16:       return 2;
    case R16F:      return 2;
    case R16I:      return 2;
    case R16UI:     return 2;
    case R32F:      return 4;
    case R32I:      return 4;
    case R32UI:     return 4;
    case RG8:       return 2;
    case RG16:      return 4;
    case RG16F:     return 4;
    case RGBA8:     return 4;
    case RGBA8I:    return 4;
    case RGBA8UI:   return 4;
    case RGB10_A2:  return 4;
    case RGBA12:    return 6;
    case RGBA16:    return 8;
    case RGBA16F:   return 8;
    case RGBA16I:   return 8;
    case RGBA16UI:  return 8;
    case RGBA32F:   return 16;
    case RGBA32I:   return 16;
    default:
      error("Unsupported Texture Format");
      return 0;
    }
  }
}

namespace GL_TextureFilter {
  enum Enum {
    Linear                     = GL_LINEAR,
    Nearest                    = GL_NEAREST
  };
}

namespace GL_TextureFilterMip {
  enum Enum {
    Linear                     = GL_LINEAR,
    LinearMipLinear            = GL_LINEAR_MIPMAP_LINEAR,
    LinearMipNearest           = GL_LINEAR_MIPMAP_NEAREST,
    Nearest                    = GL_NEAREST,
    NearestMipLinear           = GL_NEAREST_MIPMAP_LINEAR,
    NearestMipNearest          = GL_NEAREST_MIPMAP_NEAREST
  };
}

namespace GL_TextureParameter {
  enum Enum {
    BaseLevel                  = GL_TEXTURE_BASE_LEVEL,
    CompareMode                = GL_TEXTURE_COMPARE_MODE,
    CompareFunc                = GL_TEXTURE_COMPARE_FUNC,
    DepthMode                  = GL_DEPTH_TEXTURE_MODE,
    GenerateMipMap             = GL_GENERATE_MIPMAP,
    LODBias                    = GL_TEXTURE_LOD_BIAS,
    MagFilter                  = GL_TEXTURE_MAG_FILTER,
    MaxLevel                   = GL_TEXTURE_MAX_LEVEL,
    MaxLOD                     = GL_TEXTURE_MAX_LOD,
    MinFilter                  = GL_TEXTURE_MIN_FILTER,
    MinLOD                     = GL_TEXTURE_MIN_LOD,
    Priority                   = GL_TEXTURE_PRIORITY,
    WrapR                      = GL_TEXTURE_WRAP_R,
    WrapS                      = GL_TEXTURE_WRAP_S,
    WrapT                      = GL_TEXTURE_WRAP_T
  };
}

namespace GL_TextureParameterRead {
  enum Enum {
    AlphaSize                  = GL_TEXTURE_ALPHA_SIZE,
    BlueSize                   = GL_TEXTURE_BLUE_SIZE,
    Border                     = GL_TEXTURE_BORDER,
    Compressed                 = GL_TEXTURE_COMPRESSED,
    CompressedImageSize        = GL_TEXTURE_COMPRESSED_IMAGE_SIZE,
    Depth                      = GL_TEXTURE_DEPTH,
    DepthSize                  = GL_TEXTURE_DEPTH_SIZE,
    GreenSize                  = GL_TEXTURE_RED_SIZE,
    Height                     = GL_TEXTURE_HEIGHT,
    InternalFormat             = GL_TEXTURE_INTERNAL_FORMAT,
    RedSize                    = GL_TEXTURE_RED_SIZE,
    Width                      = GL_TEXTURE_WIDTH
  };
}

namespace GL_TextureTarget {
  enum Enum {
    CubeMap                    = GL_TEXTURE_CUBE_MAP,
    CubeMapPositiveX           = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    CubeMapNegativeX           = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    CubeMapPositiveY           = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    CubeMapNegativeY           = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    CubeMapPositiveZ           = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    CubeMapNegativeZ           = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
    T1D                        = GL_TEXTURE_1D,
    T2D                        = GL_TEXTURE_2D,
    T3D                        = GL_TEXTURE_3D
  };
}

/* Specific cubemap faces cannot be bound as texture targets, so this enum
 * helps us be safer for binding. */
namespace GL_TextureTargetBindable {
  enum Enum {
    CubeMap                    = GL_TEXTURE_CUBE_MAP,
    T1D                        = GL_TEXTURE_1D,
    T2D                        = GL_TEXTURE_2D,
    T3D                        = GL_TEXTURE_3D
  };
}

namespace GL_TextureWrapMode {
  enum Enum {
    ClampToBorder              = GL_CLAMP_TO_BORDER,
    ClampToEdge                = GL_CLAMP_TO_EDGE,
    MirroredRepeat             = GL_MIRRORED_REPEAT,
    Repeat                     = GL_REPEAT
  };
}

namespace GL_VertexFormat {
  enum Enum {
    Short                      = GL_SHORT,
    Int                        = GL_INT,
    Float                      = GL_FLOAT,
    Double                     = GL_DOUBLE
  };
}

#endif
