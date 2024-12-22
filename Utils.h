#pragma once
#ifdef _WIN32
#include <Windows.h>
#endif

//cabin look keys 

#define KEY_LUP 'W'
#define KEY_LDOWN 'S'
#define KEY_LLEFT 'A'
#define KEY_LRIGHT 'D'

#ifdef _WIN32
inline void DisplayMessage(const char* msg)
{
  MessageBox(NULL, msg, "OpenGL Message", MB_ICONINFORMATION);
}
#endif

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