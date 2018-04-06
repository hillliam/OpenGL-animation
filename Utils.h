#pragma once
#include <Windows.h>

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

inline void DisplayMessage(const char* msg)
{
  int len=strlen(msg)+1;
  wchar_t* text=(wchar_t*) malloc(2*len);
  MultiByteToWideChar(CP_ACP, 0, msg, -1, text, len);
  MessageBox(NULL, text, L"OpenGL Message", MB_ICONINFORMATION);
  free(text);
}

inline float lerpbetween(float svalue, float evalue, DWORD current, float stime, float etime)
{
	if (current < stime)
	{
		return svalue;
	}
	else if (current > etime)
	{
		return evalue;
	}
	else
	{
		float r = current * 0.005;
		double u = r - (int)r;
		float x = (1 - u) * svalue + u * evalue;
		return x;
	}
}

struct location
{
	float x;
	float y;
	float z;
};