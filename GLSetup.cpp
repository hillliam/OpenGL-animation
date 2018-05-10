#include "GLSetup.h"
#include <stdio.h>
//#include <io.h>
#include "Utils.h"
#include <iostream>
using namespace std;

#ifdef _WIN32
BOOL SetupPixelFormat(HDC hdc);
#endif
int LoadProgram(int type, const char* filename);
bool LinkProgram(int program, int shader, int fragment);

#ifdef _WIN32
HDC InitGL(HWND parent)
{
  HDC hdcNew=::GetDC(parent);

  if (!SetupPixelFormat(hdcNew))
    return 0;
  HGLRC hrcs = wglCreateContext(hdcNew);
  wglMakeCurrent(hdcNew, hrcs);
  GLenum err = glewInit();
  const int attributes[] =
  {
	  WGL_CONTEXT_MAJOR_VERSION_ARB, 4, 
	  WGL_CONTEXT_MINOR_VERSION_ARB, 3, 
	  //WGL_CONTEXT_PROFILE_MASK_ARB,
	  //WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
	  //WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
	  WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB, 0
  };
  if (wglCreateContextAttribsARB)
  {
	  HGLRC hrc = wglCreateContextAttribsARB(hdcNew, NULL, attributes);
	  wglMakeCurrent(NULL, NULL);
	  wglDeleteContext(hrcs);
	  wglMakeCurrent(hdcNew, hrc);
  }
  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);
  const GLubyte* message = glGetString(GL_VERSION);
  //glDisable(GL_CULL_FACE);
  return hdcNew;
}

BOOL SetupPixelFormat(HDC hdc)
{
  int pixelformat;
  PIXELFORMATDESCRIPTOR pfd = 
	{
    sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
    1,                              // version number
    PFD_DRAW_TO_WINDOW |            // support window
    PFD_SUPPORT_OPENGL |            // support OpenGL
    PFD_DOUBLEBUFFER,               // double buffered
    PFD_TYPE_RGBA,                  // RGBA type
    32,                             // 32-bit color depth
    0, 0, 0, 0, 0, 0,               // color bits ignored
    0,                              // no alpha buffer
    0,                              // shift bit ignored
    0,                              // no accumulation buffer
    0, 0, 0, 0,                     // accum bits ignored
    32,                             // 32-bit z-buffer
    32,                             // 32-bit stencil buffer  - !no stencil buffer
    0,                              // no auxiliary buffer
    PFD_MAIN_PLANE,                 // main layer
    0,                              // reserved
    0, 0, 0                         // layer masks ignored
  };

  if (!(pixelformat=ChoosePixelFormat(hdc, &pfd)))
  {
    MessageBox(NULL, L"ChoosePixelFormat failed", L"GLSetup", MB_OK);
    return FALSE;
  }

  if (!SetPixelFormat(hdc, pixelformat, &pfd))
  {
    MessageBox(NULL, L"SetPixelFormat failed", L"GLSetup", MB_OK);
    return FALSE;
  }
  return TRUE;
}
#endif

int LoadShaders(const char* vertshader, const char* fragshader)
{
  int vhandle=LoadProgram(GL_VERTEX_SHADER, vertshader); 
  int fhandle=LoadProgram(GL_FRAGMENT_SHADER, fragshader);

  if (vhandle!=0 && fhandle!=0)
  {
    int phandle=glCreateProgram();
    if (LinkProgram(phandle, vhandle, fhandle))
      return phandle;
    else
    {
      glDeleteProgram(phandle);
      glDeleteShader(fhandle);
      glDeleteShader(vhandle);
    }
  }
  return 0;
}

int LoadProgram(int type, const char* filename)
{
  int shader=glCreateShader(type);
    
  if (shader!=0)
  {
    FILE* file=NULL;
    file = fopen(filename, "rb");
    cout<< "open file: "<<filename<<endl;
    if (file)
    {
      fseek(file, 0L, SEEK_END);
      int len=ftell(file);
      rewind(file);
      char* data=(char*) malloc(len);
      int read=0;
      int pos=0;
      do
      {
        read=fread(data+pos, 1, len-pos, file);
        if (read>0)
          pos+=read;
      }
      while (read>0 && pos!=len);
      fclose(file);
      const char* sdata = (const char *)data;
      glShaderSource(shader, 1, &sdata, &len);
      glCompileShader(shader);
      free(data);
      
      int status;
      glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

      if(status==GL_FALSE) 
      {
        // get the shader info log
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        char* error=(char*) malloc(len+1);
        glGetShaderInfoLog(shader, len, &status, error);
        error[len]='\0';
        //DisplayMessage(error);
        free(error);

        glDeleteShader(shader);
        shader=0;
      }
    }
    else
    {
      cout<< "failed to open file: "<<filename<<endl;
      glDeleteShader(shader);
      shader=0;
    }
  }
  return shader;
}

bool LinkProgram(int program, int shader, int fragment)
{
  glAttachShader(program, shader);
  glAttachShader(program, fragment);
  //glBindFragDataLocation(program, 0, "result");
  glLinkProgram(program);
      
  int status;
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if(status==GL_FALSE)
  {
    int length;

    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    char* error=(char*) malloc(length+1);
    glGetProgramInfoLog(program, length, &status, error);
    error[length]='\0';
    //DisplayMessage(error);
    free(error);
    return false;
  }
  return true;
}
