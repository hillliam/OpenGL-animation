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
		if (evalue > svalue)
		{
			double dif = stime - etime;
			double increase = r - dif;
			int u = (increase / dif * 100); // must be between 0 and 1
			double value = (100 - u) / 100.0;
			float x = (1 - value) * svalue + value * evalue;
			return x;
		}
		else
		{
			double dif = etime - stime;
			double decrease = dif - r;
			int u = (decrease / dif * 100); // must be between 0 and 1
			double value = (100 - u) / 100.0;
			float x = (1 - value) * svalue + value * evalue;
			return x;
		}
	}
}

struct location
{
	double x;
	double y;
	double z;
};