#pragma once

#define GLEW_STATIC 

#ifdef _WIN32
#include <Windows.h>
#include "GL/glew.h"
#else
#include <GL/glew.h>
#endif

//#include "GL\eglew.h"
//#include "GL\glxew.h"
#ifdef _WIN32
#include "GL/wglew.h"
#endif
#ifndef _WIN32
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <time.h>
#endif

#ifdef _WIN32
HDC InitGL(HWND parent);
#endif
int LoadShaders(const char* vertshader, const char* fragshader);
void showerror();
