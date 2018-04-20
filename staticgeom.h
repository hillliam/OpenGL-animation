#pragma once

class staticgeom
{
public:
	staticgeom(const wchar_t* filename);
	~staticgeom();
	void setlocation(float x, float y, float z);
	void setscale(float x, float y, float z);
	void setrotation(float x, float y, float z);
	void draw(RenderingContext * rcontext);
	location startpoint = {0,0,-1};
	location startrotation = { 0,0,0 };
	location startscale = { 1,1,1 };
	Model3D* model;
private:
	void maplocal();
};

staticgeom::staticgeom(const wchar_t* filename)
{
	model = Model3D::LoadModel(filename);
	//startpoint.x = startpoint.y = startpoint.z = 0;
	maplocal();
}

staticgeom::~staticgeom()
{
	delete model;
}

inline void staticgeom::setlocation(float x, float y, float z)
{
	startpoint.x = x;
	startpoint.y = y;
	startpoint.z = z;
}

inline void staticgeom::setscale(float x, float y, float z)
{
	startscale.x = x;
	startscale.y = y;
	startscale.z = z;
}

inline void staticgeom::setrotation(float x, float y, float z)
{
	startrotation.x = x;
	startrotation.y = y;
	startrotation.z = z;
}

inline void staticgeom::draw(RenderingContext * rcontext)
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

inline void staticgeom::maplocal()
{
	Object3D* base = model->GetObjects()[0];
	for (int i = 1; i != model->GetNoOfObjects(); i++)
	{
		Object3D* item = model->GetObjects()[i];
		item->getlocalmove(base);
	}
}
