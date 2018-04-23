#include <Windows.h>  // for access to the windows APIs
#include <crtdbg.h>   // for debug stuff
#include <math.h>
#include <iostream>
#include "GLSetup.h"
#include "Utils.h"
#include "RenderingContext.h"
#include "Model3D.h"
#include "testing.h"
#include "picker.h"
#include "staticgeom.h"
#include "HUD.h"
#include "enviroment.h"

static HWND hwnd;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

RenderingContext rcontext;
Model3D* sphere, *box, *car, *Cylinder;

Object3D* cube; // static item
Object3D* screen; // static item

//world objects
staticgeom* tower, *house, *ground;

picker* mainobject;
// learnopengl.com

DWORD start;

void OnCreate();
void OnDraw();
void OnTimer(UINT nIDEvent);
void OnSize(DWORD type, UINT cx, UINT cy);
void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
void OnLButtonDown(UINT nFlags, int x, int y);
void OnMouseMove(UINT nFlags, int x, int y);
void CreateObjects();
void CleanUp();
void lights();
void sethalfplane();
void redraw();
void setupshader(int program);
void makeframebuffer();

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

// Win32 entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Test::testmatrixclass();
	//return 0;
  // This mini section is really useful to find memory leaks
#ifdef _DEBUG   // only include this section of code in the DEBUG build
//_CrtSetBreakAlloc(12);  // really useful line of code to help find memory leaks
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
  OnSize(NULL, width, hight);
  // Create the window using the definition provided above
  CreateWindowEx(NULL, L"GettingStarted", L"Getting Started with OpenGL - lighting", WS_OVERLAPPEDWINDOW|WS_VISIBLE, offx, offy, width, height, NULL, NULL, hInstance, NULL);

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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
}

// This is called then the window is first created and useful to get things ready (e.g. load or create pens, brushes, images, etc)
void OnCreate()
{
  InitGL(hwnd);

  GLenum err=glewInit();
  if (err!=GLEW_OK)
    DisplayMessage((char*) glewGetErrorString(err));

  CreateObjects();
  setupskybox();
  setupfont();
  makeframebuffer();
  rcontext.glprogram=LoadShaders(L"vertshader.txt", L"fragshader.txt");
  rcontext.nullglprogram = LoadShaders(L"nvertshader.txt", L"nfragshader.txt");
  rcontext.screenprogram = LoadShaders(L"svertshader.txt", L"sfragshader.txt");
  setupshader(rcontext.nullglprogram);
  setupshader(rcontext.glprogram);
  setupshader(rcontext.screenprogram);

  glUseProgram(rcontext.glprogram);
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
}

void setupshader(int program)
{
	// Light
	rcontext.lighthandles[0] = glGetUniformLocation(program, "u_l_direction");
	rcontext.lighthandles[1] = glGetUniformLocation(program, "u_l_halfplane");
	rcontext.lighthandles[2] = glGetUniformLocation(program, "u_l_ambient");
	rcontext.lighthandles[3] = glGetUniformLocation(program, "u_l_diffuse");
	rcontext.lighthandles[4] = glGetUniformLocation(program, "u_l_specular");
	// Material
	rcontext.mathandles[0] = glGetUniformLocation(program, "u_m_ambient");
	rcontext.mathandles[1] = glGetUniformLocation(program, "u_m_diffuse");
	rcontext.mathandles[2] = glGetUniformLocation(program, "u_m_specular");
	rcontext.mathandles[3] = glGetUniformLocation(program, "u_m_shininess");
	// Matrices
	rcontext.mvhandle = glGetUniformLocation(program, "u_mvmatrix");
	rcontext.mvphandle = glGetUniformLocation(program, "u_mvpmatrix");

	// Attributes
	rcontext.verthandles[0] = glGetAttribLocation(program, "a_position");
	rcontext.verthandles[1] = glGetAttribLocation(program, "a_normal");
	rcontext.verthandles[2] = glGetAttribLocation(program, "a_uv");

	// texture flag
	rcontext.textureflag[0] = glGetUniformLocation(program, "u_textured");
}

void drawarm()
{
	rcontext.PushModelMatrix();
	rcontext.Scale(1.0, 7.0, 1.0);
	//rcontext.RotateX(90);
	//rcontext.RotateZ(90);
	Cylinder->Draw(&rcontext);
	rcontext.PopModelMatrix();
}

void drawsphere()
{
	rcontext.Translate(0.0f, 2.5f, 0.0f);
	rcontext.PushModelMatrix();
	//rcontext.Translate(0.0f, 2.0f, 0.0f);
	rcontext.Scale(2.0, 2.0, 2.0);
	sphere->Draw(&rcontext);
	rcontext.PopModelMatrix();
}

void drawcar()
{
	rcontext.PushModelMatrix();
	rcontext.Translate(1.0f, -1.0f, 1.0f);
	rcontext.RotateX(180);
	rcontext.Scale(1.2, 1.2, 1.2);
	car->Draw(&rcontext);
	rcontext.PopModelMatrix();
}

// This is called when the window needs to be redrawn
void OnDraw()
{
	//pre draw
	glBindFramebuffer(GL_FRAMEBUFFER, rcontext.framebuffer);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
  glUseProgram(rcontext.glprogram);

  rcontext.InitModelMatrix(true);
  //drawarm();
  //rcontext.Translate(-1.0f, -0.3f, -1.0f);
  //rcontext.Scale(0.3f,0.3f,0.3f);
  glUseProgram(rcontext.nullglprogram);
  setupshader(rcontext.nullglprogram);
  drawskybox(&rcontext);
  glUseProgram(rcontext.glprogram);
  setupshader(rcontext.glprogram);
  tower->draw(&rcontext);
  ground->draw(&rcontext);
  mainobject->drawpicker(&rcontext);
  //glFinish();
  //end rendering
  glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE); // saves having to use diffrent code for plane
  //glBindTexture(GL_TEXTURE_2D, rcontext.texColorBuffer);
  glUseProgram(rcontext.screenprogram);
  setupshader(rcontext.screenprogram);
  screen->SetTextureMap(rcontext.texColorBuffer);
  //rcontext.RotateX(180);
  screen->Draw(&rcontext);
  HDC display = wglGetCurrentDC();
  drawhud(display, width, hight);
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
void OnSize(DWORD type, UINT cx, UINT cy)
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
	}
}
void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	/*case 'A':
		eye[0] += 0.1f;
		//centre[0] += 0.1f;
		break;
	case 'S':
		eye[1] += 0.1f;
		//centre[1] += 0.1f;
		break;
	case 'Z':
		eye[0] -= 0.1f;
		break;
	case 'X':
		eye[1] -= 0.1f;
		break;*/
	case 'Q':
		mainobject->foldedmirrors += 90;
		break;
	case 'W':
		mainobject->foldedmirrors -= 90;
		break;
	case 'E':
		mainobject->baserotation += 1;
		break;
	case 'R':
		mainobject->baserotation -= 1;
		break;
	case 'T':
		mainobject->sisorx += 1;
		break;
	case 'Y':
		mainobject->sisorx -= 1;
		break;
	case 'U':
		mainobject->boxy += 1;
		break;
	case 'I':
		mainobject->boxy -= 1;
		break;
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
	default:
		mainobject->keypress(nChar);
		break;
	}
	sethalfplane();
	Matrix::SetLookAt(rcontext.viewmatrix, eye, centre, up);
	redraw();
}

void OnTimer(UINT nIDEvent)
{
	mainobject->handleanimation(start);
	redraw();
}
void OnLButtonDown(UINT nFlags, int x, int y)
{
}

void OnMouseMove(UINT nFlags, int x, int y)
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
	Matrix::SetLookAt(rcontext.viewmatrix, eye, centre, up);
	redraw();
}

void redraw()
{
	PAINTSTRUCT paint;
	BeginPaint(hwnd, &paint);
	OnDraw();
	EndPaint(hwnd, &paint);
}

void CreateObjects()
{
  tower = new staticgeom(L"assets\\monument.3dm");
  ground = new staticgeom(L"assets\\landscape-nouv.3dm");
  sphere = Model3D::LoadModel(L"assets\\Sphere2-nouv.3dm");
  box = Model3D::LoadModel(L"assets\\Box-nouv.3dm");
  car = Model3D::LoadModel(L"assets\\car.3dm");
  Cylinder = Model3D::LoadModel(L"assets\\cilinder-nouv.3dm");
  mainobject = new picker();
  screen = new Object3D();
  screen->SetName("plane");
  screen->makeplane();
  cube = new Object3D();
  cube->SetName("cube");
  //cube->makecube();
  ground->setlocation(1,0.6,1);
  ground->setscale(10, 10, 10);
  tower->setlocation(-0.5, -0.7, -1);
  tower->bindbyname("cube", "textures\\window.jpg");
  ground->rename("plane","lane");
  ground->bindbyname("lane", "textures\\grass.jpg");
  ground->bindbyname("Circtair", "textures\\btile.jpg");
  ground->bindbyname("ramid", "textures\\lbtile.jpg");
  mainobject->targetpoint.x = -0.5;
  mainobject->targetpoint.y = -0.7;
  mainobject->targetpoint.z = -1;
}

void makeframebuffer()
{
	glGenFramebuffers(1, &rcontext.framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, rcontext.framebuffer);

	// generate texture
	glGenTextures(1, &rcontext.texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, rcontext.texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rcontext.texColorBuffer, 0);

	glGenRenderbuffers(1, &rcontext.rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rcontext.rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rcontext.rbo);
}

void CleanUp()
{
  glDeleteProgram(rcontext.glprogram);
  glDeleteProgram(rcontext.nullglprogram);
  cleanhud();
  delete sphere;
  delete box;
  delete car;
  delete Cylinder;
  delete screen;
  delete cube;
  delete mainobject;
}