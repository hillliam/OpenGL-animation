#pragma once
#include <Windows.h>

// keys that will be shared between cameras
#define KEY_MUP 'I'
#define KEY_MDOWN 'K'
#define KEY_MLEFT 'J'
#define KEY_MRIGHT 'L'

#define KEY_PUP 'I'
#define KEY_PDOWN 'K'
#define KEY_PLEFT 'J'
#define KEY_PRIGHT 'L'

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