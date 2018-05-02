#include "picker.h"

void picker::geteye(float *eye, float *center)
{
	eye[0] = cabinpoint.x + startpoint.x;
	eye[1] = cabinpoint.y + startpoint.y;
	eye[2] = cabinpoint.z + startpoint.z;
	center[0] = startrotation.y; // point the camera forword
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
{ // handle moving parts
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
	case 'O':
		armscale -= 0.1;
		break;
	case 'P':
		armscale += 0.1;
		break;
	case 'Q':
		foldedmirrors += 90;
		break;
	case 'W':
		foldedmirrors -= 90;
		break;
	case 'E':
		baserotation += 1;
		break;
	case 'R':
		baserotation -= 1;
		break;
	case 'T':
		sisorx += 1;
		break;
	case 'Y':
		sisorx -= 1;
		break;
	case 'U':
		boxy += 1;
		break;
	case 'I':
		boxy -= 1;
		break;
	}
}

void picker::handleanimation(unsigned int start)
{

	unsigned int elapsed = GetTickCount() - start;
	float r = elapsed * 0.005;
		switch (animationstage)
		{
		case 0: // chery picker move to piramid
			startpoint.x = lerpbetween(0, targetpoint.x, r, 0, 40);
			//startpoint.y = lerpbetween(0, targetpoint.y, r, start, 40);
			startpoint.z = lerpbetween(0, targetpoint.z, r, 0, 40);
			if (r > 40.0)
				animationstage++;
			break;
		case 1: // fold mirrors in
			foldedmirrors = lerpbetween(0, 90, r,40,70);
			if (r > 70.0)
				animationstage++;
			break;
		case 2: // rotate base of arm 90 left
			baserotation = lerpbetween(0, 90, r, 70, 120);
			if (r > 120.0)
				animationstage++;
			break;
		case 3: // raise arm up
			sisorx = lerpbetween(0, -35, r, 120, 160);
			if (r > 160.0)
				animationstage++;
			break;
		case 4: // wait
			if (r > 190.0)
				animationstage++;
			break;
		case 5: // lower arm
			sisorx = lerpbetween(-35, 0, r, 190, 230);
			if (r > 230.0)
				animationstage++;
			break;
		}
}

void picker::drawpicker(RenderingContext* rcontext)
{
	rcontext->PushModelMatrix();
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
	rcontext->PopModelMatrix();
}

inline picker::picker()
{
	pickers = Model3D::LoadModel("assets\\crane.3dm");
	populatepicker();
	calculateoffsetpicker();
	maptextures();
}

inline void picker::maptextures()
{
	pickers->bindbyname("arm_mid", "textures\\rust.jpg"); 
	pickers->copybyname("arm_end", "arm_mid"); // save memory by copying texture id
	pickers->copybyname("arm_base", "arm_mid");
	pickers->copybyname("armjoint", "arm_mid");
	pickers->copybyname("lift_box", "arm_mid");
	pickers->bindbyname("left_wind", "textures\\glass.jpg");
	pickers->copybyname("right_wind", "left_wind");
	pickers->bindbyname("cabin", "textures\\metal.jpg");
	pickers->copybyname("base", "cabin");
	pickers->copybyname("right_mirr", "cabin");
	pickers->copybyname("left_mirro", "cabin");
	//pickers->bindbyname("rear_wheel", "textures\\tyre_tread.jpg"); texturing wheels is not working
	//pickers->bindbyname("left_wheel", "textures\\tyre_tread.jpg");
	//pickers->bindbyname("rightwheel", "textures\\tyre_tread.jpg");
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
	rcontext->PushModelMatrix();
	rcontext->RotateZ(sisorx);
	rcontext->Scale(armscale, 1, 1); // this should only be applied to the arm end
	arm_end->Draw(rcontext);
	rcontext->PopModelMatrix();
	rcontext->RotateZ(sisorx);
	rcontext->Translate(arm_end->local[0], arm_end->local[1], arm_end->local[2]);
}

void picker::draw_arm_3(RenderingContext* rcontext)
{
	rcontext->Translate(armjoin.x * armscale, armjoin.y, armjoin.z);
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
	cabinpoint.x = cabin->local[0]; // camera point
	cabinpoint.y = cabin->local[1];
	cabinpoint.z = cabin->local[2];
	armjoin.x = lift_box_p->local[0];
	armjoin.y = lift_box_p->local[1];
	armjoin.z = lift_box_p->local[2];
	lift_box_p->resetlocal();
}

void picker::populatepicker()
{
	for (int i = 0; i < pickers->GetNoOfObjects(); i++)
	{
		Object3D* current = pickers->GetObjects()[i];
		if (strcasecmp(current->getName(), "arm_base") == 0)
		{
			arm_base = current;
		}
		else if (strcasecmp(current->getName(), "arm_mid") == 0)
		{
			arm_mid = current;
		}
		else if (strcasecmp(current->getName(), "armjoint") == 0)
		{
			armjoint = current;
		}
		else if (strcasecmp(current->getName(), "arm_end") == 0)
		{
			arm_end = current;
		}
		else if (strcasecmp(current->getName(), "lift_pbox") == 0)
		{
			lift_box_p = current;
		}
		else if (strcasecmp(current->getName(), "lift_box") == 0)
		{
			lift_box = current;
		}
		else if (strcasecmp(current->getName(), "base") == 0)
		{
			base = current;
		}
		else if (strcasecmp(current->getName(), "cabin") == 0)
		{
			cabin = current;
		}
		else if (strcasecmp(current->getName(), "left_wind") == 0)
		{
			left_wind = current;
		}
		else if (strcasecmp(current->getName(), "rear_wheel") == 0)
		{
			rear_wheels = current;
		}
		else if (strcasecmp(current->getName(), "right_wind") == 0)
		{
			right_wind = current;
		}
		else if (strcasecmp(current->getName(), "left_wheel") == 0)
		{
			left_wheel = current;
		}
		else if (strcasecmp(current->getName(), "rightwheel") == 0)
		{
			rightwheel = current;
		}
		else if (strcasecmp(current->getName(), "right_mirr") == 0)
		{
			right_mirror = current;
		}
		else if (strcasecmp(current->getName(), "left_mirro") == 0)
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
		startpoint.z += 0.2;
		if (wheelangle > 0)
		{
			startpoint.x += 0.2 * wheelangle;
		}
		else if (wheelangle < 0)
		{
			startpoint.x -= 0.2 * -wheelangle;
		}
	}
	else
	{
		startpoint.z -= 0.2;
		if (wheelangle > 0)
		{
			startpoint.x -= 0.2 * wheelangle;
		}
		else if (wheelangle < 0)
		{
			startpoint.x += 0.2 * -wheelangle;
		}
	}
}
