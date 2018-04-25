#pragma once
#include <Windows.h>  // for access to the windows APIs
#include <iostream>
#include <string>
#include <sstream>

HFONT hfont;
HDC hdcsrc;
void drawtext(HDC display, const char* message, int x, int y);

void drawhud(HDC display, float width, float hight)
{ // draw to seperate HDC to save result
	HDC temp = ::GetDC(NULL);
	HDC src = ::CreateCompatibleDC(temp);
	::ReleaseDC(NULL, temp);
	drawtext(src, "liams openGL city", 200, 70);
	drawtext(src,"test string to see if it works",200,100);
	drawtext(src, "keys:", 250, width - 100);
	drawtext(src, "m: toggle animation", 270, width - 100);
	drawtext(src, "1: world camera", 290, width - 100);
	drawtext(src, "2: driver camera", 290, width - 100);
	::BitBlt(display, 0, 0, width, hight, src, 0, 0, SRCAND); // add text to screen
	DeleteDC(hdcsrc);
}

void cleanhud()
{// on close code
	::DeleteObject(hfont);
	::DeleteObject(hdcsrc);
}

void setupfont()
{
	HDC hdctemp = ::GetDC(NULL);
	hdcsrc = ::CreateCompatibleDC(hdctemp);
	::ReleaseDC(NULL, hdctemp);
	LOGFONT logfont;	
	logfont.lfHeight = -MulDiv(10, GetDeviceCaps(hdcsrc, LOGPIXELSY), 72);
	logfont.lfWidth = 0;
	logfont.lfEscapement = 0;
	logfont.lfOrientation = 0;
	logfont.lfWeight = FW_REGULAR;
	logfont.lfItalic = 0;
	logfont.lfUnderline = 0;
	logfont.lfStrikeOut = 0;
	logfont.lfCharSet = DEFAULT_CHARSET;
	logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	logfont.lfQuality = DEFAULT_QUALITY;
	logfont.lfPitchAndFamily = DEFAULT_PITCH;
	hfont = ::CreateFontIndirect(&logfont);
}

void drawtext(HDC display, const char* message, int x, int y)
{
	::SelectObject(display, hfont);
	::SetBkMode(display, TRANSPARENT);
	::SetTextColor(display, 0x0);
	::SetTextAlign(display, TA_TOP | TA_CENTER);
	std::wostringstream oss;
	oss << message;

	std::wstring ws = oss.str();
	::TextOut(hdcsrc, x, y, ws.c_str(), ws.length());
}