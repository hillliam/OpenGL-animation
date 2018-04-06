#pragma once

//main class used by picker

class picker
{
private:
	Model3D* pickers;
	Object3D * arm_base, *arm_mid, *armjoint, *arm_end, *lift_box_p, *lift_box, // arm
		*base, *cabin, *left_wind, *rear_wheels, *right_wind, //root
		*left_wheel, *rightwheel, //driving
		*right_mirror, *left_mirror; // mirrors
	
	location cabinpoint;
	location lmirror;
	location rmirror;

	location lwheel;
	location rwheel;

	location startpoint = { 0,0,0 };
	location startrotation = { 0,270,0 };
	location startscale = { 1,1,1 };
	int animationstage = 0;

	void drawbase(RenderingContext * rcontext);
	void draw_arm_1(RenderingContext * rcontext);
	void draw_arm_2(RenderingContext * rcontext);
	void draw_arm_3(RenderingContext * rcontext);
	void drawmirrors(RenderingContext * rcontext);
	void draw_wheels(RenderingContext * rcontext);
	void calculateoffsetpicker();
	void populatepicker();
	void movedirection(bool up);
public:
	// is the eye set to the cabin
	bool cabineye = 0;
	// rotation of wheels 
	float wheelangle = 0;
	// are the mirrors folder 90 yes 0 no other inbetween
	int foldedmirrors = 0;
	// arm base rotation
	int baserotation = 0;
	// arm hight
	int sisorx = 0;
	// piviot box turning
	int boxy = 0;
	void drawpicker(RenderingContext * rcontext);
	void handleanimation(DWORD start);
	void geteye(float *eye, float * center);
	void setlocation(float x, float y, float z);
	void setscale(float x, float y, float z);
	void setrotation(float x, float y, float z);
	void keypress(UINT nChar);
	picker();
	~picker();
};
void picker::geteye(float *eye, float *center)
{
	eye[0] = cabinpoint.x + startpoint.x;
	eye[1] = cabinpoint.y + startpoint.y;
	eye[2] = cabinpoint.z + startpoint.z;
	//center[0] = 90; // point the camera forword
}

inline void picker::setlocation(float x, float y, float z)
{
	startpoint.x = x;
	startpoint.y = y;
	startpoint.z = z;
}

inline void picker::setscale(float x, float y, float z)
{
	startscale.x = x;
	startscale.y = y;
	startscale.z = z;
}

inline void picker::setrotation(float x, float y, float z)
{
	startrotation.x = x;
	startrotation.y = y;
	startrotation.z = z;
}

inline void picker::keypress(UINT nChar)
{
	if (cabineye)
	{ // handle driveing
		switch (nChar)
		{
		case KEY_UP:
		case KEY_DOWN:
			movedirection(nChar == KEY_UP);
			break;
		case KEY_LEFT:
			wheelangle -= 1;
			break;
		case KEY_RIGHT:
			wheelangle -= 1;
			break;
		}
	}
	switch (nChar)
	{
	case 'G':
		startpoint.x += 0.2;
		break;
	case 'H':
		startpoint.x -= 0.2;
		break;
	case 'J':
		startpoint.y += 0.2;
		break;
	case 'K':
		startpoint.y -= 0.2;
		break;
	case 'C':
		startpoint.z += 0.2;
		break;
	case 'V':
		startpoint.z -= 0.2;
		break;
	}
}
void picker::handleanimation(DWORD start)
{

	DWORD elapsed = GetTickCount() - start;
		switch (animationstage)
		{
		case 0: // chery picker move to piramid
			startpoint.x = lerpbetween(startpoint.x, startpoint.x+10, elapsed, start, 35.5);
			startpoint.x = lerpbetween(startpoint.y, startpoint.y+10, elapsed, start, 35.5);
			if (elapsed >= 35.5)
				animationstage = 1;
			break;
		case 1: // fold mirrors in
			foldedmirrors = lerpbetween(0, 90, elapsed,35.5,50);
			break;
		case 2: // rotate base of arm 90 left

			break;
		case 3: // raise arm up

			break;
		}
}
void picker::drawpicker(RenderingContext* rcontext)
{
	rcontext->Translate(startpoint.x, startpoint.y, startpoint.z);
	rcontext->Scale(startscale.x, startscale.y, startscale.z);
	rcontext->RotateX(startrotation.x);
	rcontext->RotateY(startrotation.y);
	rcontext->RotateZ(startrotation.z);
	rcontext->PushModelMatrix();
	drawbase(rcontext);
	rcontext->PushModelMatrix();
	draw_wheels(rcontext);
	rcontext->PopModelMatrix();
	rcontext->PushModelMatrix();
	draw_arm_1(rcontext);
	rcontext->PushModelMatrix();
	draw_arm_2(rcontext);
	rcontext->PushModelMatrix();
	draw_arm_3(rcontext);
	rcontext->PopModelMatrix();
	rcontext->PopModelMatrix();
	rcontext->PopModelMatrix();

	drawmirrors(rcontext);

	rcontext->PopModelMatrix();
}

inline picker::picker()
{
	pickers = Model3D::LoadModel(L"assets\\crane.3dm");
	populatepicker();
	calculateoffsetpicker();
}

inline picker::~picker()
{
	delete pickers;
}

void picker::drawbase(RenderingContext* rcontext)
{
	base->Draw(rcontext);
	rcontext->PushModelMatrix();
	rear_wheels->Draw(rcontext);
	rcontext->PopModelMatrix();
}

void picker::draw_arm_1(RenderingContext* rcontext)
{
	arm_base->Draw(rcontext);
	rcontext->RotateY(baserotation);
	arm_mid->Draw(rcontext);
}

void picker::draw_arm_2(RenderingContext* rcontext)
{
	armjoint->Draw(rcontext);
	rcontext->RotateZ(sisorx);
	arm_end->Draw(rcontext);
}

void picker::draw_arm_3(RenderingContext* rcontext)
{
	lift_box_p->Draw(rcontext);
	rcontext->RotateY(boxy);
	lift_box->Draw(rcontext);
}

void picker::drawmirrors(RenderingContext* rcontext)
{
	float loffset = 0;
	float roffset = 0;
	if (foldedmirrors != 0)
	{
		loffset = 0.03;
		roffset = 0.05;
	}
	cabin->Draw(rcontext);
	rcontext->PushModelMatrix();
	rcontext->PushModelMatrix();
	rcontext->Translate(rmirror.x, rmirror.y, rmirror.z - roffset);
	rcontext->RotateY(foldedmirrors);
	right_mirror->Draw(rcontext);
	rcontext->PopModelMatrix();
	rcontext->PushModelMatrix();
	rcontext->Translate(lmirror.x, lmirror.y, lmirror.z + loffset);
	rcontext->RotateY(foldedmirrors);
	left_mirror->Draw(rcontext);
	rcontext->PopModelMatrix();
	rcontext->PopModelMatrix();
	rcontext->PushModelMatrix();
	right_wind->Draw(rcontext);
	rcontext->PopModelMatrix();
	rcontext->PushModelMatrix();
	left_wind->Draw(rcontext);
	rcontext->PopModelMatrix();
}

void picker::draw_wheels(RenderingContext* rcontext)
{
	rcontext->PushModelMatrix();
	left_wheel->Draw(rcontext);
	rcontext->PopModelMatrix();
	rcontext->PushModelMatrix();
	rightwheel->Draw(rcontext);
	rcontext->PopModelMatrix();
}

void picker::calculateoffsetpicker()
{
	arm_base->getlocalmove(base);
	arm_mid->getlocalmove(arm_base);
	armjoint->getlocalmove(arm_mid);
	arm_end->getlocalmove(armjoint);
	lift_box_p->getlocalmove(arm_end);
	lift_box->getlocalmove(lift_box_p);
	cabin->getlocalmove(base);
	left_wind->getlocalmove(cabin);
	rear_wheels->getlocalmove(base);
	right_wind->getlocalmove(cabin);
	left_wheel->getlocalmove(base);
	rightwheel->getlocalmove(base);
	right_mirror->getlocalmove(cabin);
	left_mirror->getlocalmove(cabin);
	lmirror.x = left_mirror->local[0]; // saves editing the model
	lmirror.y = left_mirror->local[1];
	lmirror.z = left_mirror->local[2];
	rmirror.x = right_mirror->local[0];
	rmirror.y = right_mirror->local[1];
	rmirror.z = right_mirror->local[2];
	left_mirror->resetlocal();
	right_mirror->resetlocal();
	cabinpoint.x = cabin->local[0];
	cabinpoint.y = cabin->local[1];
	cabinpoint.z = cabin->local[2];
}

void picker::populatepicker()
{
	for (int i = 0; i < pickers->GetNoOfObjects(); i++)
	{
		Object3D* current = pickers->GetObjects()[i];
		if (_stricmp(current->getName(), "arm_base") == 0)
		{
			arm_base = current;
		}
		else if (_stricmp(current->getName(), "arm_mid") == 0)
		{
			arm_mid = current;
		}
		else if (_stricmp(current->getName(), "armjoint") == 0)
		{
			armjoint = current;
		}
		else if (_stricmp(current->getName(), "arm_end") == 0)
		{
			arm_end = current;
		}
		else if (_stricmp(current->getName(), "lift_pbox") == 0)
		{
			lift_box_p = current;
		}
		else if (_stricmp(current->getName(), "lift_box") == 0)
		{
			lift_box = current;
		}
		else if (_stricmp(current->getName(), "base") == 0)
		{
			base = current;
		}
		else if (_stricmp(current->getName(), "cabin") == 0)
		{
			cabin = current;
		}
		else if (_stricmp(current->getName(), "left_wind") == 0)
		{
			left_wind = current;
		}
		else if (_stricmp(current->getName(), "rear_wheel") == 0)
		{
			rear_wheels = current;
		}
		else if (_stricmp(current->getName(), "right_wind") == 0)
		{
			right_wind = current;
		}
		else if (_stricmp(current->getName(), "left_wheel") == 0)
		{
			left_wheel = current;
		}
		else if (_stricmp(current->getName(), "rightwheel") == 0)
		{
			rightwheel = current;
		}
		else if (_stricmp(current->getName(), "right_mirr") == 0)
		{
			right_mirror = current;
		}
		else if (_stricmp(current->getName(), "left_mirro") == 0)
		{
			left_mirror = current;
		}
		else
		{
			current->getName();
		}
	}
}

inline void picker::movedirection(bool up)
{
	if (up)
	{
		startpoint.y += 0.2;
		if (wheelangle > 0)
		{
			startpoint.z += 0.01 * wheelangle;
		}
		else if (wheelangle < 0)
		{
			startpoint.z -= 0.2 * -wheelangle;
		}
	}
	else
	{
		startpoint.y -= 0.2;
		if (wheelangle > 0)
		{
			startpoint.z -= 0.01 * wheelangle;
		}
		else if (wheelangle < 0)
		{
			startpoint.z += 0.2 * -wheelangle;
		}
	}
}
