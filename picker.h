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

	int animationstage = 0;

	void drawbase(RenderingContext * rcontext);
	void draw_arm_1(RenderingContext * rcontext);
	void draw_arm_2(RenderingContext * rcontext);
	void draw_arm_3(RenderingContext * rcontext);
	void drawmirrors(RenderingContext * rcontext);
	void draw_wheels(RenderingContext * rcontext);
	void calculateoffsetpicker();
	void populatepicker();
public:
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
	void handleanimation();
	picker();
	~picker();
};
void picker::handleanimation()
{

	DWORD elapsed = GetTickCount() - start;
		switch (animationstage)
		{
		case 0: // chery picker extend legs
			break;
		case 1: // fold mirrors in
			break;
		case 2: // rotate base of arm 90 left
			break;
		case 3: // raise arm up
			break;
		}
}
void picker::drawpicker(RenderingContext* rcontext)
{
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
	cabin->Draw(rcontext);
	rcontext->PushModelMatrix();
	rcontext->PushModelMatrix();
	rcontext->RotateY(foldedmirrors);
	right_mirror->Draw(rcontext);
	rcontext->PopModelMatrix();
	rcontext->PushModelMatrix();
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