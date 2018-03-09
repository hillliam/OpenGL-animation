#pragma once
#include "GLSetup.h"
#include "RenderingContext.h"

class Object3D
{
public:
  Object3D();
  ~Object3D();

  void SetName(const char* name);

  void SetVertexData(byte* buffer, int noofverts, int bufferlen);
  void SetTriangles(byte* buffer, int noofpolys);

  void Draw(RenderingContext rcontext);

  void SetTranslation(byte* buffer);
  void SetTranslation(float x, float y, float z);


private:
  char* name;
  
  float translation[3];
  
  bool incuv;
  int elementtype, elementcount;
  int noofverts;
  float* vertexdata;
  unsigned short* polygons;
  
  unsigned int* vbos;
  

  void InitVBOs();

};

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
