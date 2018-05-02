#pragma once
#include "Model3D.h"
#include "Utils.h"

class staticgeom
{
public:
	staticgeom(const char* filename);
	~staticgeom();
	void setlocation(float x, float y, float z);
	void setscale(float x, float y, float z);
	void setrotation(float x, float y, float z);
	void draw(RenderingContext * rcontext);
	void bindbyname(const char* name, const char * filename);
	void rename(const char * name, const char * newname);
	void copybyname(const char * name, const char * copyname);

	location startpoint = {0,0,-1};
	location startrotation = { 0,0,0 };
	location startscale = { 1,1,1 };
	Model3D* model;
private:
	void maplocal();
};