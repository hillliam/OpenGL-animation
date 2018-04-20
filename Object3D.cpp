#include "Object3D.h"

Object3D::Object3D()
{
  vbos=NULL;
  incuv=false;
  vertexdata=NULL;
  polygons=NULL;
  name=NULL;
  translation[0]=translation[1]=translation[2]=0.0f;
  local[0] = local[1] = local[2] = 0.0f;
  speclevel=5;
  glossiness=25;

  elementcount=0;
  elementtype=GL_TRIANGLES;    

  ambient[0]=ambient[1]=ambient[2]=0.2f;
  ambient[3]=1.0f;
  
  diffuse[0]=diffuse[1]=diffuse[2]=0.8f;
  diffuse[3]=1.0f;

  specular[0]=specular[1]=specular[2]=0;
  specular[3]=1.0f;
    
  texturemap=-1;
}

void Object3D::circle()
{
	const int n = 3*10;
	const int NOOFVERTS = (n + 1);
	const int NOOFTRIANGLES = n;
	const int size2 = 3 * NOOFVERTS * sizeof(float);
	vertexdata = (float*)malloc(size2);
	this->noofverts = NOOFVERTS;
	const int size = 3 * NOOFTRIANGLES * sizeof(unsigned short);
	polygons = (unsigned short*)malloc(size);
	elementcount = 3 * NOOFTRIANGLES;
	
	vertexdata[0] = 0;
	vertexdata[1] = 0;
	vertexdata[2] = 0;

	for (int i = 0; i < n; i += 3)
	{
		float rads = DEGSTORADS(i*(360.0 / n));
		float x = sin(rads * 0.9);
		float y = cos(rads * 0.9);
		float z = tan(rads * 0.9);
		vertexdata[i] = x;
		vertexdata[i+1] = y;
		vertexdata[i+2] = z;
	}
	for (int i = 2; i < n; i++)
	{
			polygons[i - 2] = (i - 2) *3;
			polygons[i - 1] = (i - 1) * 3;
			polygons[i] = i * 3;
	}
	polygons[n - 1] = 0;
	polygons[n] = 1;
	polygons[n + 1] = n;
}

Object3D::Object3D(bool circledraw)
{
	vbos = NULL;
	incuv = false;
	vertexdata = NULL;
	polygons = NULL;
	name = NULL;

	SetName("circle");

	translation[0] = translation[1] = translation[2] = 0.0f;
	local[0] = local[1] = local[2] = 0.0f;
	speclevel = 5;
	glossiness = 25;

	elementcount = 0;
	elementtype = GL_TRIANGLES;

	ambient[0] = ambient[1] = ambient[2] = 0.2f;
	ambient[3] = 1.0f;

	diffuse[0] = diffuse[1] = diffuse[2] = 0.8f;
	diffuse[3] = 1.0f;

	specular[0] = specular[1] = specular[2] = 0;
	specular[3] = 1.0f;

	texturemap = -1;

	//cube();
	circle();
}

Object3D::~Object3D()
{
  free(vbos);
  free(vertexdata);
  free(polygons);
  free(name);
}

void Object3D::getlocalmove(const Object3D* root)
{
	local[0] = this->translation[0] - root->translation[0];
	local[1] = this->translation[1] - root->translation[1];
	local[2] = this->translation[2] - root->translation[2];
}

void Object3D::InitVBOs()
{
	if (!vbos)
		vbos = (unsigned int*)malloc(2 * sizeof(unsigned int));
	glGenBuffers(2, vbos);

	if (_stricmp(getName(), "circle") == 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*4*4, vertexdata, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short)* 6, polygons, GL_STATIC_DRAW);
	}
	else
	{
		int size = 4 * noofverts*(incuv ? 8 : 6);
		glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
		glBufferData(GL_ARRAY_BUFFER, size, vertexdata, GL_STATIC_DRAW);

		size = 2 * elementcount;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, polygons, GL_STATIC_DRAW);
	}
}
  
void Object3D::SetVertexData(const byte* buffer, int noofverts, int bufferlen)
{
  incuv=(bufferlen==4*(noofverts*8));
  vertexdata=(float*) malloc(bufferlen);
  memcpy(vertexdata, buffer, bufferlen);
  this->noofverts=noofverts;
}
  
void Object3D::SetTriangles(const byte* buffer, int noofpolys)
{
  const int size=3*noofpolys*sizeof(unsigned short);
  polygons=(unsigned short*) malloc(size);
  memcpy(polygons, buffer, size);
    
  elementtype=GL_TRIANGLES;
  elementcount=3*noofpolys;
}

void Object3D::SetTranslation(const byte* buffer)
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

void Object3D::resetlocal()
{
	local[0] = local[1] = local[2] = 0.0f;
}
  
void Object3D::SetMaterial(const byte* buffer)
{
  int i;
  int offset=0;
  for (i=0; i<4; i++, offset+=4)
  {
    ambient[i]=*(float*) (buffer+offset);
    diffuse[i]=*(float*) (buffer+offset+16);
    specular[i]=*(float*) (buffer+offset+32);
  }

  WORD tmp=*(WORD*) (buffer+48);
  glossiness=(float) tmp;
  if (glossiness>100)
    glossiness=100;

  tmp=*(WORD*) (buffer+50);
  speclevel=(float) tmp;

  if (speclevel>100)
    speclevel=100;

  float spec=speclevel/100.0f;
  for (int i=0; i<4; i++)
    specular[i]*=spec;
}
  
void Object3D::Draw(RenderingContext* rcontext)
{
  // If this method gives you any errors, make sure you update with the correct shader program handles for the uniforms and attributes
  // You may also need to make sure your matrix class is working correctly

  // Material
  glUniform4fv(rcontext->mathandles[0], 1, ambient);
  glUniform4fv(rcontext->mathandles[1], 1, diffuse);
  glUniform4fv(rcontext->mathandles[2], 1, specular);
  glUniform1f(rcontext->mathandles[3], glossiness);

  if (!vbos)
    InitVBOs();
    
  //rcontext->PushModelMatrix();
   // rcontext->Translate(translation[0], translation[1], translation[2]);
  rcontext->Translate(local[0], local[1], local[2]);
  //rcontext->PushModelMatrix();
    rcontext->UpdateMVPs();
    glUniformMatrix4fv(rcontext->mvhandle, 1, false, rcontext->mvmatrix);
    glUniformMatrix4fv(rcontext->mvphandle, 1, false, rcontext->mvpmatrix);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]); 
	// texture 
	if (texturemap != -1)
	{
	}
    // Attributes
    if (incuv)
    {
      glVertexAttribPointer(rcontext->verthandles[0], 3, GL_FLOAT, false, 4*8, (void*) 0);
      glVertexAttribPointer(rcontext->verthandles[1], 3, GL_FLOAT, false, 4*8, (void*) (4*3));
     //glVertexAttribPointer(rcontext->verthandles[2], 2, GL_FLOAT, false, 4*8, (void*) (4*6));
      glEnableVertexAttribArray(rcontext->verthandles[0]);
      glEnableVertexAttribArray(rcontext->verthandles[1]);
     //glEnableVertexAttribArray(rcontext->verthandles[2]);
    }
	else if (_stricmp(getName(), "circle") == 0)
	{
		glVertexAttribPointer(rcontext->verthandles[0], 3, GL_FLOAT, false, 3 * sizeof(float), (void*)0);
		//glVertexAttribPointer(rcontext->verthandles[0], 3, GL_FLOAT, false, 4, (void*)0);
		glEnableVertexAttribArray(rcontext->verthandles[0]);
	}
    else
    {
      glVertexAttribPointer(rcontext->verthandles[0], 3, GL_FLOAT, false, 4*6, (void*) 0);
      glVertexAttribPointer(rcontext->verthandles[1], 3, GL_FLOAT, false, 4*6, (void*) (4*3));
      glEnableVertexAttribArray(rcontext->verthandles[0]);
      glEnableVertexAttribArray(rcontext->verthandles[1]);
      //glDisableVertexAttribArray(rcontext->verthandles[2]);
    }
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);    
    glDrawElements(elementtype, elementcount, GL_UNSIGNED_SHORT, 0);

 // rcontext->PopModelMatrix();
}

void Object3D::makeplane()
{
	vertexdata = (float*)malloc((sizeof(float) * 3) * 4);
	vertexdata[0] = 0.5;
	vertexdata[1] = 0.5;
	vertexdata[2] = 0;
	vertexdata[3] = 0.5; 
	vertexdata[4] = -0.5; 
	vertexdata[5] = 0;
	vertexdata[6] = -0.5;
	vertexdata[7] = -0.5;
	vertexdata[8] = 0;
	vertexdata[9] = -0.5;
	vertexdata[10] = 0.5;
	vertexdata[11] = 0;
	this->noofverts = 4;

	polygons = (unsigned short*)malloc((sizeof(unsigned short)* 3)*2);
	polygons[0] = 0;
	polygons[1] = 1;
	polygons[2] = 3;
	polygons[3] = 1;
	polygons[4] = 2;
	polygons[5] = 3;

	elementtype = GL_TRIANGLES;
	elementcount = 3 * 2;
}
