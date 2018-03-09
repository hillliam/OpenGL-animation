#include "Object3D.h"

Object3D::Object3D()
{
  vbos=NULL;
  incuv=false;
  vertexdata=NULL;
  polygons=NULL;
  name=NULL;
    
  translation[0]=translation[1]=translation[2]=0.0f;
    
  elementcount=0;
  elementtype=GL_TRIANGLES;    
}

Object3D::~Object3D()
{
  free(vbos);
  free(vertexdata);
  free(polygons);
  free(name);
}

void Object3D::InitVBOs()
{
  if (!vbos)
    vbos=(unsigned int*) malloc(2*sizeof(unsigned int));
  glGenBuffers(2, vbos);

  int size=4*noofverts*(incuv?8:6);
  glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
  glBufferData(GL_ARRAY_BUFFER, size, vertexdata, GL_STATIC_DRAW);
    
  size=2*elementcount;
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, polygons, GL_STATIC_DRAW);
}
  
void Object3D::SetVertexData(byte* buffer, int noofverts, int bufferlen)
{
  incuv=(bufferlen==4*(noofverts*8));
  vertexdata=(float*) malloc(bufferlen);
  memcpy(vertexdata, buffer, bufferlen);
  this->noofverts=noofverts;
}
  
void Object3D::SetTriangles(byte* buffer, int noofpolys)
{
  const int size=3*noofpolys*sizeof(unsigned short);
  polygons=(unsigned short*) malloc(size);
  memcpy(polygons, buffer, size);
    
  elementtype=GL_TRIANGLES;
  elementcount=3*noofpolys;
}

void Object3D::SetTranslation(byte* buffer)
{
  translation[0]=*(float*) buffer;
  translation[1]=*(float*) (buffer+4);
  translation[2]=*(float*) (buffer+8);
}
  
void Object3D::SetTranslation(float x, float y, float z)
{
  translation[0]=x;
  translation[1]=y;
  translation[2]=z;
}
  
  
void Object3D::Draw(RenderingContext rcontext)
{
  if (!vbos)
    InitVBOs();
    
  rcontext.PushModelMatrix();
    rcontext.Translate(translation[0], translation[1], translation[2]);
    rcontext.UpdateMVPs();
    glUniformMatrix4fv(rcontext.mvhandle, 1, false, rcontext.mvmatrix);
    glUniformMatrix4fv(rcontext.mvphandle, 1, false, rcontext.mvpmatrix);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);    

    // Attributes
    glVertexAttribPointer(rcontext.verthandles[0], 3, GL_FLOAT, false, 4*6, (void*) 0);
    glVertexAttribPointer(rcontext.verthandles[1], 3, GL_FLOAT, false, 4*6, (void*) (4*3));
    glEnableVertexAttribArray(rcontext.verthandles[0]);
    glEnableVertexAttribArray(rcontext.verthandles[1]);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);    
    glDrawElements(elementtype, elementcount, GL_UNSIGNED_SHORT, 0);

  rcontext.PopModelMatrix();
}