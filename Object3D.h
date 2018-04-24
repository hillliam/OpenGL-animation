#pragma once
#include "GLSetup.h"
#include "RenderingContext.h"

class Object3D
{
public:
  Object3D();
  Object3D(bool circledraw);
  ~Object3D();
  float local[3];
  int texturemap;

  void SetName(const char* name);
  char* getName();
  void SetVertexData(const byte* buffer, int noofverts, int bufferlen);
  void SetTriangles(const byte* buffer, int noofpolys);
  void getlocalmove(const Object3D* root);
  void Draw(RenderingContext* rcontext);
  void makeplane();
  void makecube();
  void SetMaterial(const byte* buffer);
  void SetMaterial(const Object3D * copy);
  void SetDiffuse(float r, float g, float b, float a);
  const float* GetDiffuse();
  void SetTextureMap(int id);

  void SetTranslation(const byte* buffer);
  void SetTranslation(float x, float y, float z);

  void resetlocal();
  void bindtexture(const char * file);
private:
  char* name;
  
  float translation[3];
  
  bool incuv;
  int elementtype, elementcount;
  int noofverts;
  float* vertexdata;
  unsigned short* polygons;
  
  float glossiness, speclevel;
  float ambient[4], diffuse[4], specular[4];

  unsigned int* vbos;

  void InitVBOs();
  void circle();
};

inline void Object3D::SetTextureMap(int id)
{
  texturemap=id;
}

inline void Object3D::SetDiffuse(float r, float g, float b, float a)
{
  diffuse[0]=r;
  diffuse[1]=g;
  diffuse[2]=b;
  diffuse[3]=a;
}
  
inline char* Object3D::getName()
{
	return name;
}

inline const float* Object3D::GetDiffuse()
{
  return diffuse;
}

inline void Object3D::SetName(const char* name)
{
  const int len=strlen(name)+1;
  this->name=(char*) realloc(this->name, len);
  strcpy_s(this->name, len, name);

  int noofitems=0;
  int* data=(int*) malloc(sizeof(int)*noofitems);

  data=(int*) realloc(data, sizeof(int)*(noofitems+10));

  free(data);

}
