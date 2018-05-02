#pragma once

#define GLEW_STATIC 

#ifdef _WIN32
#include <Windows.h>
#endif
#include "GL/glew.h"
//#include "GL\eglew.h"
//#include "GL\glxew.h"
#ifdef _WIN32
#include "GL/wglew.h"
#endif
#include "GL/glut.h"

#ifdef _WIN32
HDC InitGL(HWND parent);
#endif
int LoadShaders(const char* vertshader, const char* fragshader);
