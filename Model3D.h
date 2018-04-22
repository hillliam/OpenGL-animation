#pragma once
#include "Object3D.h"
#include <stdio.h>

class Model3D
{
public:
  Model3D();
  ~Model3D();

  static Model3D* LoadModel(const wchar_t* filename);

  void Draw(RenderingContext* rcontext);

  void bindbyname(const char * name, const char * filename);

  int GetNoOfObjects();
  Object3D** GetObjects();

private:
  int noofobjects;
  Object3D** objects;

  static void Model3D::Read3DSVersion4(FILE* file, Model3D* model);
};

inline int Model3D::GetNoOfObjects()
{
  return noofobjects;
}
  
inline Object3D** Model3D::GetObjects()
{
  return objects;
}

inline void Model3D::Draw(RenderingContext* rcontext)
{
	for (int i = 0; i < noofobjects; i++)
	{
		rcontext->PushModelMatrix();
		objects[i]->Draw(rcontext);
		rcontext->PopModelMatrix();
	}
}

inline void Model3D::bindbyname(const char* name, const char * filename)
{
	for (int i = 1; i != noofobjects; i++)
	{
		Object3D* item = objects[i];
		if (_stricmp(item->getName(), name) == 0)
		{
			item->bindtexture(filename);
		}
	}
}
