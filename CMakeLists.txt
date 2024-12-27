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
find_package(GLEW REQUIRED)

# Add FreeGLUT source directory
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/freeglut-3.6.0)

# Link libraries
target_link_libraries(OpenGLAnimation
  OpenGL::GL
  freeglut
  GLEW::GLEW
)

# Add tests
add_subdirectory(tests)
