#pragma once
#include <Windows.h>

// keys that will be shared between cameras
#define KEY_MUP 'I'
#define KEY_MDOWN 'K'
#define KEY_MLEFT 'J'
#define KEY_MRIGHT 'L'

#define KEY_PUP 'w'
#define KEY_PDOWN 'a'
#define KEY_PLEFT 's'
#define KEY_PRIGHT 'd'

inline void DisplayMessage(const char* msg)
{
  int len=strlen(msg)+1;
  wchar_t* text=(wchar_t*) malloc(2*len);
  MultiByteToWideChar(CP_ACP, 0, msg, -1, text, len);
  MessageBox(NULL, text, L"OpenGL Message", MB_ICONINFORMATION);
  free(text);
}

inline float lerpbetween(float svalue, float evalue, double r, float stime, float etime)
{
	if (r <= stime)
	{
		return svalue;
	}
	else if (r >= etime)
	{
		return evalue;
	}
	else
	{
		//double u = r - (int)r;
		double dif = etime - stime;
		double decrease = dif - r;
		double u = (decrease / dif * 100) - 100; // must be between 0 and 1
		float x = (1 - u) * svalue + u * evalue;
		return x;
	}
}

struct location
{
	double x;
	double y;
	double z;
};