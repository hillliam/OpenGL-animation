#include "staticgeom.h"

staticgeom::staticgeom(const char* filename)
{
	model = Model3D::LoadModel(filename);
	//startpoint.x = startpoint.y = startpoint.z = 0;
	maplocal();
}

staticgeom::~staticgeom()
{
	delete model;
}

void staticgeom::setlocation(float x, float y, float z)
{
	startpoint.x = x;
	startpoint.y = y;
	startpoint.z = z;
}

void staticgeom::setscale(float x, float y, float z)
{
	startscale.x = x;
	startscale.y = y;
	startscale.z = z;
}

void staticgeom::setrotation(float x, float y, float z)
{
	startrotation.x = x;
	startrotation.y = y;
	startrotation.z = z;
}

void staticgeom::draw(RenderingContext * rcontext)
{
	rcontext->PushModelMatrix();
	rcontext->Translate(startpoint.x, startpoint.y, startpoint.z);
	rcontext->Scale(startscale.x, startscale.y, startscale.z);
	rcontext->RotateX(startrotation.x);
	rcontext->RotateY(startrotation.y);
	rcontext->RotateZ(startrotation.z);
	model->Draw(rcontext);
	rcontext->PopModelMatrix();
}

void staticgeom::bindbyname(const char* name, const char * newname)
{
	model->bindbyname(name, newname);
}

void staticgeom::rename(const char* name, const char * newname)
{
	Object3D** models = model->GetObjects();
	for (int i = 1; i != model->GetNoOfObjects(); i++)
	{
		Object3D* item = model->GetObjects()[i];
		if (strcasecmp(item->getName(), name) == 0)
		{
			item->SetName(newname);
		}
	}
}
void staticgeom::copybyname(const char * name, const char * copyname)
{
	model->copybyname(name, copyname);
}

void staticgeom::maplocal()
{
	Object3D* base = model->GetObjects()[0];
	for (int i = 1; i != model->GetNoOfObjects(); i++)
	{
		Object3D* item = model->GetObjects()[i];
		item->getlocalmove(base);
	}
}
