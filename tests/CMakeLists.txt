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
)
target_link_libraries(
  MatrixTest
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(Object3DTest)
gtest_discover_tests(MatrixTest)
