# openGL animation

Liam Hill
-----------------------------

openGL animation of a cherry picker

check list
------------
- [X] draws the cherry picker
- [X] matrix look at works
- [X] draws manually created cube
- [X] keyboard control of cherry picker
- [X] keyboard input to move camera when in driver view
- [X] keyboard input to move camera around world
- [X] world lighting
- [X] texture different parts of the cherry picker
- [X] animate the cherry picker
- [X] world camera
- [X] driver camera
- [X] draws the skybox
- [X] texture the skybox
- [X] add objects to the world

extra improvements
--------------
- [X] render world to framebuffer
- [X] add different effects to world
- [X] framebuffer scales with screen
- [X] create openGL core context
- [X] updated shaders to old words
- [X] use vao to fix 3.3 errors

## Build Instructions

### Linux

To build the project on Linux, use the following command:

```sh
g++-10 -O2 -std=c++11 -L /usr/include/GL/* -L /usr/lib/libglut/* Object3D.cpp Model3D.cpp GLSetup.cpp Matrix.cpp picker.cpp staticgeom.cpp RenderingContext.cpp testing.cpp linuxmain.cpp -o main -lglut -lGL -lGLU -lGLEW
```

### Emscripten

To build the project using Emscripten, use the following command:

```sh
em++ -O2 -std=c++11 Object3D.cpp Model3D.cpp GLSetup.cpp Matrix.cpp picker.cpp staticgeom.cpp RenderingContext.cpp testing.cpp linuxmain.cpp -o index.html -s DEMANGLE_SUPPORT=1 -s FULL_ES3=1 -s USE_WEBGL2=1 -s ALLOW_MEMORY_GROWTH=1 --preload-file fragshader.txt --preload-file vertshader.txt --preload-file sfragshader.txt --preload-file svertshader.txt --preload-file nfragshader.txt --preload-file nvertshader.txt --preload-file assets/crane.3dm --preload-file assets/landscape-nouv.3dm --preload-file assets/monument.3dm --preload-file textures/grass.jpg --preload-file textures/btile.jpg --preload-file textures/lbtile.jpg --preload-file textures/green.bmp --preload-file textures/rust.jpg --preload-file textures/glass.jpg --preload-file textures/metal.jpg --preload-file textures/negx.jpg --preload-file textures/posx.jpg --preload-file textures/negx.jpg --preload-file textures/negz.jpg --preload-file textures/posz.jpg --preload-file textures/posy.jpg
```

## Running Unit Tests

### Prerequisites

Make sure you have CMake and GoogleTest installed.

### Build and Run Tests

1. Navigate to the project directory:

```sh
cd /c:/data/projects/OpenGL-animation
```

2. download freeglut 3.6.0 and place in folder freeglut-3.6.0
https://freeglut.sourceforge.net/index.php#download

3. download glew from and place in folder glew-2.1.0
https://glew.sourceforge.net/

3. Create a build directory and navigate into it:

```sh
mkdir build
cd build
```

4. Run CMake to configure the project:

```sh
cmake ..
```

5. Build the project:

```sh
make
```

6. Run the tests:

```sh
ctest
```