cmake_minimum_required (VERSION 2.8)
project(LTheory)

find_package(OpenGL REQUIRED)

# Set build path
set(BUILD_DIR ${CMAKE_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${BUILD_DIR} CACHE PATH "Build directory" FORCE)
set(LIBRARY_OUTPUT_PATH ${BUILD_DIR} CACHE PATH "Build directory" FORCE)

# Detect OS
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  set(WINDOWS 1)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(LINUX 1)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
  set(LINUX 1)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(MAC 1)
else()
  message(FATAL_ERROR "Unsupported operating system")
  return()
endif()

# Detect Architecture
include(CheckTypeSize)
check_type_size(void* SIZEOF_VOID_PTR)
if("${SIZEOF_VOID_PTR}" STREQUAL "4")
  set(ARCH_32 1)
elseif("${SIZEOF_VOID_PTR}" STREQUAL "8")
  set(ARCH_64 1)
else()
  message(FATAL_ERROR "Unsupported architecture")
  return()
endif()

# Get ext includes
include_directories(src/liblt include include/FreeType)

# Get ext libraries
if(WINDOWS)
  
  file(GLOB_RECURSE EXTLIB extlib/win32/*.lib)
  set(LINK_LIBRARIES ${GLEW_GLEW_LIBRARY})

elseif(MAC)

  set(LINK_LIBRARIES
    # enet
      fmodex
      fmodevent
      freetype
      GLEW
      sfml-graphics
      sfml-network
      sfml-system
      sfml-window
      )

  link_directories(${PROJECT_SOURCE_DIR}/extbin/osx)

elseif(LINUX)

  set(LINK_LIBRARIES
      dl
      # enet
      freetype
      sfml-graphics
      sfml-network
      sfml-system
      sfml-window
      GLEW)

  if(ARCH_32)

    set(LINK_LIBRARIES ${LINK_LIBRARIES} fmodex fmodevent)
    set(LINK_LIBRARIES ${LINK_LIBRARIES} TelemetryX86.link)
    link_directories(${PROJECT_SOURCE_DIR}/extbin/linux32)

  elseif(ARCH_64)

    set(LINK_LIBRARIES ${LINK_LIBRARIES} fmodex64 fmodevent64)
    set(LINK_LIBRARIES ${LINK_LIBRARIES} TelemetryX64.link)
    link_directories(${PROJECT_SOURCE_DIR}/extbin/linux64)

  endif()

endif()

set(LINK_LIBRARIES ${LINK_LIBRARIES} ${OPENGL_LIBRARIES})

# Build shared libary
file(GLOB_RECURSE LIBLT_SRC src/liblt/*.cpp)
add_library(lt SHARED ${LIBLT_SRC})
target_link_libraries(lt ${LINK_LIBRARIES} ${EXTLIB})

if (MSVC)
  set_target_properties(lt PROPERTIES PREFIX "../")
endif()

# Build binaries
set(TARGETS
    launch)

foreach(TARGET ${TARGETS})
  
  add_executable(${TARGET} ${PROJECT_SOURCE_DIR}/src/${TARGET}/${TARGET}.cpp src/resources.rc)
  target_link_libraries(${TARGET} ${LINK_LIBRARIES})

  # If we're on MSVC, build in the build dir instead of /Release /Debug, etc.
  if (MSVC)
    set_target_properties(${TARGET} PROPERTIES PREFIX "../")
  endif()

endforeach(TARGET)

# Link dynamic targets to the shared lib
foreach(TARGET ${TARGETS})

  target_link_libraries(${TARGET} lt)

endforeach(TARGET)

# Set target properties
if(WINDOWS)

  # No exception handling
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHs")
  
  # Multi-threaded compilation
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fp:fast")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:SSE2")

  # No libcmt warnings
  set_target_properties(lt PROPERTIES LINK_FLAGS "/NODEFAULTLIB:libcmt")
  foreach(TARGET ${EXECUTABLES})
    set_target_properties(${TARGET} PROPERTIES LINK_FLAGS "/NODEFAULTLIB:libcmt")
  endforeach(TARGET)

  # The updater requires administrator privileges to run
  # set_target_properties(updater PROPERTIES LINK_FLAGS "/level='requireAdministrator' /uiAccess='false'")

else()

  # Gold linker
  if (LINUX)
    set(CMAKE_LINKER "/usr/bin/gold")
  endif()

  # No exception handling 
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")

  # Enable C++0x features
  add_definitions("-std=c++0x")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g")

  # Enable lots of warnings
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -Werror")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-invalid-offsetof")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-const-variable")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-tautological-compare")

  # Stop on errors
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wfatal-errors")

  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wconversion")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-sign-conversion")

  # Enable SSE / SSE2
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse -msse2")

endif()
