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

HDC InitGL(HWND parent);
int LoadShaders(const wchar_t* vertshader, const wchar_t* fragshader);
