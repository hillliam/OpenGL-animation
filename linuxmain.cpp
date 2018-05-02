#include <math.h>
#include <iostream>
#include "GLSetup.h"
#include "Utils.h"
#include "RenderingContext.h"
#include "Model3D.h"
#include "testing.h"
#include "picker.h"
#include "staticgeom.h"
//#include "HUD.h"
#include "enviroment.h"
#include "framebuffer.h"
#include "shaders.h"

static HWND hwnd;

RenderingContext rcontext;

Object3D* cube; // static item

//world objects
staticgeom* tower, *ground;

picker* mainobject;
// learnopengl.com

unsigned int start;

void OnDraw();
void OnTimer(unsigned int nIDEvent);
void OnSize(unsigned int cx, unsigned int cy);
void OnKeyDown(unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags);
void OnLButtonDown(unsigned int nFlags, int x, int y);
void OnMouseMove(unsigned int nFlags, int x, int y);
void CreateObjects();
void CleanUp();
void lights();
void sethalfplane();
void redraw();

const float defaulteye[3] = { 0.0f, 1.0f, 3.0f };
float eye[3] = { defaulteye[0], defaulteye[1], defaulteye[2] };
float centre[3] = { 0.0f, 0.0f, 0.0f };
float up[3] = { 0.0f, 1.0f, 0.0f };
float lightpos[3] = { 4.0f, 4.0f, 3.0f };
float width = 0;
float hight = 0;
const float FOVY = (60.0f*(float)M_PI / 180.0f);
float fAspect = 0;
// our FOV is 60 degrees 
const float NEAR_CLIP = 0.01f;  // for example
const float FAR_CLIP = 100.0f;  // for example

float ray_pos[3];
float ray_dir[3];

int vbo[2] = { 0 };
float verts[6];
bool animating = false;
// starting location on screen of mouse
float lastx = 0;
float lasty = 0;
// active screen effect
int activeeffect = 0;
// framebuffer flag
// Win32 entry point
/*int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Test::testmatrixclass();

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
  classname.lpszClassName=L"GL world";
  classname.hIconSm=LoadIcon(NULL, IDI_WINLOGO);
  RegisterClassEx(&classname);

  // Default size will be 1024x768 and we'll centre it on the screen - this include the caption and window borders so might not be the canvas size (which will be smaller)
  int width=1024;
  int height=768;
  int offx=(::GetSystemMetrics(SM_CXSCREEN)-width)/2;
  int offy=(::GetSystemMetrics(SM_CYSCREEN)-height)/2;
  OnSize(NULL, width, hight);
  // Create the window using the definition provided above
  CreateWindowEx(NULL, L"GL world", L"Liam OpenGL world", WS_OVERLAPPEDWINDOW|WS_VISIBLE, offx, offy, width, height, NULL, NULL, hInstance, NULL);

  // Set the event-based message system up
  MSG msg;
  PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
  while (msg.message!=WM_QUIT)  // keep looping until we get the quit message
  {
    if (GetMessage(&msg, NULL, 0, 0)) // cause this thread to wait until there is a message to process
	//while (PeekMessage(&msg, NULL, 0, 0,0))    
	{
      // These two lines of code take the MSG structure, mess with it, and correctly dispatch it to the WndProc defined during the window creation
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  CleanUp();
  return 0;
}

// This is our message handling method and is called by the system (via the above while-loop) when we have a message to process
LRESULT CALLBACK WndProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam)
{
  switch (message) 
  {
  case WM_TIMER:
	  OnTimer(wParam);
	  break;
  case WM_CREATE:
    hwnd=hWnd;
    OnCreate();
    return 0;
    break;
  case WM_SIZE:
    OnSize((DWORD) wParam, (unsigned int) (lParam&0xFFFF), (unsigned int) (lParam>>16));
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
  case WM_KEYDOWN:
	  OnKeyDown(wParam, lParam & 0xFFFF, lParam >> 16);
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
}*/

// This is called then the window is first created and useful to get things ready (e.g. load or create pens, brushes, images, etc)
int main(int argc, char **argv)
{
  //InitGL(hwnd);
  glutInit(&argc, argv);
  glutCreateWindow("Liam OpenGL world");
  GLenum err=glewInit();
  if (err!=GLEW_OK)
    DisplayMessage((char*) glewGetErrorString(err));

  CreateObjects();
  setupskybox();
  setupfont();
  makeframebuffer(&rcontext, width, hight);
  rcontext.glprogram=LoadShaders("vertshader.txt", "fragshader.txt");
  rcontext.nullglprogram = LoadShaders("nvertshader.txt", "nfragshader.txt");
  rcontext.screenprogram = LoadShaders("svertshader.txt", "sfragshader.txt");
  setupshader(&rcontext, rcontext.nullglprogram);
  setupshader(&rcontext, rcontext.glprogram);
  setupshader(&rcontext, rcontext.screenprogram);

  // populate light

//glEnable(GL_BLEND);

  /*glGenBuffers(2, (unsigned int*)vbo);
  int size = sizeof(float) * 2 * 3;
  glLineWidth(4.0f);
  for (int i = 0; i < 3; i++)
  {
	  verts[i] = ray_pos[i];
	  verts[i + 3] = ray_pos[i] + ray_dir[i] * 1000;
  }
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, size, verts, GL_DYNAMIC_DRAW);*/

  glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
  // we can do this here because the camera never moves (for the moment...)
  Matrix::SetLookAt(rcontext.viewmatrix, eye, centre, up);
  sethalfplane();
  lights();
  return 0;
}

// This is called when the window needs to be redrawn
void OnDraw()
{
	//pre draw
	prerender(&rcontext);
  rcontext.InitModelMatrix(true);
  //drawarm();
  //rcontext.Translate(-1.0f, -0.3f, -1.0f);
  //rcontext.Scale(0.3f,0.3f,0.3f);
  setupshader(&rcontext, rcontext.nullglprogram);
  drawskybox(&rcontext);
  setupshader(&rcontext, rcontext.glprogram);
  rcontext.PushModelMatrix();
  rcontext.Translate(-3.5,0.3,-2);
  rcontext.Scale(0.5f, 0.5f, 0.5f);
  rcontext.RotateX(180);
  cube->Draw(&rcontext);
  rcontext.PopModelMatrix();
  tower->draw(&rcontext);
  ground->draw(&rcontext);
  mainobject->drawpicker(&rcontext);
  //glFinish();
  //end rendering
  postrender(&rcontext, activeeffect);
  HDC display = wglGetCurrentDC();
  //drawhud(display, width, hight);
  SwapBuffers(display);
}

void sethalfplane()
{
	float h[3];
	//float nh[3];
	//float neye[3] = { eye[0], eye[1], eye[2] };
	//float nlight[3] = { lightpos[0], lightpos[1], lightpos[2] };
	float mag = 0;
	for (int i = 0; i < 3; i++)
	{
		h[i] = eye[i] + lightpos[i];
		mag += h[i] * h[i];
//		nh[i] = neye[i] + nlight[i];
	}
	//Matrix::Normalise3(nh);
	for (int i = 0; i < 3; i++)
	{
		h[i] /= mag;// nh[i];
	}

	glUniform3f(rcontext.lighthandles[1], h[0], h[1], h[2]); // set halfplane
}

void Raytrace(double x, double y)
{
	float vlength = tan(FOVY / 2) * NEAR_CLIP;
	float h_length = fAspect*vlength;

	float view[3] = { centre[0] -eye[0], centre[1] - eye[1], centre[2] - eye[2] };
	float h[3];
	float v[3];

	Matrix::CrossProduct3(rcontext.viewmatrix, up, h);
	Matrix::CrossProduct3(h, rcontext.viewmatrix, v);

	Matrix::Normalise3(view);
	Matrix::Normalise3(h);
	Matrix::Normalise3(v);

	for (int i = 0; i < 3; i++)
	{
		v[i] = v[i] * vlength;
		h[i] = h[i] * h_length;
	}

	for (int i = 0; i < 3; i++)
	{
		ray_dir[i] = h[i] * x + v[i] * y + view[i] * NEAR_CLIP;
	}

	ray_pos[0] = eye[0] + ray_dir[0];
	ray_pos[1] = eye[1] + ray_dir[1];
	ray_pos[2] = eye[2] + ray_dir[2];
}

void lights()
{
	glUniform3fv(rcontext.lighthandles[0],3, lightpos); // set light direction
	glUniform4f(rcontext.lighthandles[2], 0.7, 0.7, 0.7, 1); // set ambiant
	glUniform4f(rcontext.lighthandles[3], 1.0, 1.0, 1.0, 1); // set difuse
	glUniform4f(rcontext.lighthandles[4], 1.0, 1.0, 1.0, 1); // set specular
}

// Called when the window is resized
void OnSize(unsigned int cx, unsigned int cy)
{
	if (cx > 0 && cy > 0)
	{
		glViewport(0, 0, cx, cy);

		fAspect = (float)cx / cy;
		float top = (float)(tan(FOVY*0.5)*NEAR_CLIP);
		float bottom = -top;
		float left = fAspect * bottom;
		float right = fAspect * top;

		width = cx;
		hight = cy;

		Matrix::SetFrustum(rcontext.projectionmatrix, left, right, bottom, top, NEAR_CLIP, FAR_CLIP);
		updateframebuffer(&rcontext, width, hight);
	}
}
void OnKeyDown(unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags)
{
	if (mainobject->cabineye)
	{
		switch (nChar)
		{
		case KEY_LUP:
			centre[1] += 0.5f;
			break;
		case KEY_LDOWN:
			centre[1] -= 0.5f;
			break;
		case KEY_LLEFT:
			centre[2] -= 0.5f;
			break;
		case KEY_LRIGHT:
			centre[2] += 0.5f;
			break;
		}
	}
	else
	{
		switch (nChar)
		{
		case KEY_LLEFT:
			eye[0] -= 0.5f;
			break;
		case 'Z':
			eye[0] += 0.5f;
			break;
		case KEY_LDOWN:
			eye[1] += 0.5f;
			break;
		case 'X':
			eye[1] -= 0.5f;
			break;
		case KEY_LRIGHT:
			eye[2] += 0.5f;
			break;
		case 'C':
			eye[2] -= 0.5f;
			break;
		case 'F':
			centre[1] += 0.5f;
			break;
		case 'V':
			centre[1] -= 0.5f;
			break;
		case 'G':
			centre[2] -= 0.5f;
			break;
		case 'B':
			centre[2] += 0.5f;
			break;
		}
	}
	switch (nChar)
	{
	case '1': // default view 
		mainobject->cabineye = false;
		eye[0] = defaulteye[0];
		eye[1] = defaulteye[1];
		eye[2] = defaulteye[2];
		break;
	case '2':// view inside picker
		mainobject->cabineye = true;
		mainobject->geteye(eye, centre);
		break;
	case 'M':
		if (!animating)
		{
			SetTimer(hwnd, 101, 60, NULL);
			start = ::GetTickCount();
			animating = true;
		}
		else
		{
			KillTimer(hwnd, 101);
			animating = false;
		}
		break;
	case '3': // wireframe mode
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case '4': // normal mode
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case '7': // normal mode
		activeeffect = 0;
		break;
	case '8': // grayscale
		activeeffect = 1;
		break;
	case '9': // inverted
		activeeffect = 2;
		break;
	default:
		mainobject->keypress(nChar);
		break;
	}
	sethalfplane();
	redraw();
}

void OnTimer(unsigned int nIDEvent)
{
	mainobject->handleanimation(start);
	redraw();
}
void OnLButtonDown(unsigned int nFlags, int x, int y)
{
}

void OnMouseMove(unsigned int nFlags, int x, int y)
{
	if (nFlags == 1) // mouse key is down
	{// diffrence between last and now
		eye[0] = (lastx - x);
		eye[1] = (lasty - y);
	}
	else if (nFlags == 2)
	{
		centre[0] = (lastx - x);
		centre[1] = (lasty - y);
	}
	else
	{ // record starting x and y
		lastx = x;
		lasty = y;
	}

	sethalfplane();
	redraw();
}

void redraw()
{
	if (mainobject->cabineye)
		mainobject->geteye(eye, centre);
	Matrix::SetLookAt(rcontext.viewmatrix, eye, centre, up);
	PAINTSTRUCT paint;
	BeginPaint(hwnd, &paint);
	OnDraw();
	EndPaint(hwnd, &paint);
}

void CreateObjects()
{
  tower = new staticgeom("assets\\monument.3dm");
  ground = new staticgeom("assets\\landscape-nouv.3dm");
  mainobject = new picker();
  cube = new Object3D();
  cube->SetName("cube");
  cube->makecube();
  cube->SetMaterial(tower->model->GetObjects()[0]);
  ground->setlocation(1,0.6,1);
  ground->setscale(10, 10, 10);
  tower->setlocation(-0.5, -0.7, -1);
  tower->rename("cube", "block");
  //tower->bindbyname("block", "textures\\window.jpg");
  ground->rename("plane","lane");
  ground->bindbyname("lane", "textures\\grass.jpg");
  ground->bindbyname("Circtair", "textures\\btile.jpg");
  ground->bindbyname("ramid", "textures\\lbtile.jpg");
  ground->bindbyname("Diamond", "textures\\green.bmp");
  ground->copybyname("Gear", "ramid");
  ground->copybyname("Mesh", "Circtair");
  ground->copybyname("Paraloid", "lane");
  ground->copybyname("Spindle", "lane");
  ground->copybyname("Star", "Circtair");
  ground->copybyname("Suzanne", "Circtair");
  ground->copybyname("Thread", "Circtair");
  mainobject->targetpoint.x = -0.5;
  mainobject->targetpoint.y = -0.7;
  mainobject->targetpoint.z = -1;
}

void CleanUp()
{
  glDeleteProgram(rcontext.glprogram);
  glDeleteProgram(rcontext.nullglprogram);
  glDeleteProgram(rcontext.screenprogram);
  cleanhud();
  freeskybox();
  freeframebuffer(&rcontext);
  delete ground;
  delete tower;
  delete cube;
  delete mainobject;
}