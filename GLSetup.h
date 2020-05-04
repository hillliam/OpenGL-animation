#pragma once

#define GLEW_STATIC 

#include <Windows.h>
#include "GL\glew.h"
//#include "GL\eglew.h"
//#include "GL\glxew.h"
#include "GL\wglew.h"

HDC InitGL(HWND parent);
int LoadShaders(const wchar_t* vertshader, const wchar_t* fragshader);
