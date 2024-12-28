cmake_minimum_required(VERSION 3.10)
project(OpenGLAnimationTests)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)

# Add GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/release-1.10.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Disable -Werror for GoogleTest
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set_target_properties(gtest PROPERTIES COMPILE_FLAGS "-Wno-error")
  set_target_properties(gtest_main PROPERTIES COMPILE_FLAGS "-Wno-error")
endif()

enable_testing()

# Add test files
add_executable(
  Object3DTest
  Object3DTest.cpp
)
add_executable(
  MatrixTest
  MatrixTest.cpp
)

# Link test files with GoogleTest
target_link_libraries(
  Object3DTest
  gtest_main
  OpenGL::GL
  freeglut
  GLEW::GLEW
)
target_link_libraries(
  MatrixTest
  gtest_main
  OpenGL::GL
  freeglut
  GLEW::GLEW
)

include(GoogleTest)
gtest_discover_tests(Object3DTest)
gtest_discover_tests(MatrixTest)
