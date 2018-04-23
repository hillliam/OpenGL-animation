#include "Object3D.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

	//SetName("circle");

	translation[0] = translation[1] = translation[2] = 0.0f;
	local[0] = local[1] = local[2] = 0.0f;
	speclevel = 5;
	glossiness = 25;

	elementcount = 0;
	elementtype = GL_TRIANGLE_STRIP;

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

	if (_stricmp(getName(), "plane") == 0 || _stricmp(getName(), "cube") == 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*5* noofverts, vertexdata, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short)* elementcount, polygons, GL_STATIC_DRAW);
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

void Object3D::bindtexture(const char* file)
{
	int width, height, nrChannels;
	unsigned char *data = stbi_load(file, &width, &height, &nrChannels, 0);
	glGenTextures(1, (unsigned int*) &texturemap);
	glBindTexture(GL_TEXTURE_2D, texturemap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
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
		glUniform1i(rcontext->textureflag[0],true);
		glBindTexture(GL_TEXTURE_2D, texturemap);
	}
	else
	{
		glUniform1i(rcontext->textureflag[0], false);
	}
    // Attributes
    if (incuv)
    {
      glVertexAttribPointer(rcontext->verthandles[0], 3, GL_FLOAT, false, 4*8, (void*) 0);
      glVertexAttribPointer(rcontext->verthandles[1], 3, GL_FLOAT, false, 4*8, (void*) (4*3));
      glVertexAttribPointer(rcontext->verthandles[2], 2, GL_FLOAT, false, 4*8, (void*) (4*6));
      glEnableVertexAttribArray(rcontext->verthandles[0]);
      glEnableVertexAttribArray(rcontext->verthandles[1]);
      glEnableVertexAttribArray(rcontext->verthandles[2]);
    }
	else if (_stricmp(getName(), "plane") == 0 || _stricmp(getName(), "cube") == 0)
	{
		glVertexAttribPointer(rcontext->verthandles[0], 3, GL_FLOAT, false, 5 * sizeof(float), (void*)0);
		//glVertexAttribPointer(rcontext->verthandles[0], 3, GL_FLOAT, false, 4, (void*)0);
		glEnableVertexAttribArray(rcontext->verthandles[0]);
		glVertexAttribPointer(rcontext->verthandles[2], 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(rcontext->verthandles[2]);
	}
    else
    {
      glVertexAttribPointer(rcontext->verthandles[0], 3, GL_FLOAT, false, 4*6, (void*) 0);
      glVertexAttribPointer(rcontext->verthandles[1], 3, GL_FLOAT, false, 4*6, (void*) (4*3));
      glEnableVertexAttribArray(rcontext->verthandles[0]);
      glEnableVertexAttribArray(rcontext->verthandles[1]);
      //glDisableVertexAttribArray(rcontext->verthandles[2]);
	  glVertexAttribPointer(rcontext->verthandles[2], 3, GL_FLOAT, false, 4 * 6, (void*)(4 * 3)); // test texture
    }
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);    
    glDrawElements(elementtype, elementcount, GL_UNSIGNED_SHORT, 0);

 // rcontext->PopModelMatrix();
}

void Object3D::makeplane()
{ // https://learnopengl.com/Getting-started/Textures
	vertexdata = (float*)malloc((sizeof(float) * 5) * 4);
	vertexdata[0] = 1;
	vertexdata[1] = 1;
	vertexdata[2] = 0;
	vertexdata[3] = 1; // u
	vertexdata[4] = 1; // v
	vertexdata[5] = 1; 
	vertexdata[6] = -1; 
	vertexdata[7] = 0;
	vertexdata[8] = 1; // u
	vertexdata[9] = 0; // v
	vertexdata[10] = -1;
	vertexdata[11] = -1;
	vertexdata[12] = 0;
	vertexdata[13] = 0; // u
	vertexdata[14] = 0; // v
	vertexdata[15] = -1;
	vertexdata[16] = 1;
	vertexdata[17] = 0;
	vertexdata[18] = 0; // u
	vertexdata[19] = 1; // v

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

void Object3D::makecube()
{ // https://learnopengl.com/Advanced-OpenGL/Framebuffers
	vertexdata = (float*)malloc((sizeof(float) * 5) * 4*2);
	// forward face
	vertexdata[0] = 1; 
	vertexdata[1] = 1;
	vertexdata[2] = -1;
	vertexdata[3] = 1; // u
	vertexdata[4] = 1; // v
	vertexdata[5] = 1;
	vertexdata[6] = -1;
	vertexdata[7] = -1;
	vertexdata[8] = 1; // u
	vertexdata[9] = 0; // v
	vertexdata[10] = -1;
	vertexdata[11] = -1;
	vertexdata[12] = -1;
	vertexdata[13] = 0; // u
	vertexdata[14] = 0; // v
	vertexdata[15] = -1;
	vertexdata[16] = 1;
	vertexdata[17] = -1;
	vertexdata[18] = 0; // u
	vertexdata[19] = 1; // v
	// back face
	vertexdata[20] = 1;
	vertexdata[21] = 1;
	vertexdata[22] = 1;
	vertexdata[23] = 1; // u
	vertexdata[24] = 1; // v
	vertexdata[25] = 1;
	vertexdata[26] = -1;
	vertexdata[27] = 1;
	vertexdata[28] = 1; // u
	vertexdata[29] = 0; // v
	vertexdata[30] = -1;
	vertexdata[31] = -1;
	vertexdata[32] = 1;
	vertexdata[33] = 0; // u
	vertexdata[34] = 0; // v
	vertexdata[35] = -1;
	vertexdata[36] = 1;
	vertexdata[37] = 1;
	vertexdata[38] = 0; // u
	vertexdata[39] = 1; // v

	this->noofverts = 4*2;

	polygons = (unsigned short*)malloc((sizeof(unsigned short) * 3) * 2 * 6);
	// front
	polygons[0] = 0;
	polygons[1] = 1;
	polygons[2] = 3;
	polygons[3] = 1;
	polygons[4] = 2;
	polygons[5] = 3;
	//back
	polygons[6] = 4;
	polygons[7] = 5;
	polygons[8] = 7;
	polygons[9] = 5;
	polygons[10] = 6;
	polygons[11] = 7;
	// left side can be made up of points from front and back
	polygons[12] = 2;
	polygons[13] = 6;
	polygons[14] = 3;
	polygons[15] = 6;
	polygons[16] = 3;
	polygons[17] = 7;
	// right side
	polygons[18] = 0;
	polygons[19] = 4;
	polygons[20] = 1;
	polygons[21] = 5;
	polygons[22] = 4;
	polygons[23] = 1;
	//bottom
	polygons[24] = 3;
	polygons[25] = 7;
	polygons[26] = 4;
	polygons[27] = 3;
	polygons[28] = 4;
	polygons[29] = 0;
	//top
	polygons[30] = 2;
	polygons[31] = 1;
	polygons[32] = 5;
	polygons[33] = 5;
	polygons[34] = 2;
	polygons[35] = 6;

	elementtype = GL_TRIANGLES;
	elementcount = (3 * 2) * 6;
}
