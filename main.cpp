#include <Windows.h>  // for access to the windows APIs
#include <crtdbg.h>   // for debug stuff
#include <math.h>
#include "GLSetup.h"
#include "Utils.h"
#include "RenderingContext.h"
#include "Model3D.h"

static HWND hwnd;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

RenderingContext rcontext;
Model3D* sphere, *box;

void OnCreate();
void OnDraw();
void OnSize(DWORD type, UINT cx, UINT cy);
void OnLButtonDown(UINT nFlags, int x, int y);
void OnMouseMove(UINT nFlags, int x, int y);

void CreateObjects();
void CleanUp();

// Win32 entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  // This mini section is really useful to find memory leaks
#ifdef _DEBUG   // only include this section of code in the DEBUG build
//  _CrtSetBreakAlloc(65);  // really useful line of code to help find memory leaks
  _onexit(_CrtDumpMemoryLeaks); // check for memory leaks when the program exits
#endif

  // To create a window, we must first define various attributes about it
	WNDCLASSEX classname;
  classname.cbSize=sizeof(WNDCLASSEX); 
  classname.style=CS_HREDRAW|CS_VREDRAW;
  classname.lpfnWndProc=(WNDPROC)WndProc;   // This is the name of the event-based callback method
  classname.cbClsExtra=0;
  classname.cbWndExtra=0;
  classname.hInstance=hInstance;
  classname.hIcon=LoadIcon(NULL, IDI_WINLOGO);
  classname.hCursor=LoadCursor(NULL, IDC_ARROW);
  classname.hbrBackground=NULL;//   ::GetSysColorBrush(COLOR_3DFACE); - note, setting this to null prevents flickering when resizing the window
  classname.lpszMenuName=NULL;
  classname.lpszClassName=L"GettingStarted";
  classname.hIconSm=LoadIcon(NULL, IDI_WINLOGO);
  RegisterClassEx(&classname);

  // Default size will be 1024x768 and we'll centre it on the screen - this include the caption and window borders so might not be the canvas size (which will be smaller)
  int width=1024;
  int height=768;
  int offx=(::GetSystemMetrics(SM_CXSCREEN)-width)/2;
  int offy=(::GetSystemMetrics(SM_CYSCREEN)-height)/2;

  // Create the window using the definition provided above
  CreateWindowEx(NULL, L"GettingStarted", L"Getting Started with OpenGL - lighting", WS_OVERLAPPEDWINDOW|WS_VISIBLE, offx, offy, width, height, NULL, NULL, hInstance, NULL);

  // Set the event-based message system up
  MSG msg;
  PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
  while (msg.message!=WM_QUIT)  // keep looping until we get the quit message
  {
    if (GetMessage(&msg, NULL, 0, 0)) // cause this thread to wait until there is a message to process
    {
      // These two lines of code take the MSG structure, mess with it, and correctly dispatch it to the WndProc defined during the window creation
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  CleanUp();

}

// This is our message handling method and is called by the system (via the above while-loop) when we have a message to process
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message) 
  {
  case WM_CREATE:
    hwnd=hWnd;
    OnCreate();
    return 0;
    break;
  case WM_SIZE:
    OnSize((DWORD) wParam, (UINT) (lParam&0xFFFF), (UINT) (lParam>>16));
    break;
  case WM_PAINT:
    {
      PAINTSTRUCT paint;
      BeginPaint(hwnd, &paint);
      OnDraw();
      EndPaint(hwnd, &paint);
    }
    break;
  case WM_LBUTTONDOWN:
    OnLButtonDown(wParam, lParam&0xFFFF, (lParam>>16)&0xFFFF);
    break;
  case WM_MOUSEMOVE:
    OnMouseMove(wParam, lParam&0xFFFF, (lParam>>16)&0xFFFF);
    break;
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  case WM_CLOSE:
    DestroyWindow(hwnd);
    PostQuitMessage(0);
    break;
  }
  return DefWindowProc(hWnd, message, wParam, lParam);
}

// This is called then the window is first created and useful to get things ready (e.g. load or create pens, brushes, images, etc)
void OnCreate()
{
  InitGL(hwnd);

  GLenum err=glewInit();
  if (err!=GLEW_OK)
    DisplayMessage((char*) glewGetErrorString(err));

  CreateObjects();

  rcontext.glprogram=LoadShaders(L"vertshader.txt", L"fragshader.txt");

  // Light

  // Material
        
  // Matrices
  rcontext.mvhandle=glGetUniformLocation(rcontext.glprogram, "u_mvmatrix");
  rcontext.mvphandle=glGetUniformLocation(rcontext.glprogram, "u_mvpmatrix");
        
  // Attributes
  rcontext.verthandles[0]=glGetAttribLocation(rcontext.glprogram, "a_position");
  rcontext.verthandles[1]=glGetAttribLocation(rcontext.glprogram, "a_normal");
  
  glUseProgram(rcontext.glprogram);

  glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);

  // we can do this here because the camera never moves (for the moment...)
  float eye[3]={0.0f, 1.0f, 3.0f};
  float centre[3]={0.0f, 0.0f, 0.0f};
  float up[3]={0.0f, 1.0f, 0.0f};
  Matrix::SetLookAt(rcontext.viewmatrix, eye, centre, up);
}

// This is called when the window needs to be redrawn
void OnDraw()
{
  glUseProgram(rcontext.glprogram);

  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  rcontext.InitModelMatrix(true);

  rcontext.PushModelMatrix();
    rcontext.Translate(-1.0f, 0.0f, 0.0f);
    rcontext.RotateX(180);  // puts the seam at the back
    sphere->Draw(rcontext);
  rcontext.PopModelMatrix();

  rcontext.PushModelMatrix();
    rcontext.Translate(1.0f, -1.0f, 0.0f);
    box->Draw(rcontext);
  rcontext.PopModelMatrix();

  glFinish();
  SwapBuffers(wglGetCurrentDC());
}

// Called when the window is resized
void OnSize(DWORD type, UINT cx, UINT cy)
{
  if (cx>0 && cy>0)
  {
    glViewport(0, 0, cx, cy);

    // our FOV is 60 degrees 
    const float FOVY=(60.0f*(float) M_PI/180.0f); 
    const float NEAR_CLIP=0.01f;  // for example
    const float FAR_CLIP=100.0f;  // for example

    float fAspect=(float) cx/cy;
    float top=(float) (tan(FOVY*0.5)*NEAR_CLIP);
    float bottom=-top;
    float left=fAspect*bottom;
    float right=fAspect*top;
  
    Matrix::SetFrustum(rcontext.projectionmatrix, left, right, bottom, top, NEAR_CLIP, FAR_CLIP);
  }
}

void OnLButtonDown(UINT nFlags, int x, int y)
{
}

void OnMouseMove(UINT nFlags, int x, int y)
{
}


void CreateObjects()
{
  sphere=Model3D::LoadModel(L"Sphere-nouv.3dm");
  box=Model3D::LoadModel(L"Box-nouv.3dm");
}

void CleanUp()
{
  glDeleteProgram(rcontext.glprogram);

  delete sphere;
  delete box;
}
