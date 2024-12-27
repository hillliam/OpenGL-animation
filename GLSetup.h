#pragma once

#ifdef _WIN32
#include <Windows.h>
#else
#include <GL/gl.h>
#endif

#ifdef _WIN32
#include "GL/wglew.h"
#endif
#ifndef _WIN32
#include <GL/glut.h>
#include "GL/freeglut_ext.h"
#include <time.h>
#endif

#ifdef _WIN32
HDC InitGL(HWND parent);
#endif
int LoadShaders(const char* vertshader, const char* fragshader);
void showerror();
