#pragma once

#define GLEW_STATIC 

#ifdef _WIN32
#include <Windows.h>
#endif
#include "GL/glut.h"
#include "GL\glew.h"
//#include "GL\eglew.h"
//#include "GL\glxew.h"
#include "GL\wglew.h"

HDC InitGL(HWND parent);
int LoadShaders(const wchar_t* vertshader, const wchar_t* fragshader);
