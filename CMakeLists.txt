cmake_minimum_required(VERSION 3.10)
project(OpenGLAnimation)

set(CMAKE_CXX_STANDARD 11)

# Add source files
add_executable(OpenGLAnimation
  linuxmain.cpp
  Object3D.cpp
  Model3D.cpp
  GLSetup.cpp
  Matrix.cpp
  picker.cpp
  staticgeom.cpp
  RenderingContext.cpp
  testing.cpp
)

# Add include directories
target_include_directories(OpenGLAnimation PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Add OpenGL and GLUT libraries
find_package(OpenGL REQUIRED)

# Add FreeGLUT source directory
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/freeglut-3.6.0)

# Specify the paths to the GLEW library and include directory
set(GLEW_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/glew-2.1.0/include")
set(GLEW_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/glew-2.1.0/lib/Release/x64/glew32.lib")

# Include GLEW headers
include_directories(${GLEW_INCLUDE_DIR})

# Link libraries
target_link_libraries(OpenGLAnimation
  OpenGL::GL
  freeglut
  ${GLEW_LIBRARY}
  kernel32.lib
  user32.lib
  gdi32.lib
  winspool.lib
  comdlg32.lib
  advapi32.lib
  shell32.lib
  ole32.lib
  oleaut32.lib
  uuid.lib
  odbc32.lib
  odbccp32.lib
)

# Add tests
add_subdirectory(tests)
