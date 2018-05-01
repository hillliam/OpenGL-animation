#pragma once
#include "Model3D.h"

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

	location armjoin;

	location startpoint = { 0,-0.6,0 };
	location startrotation = { 0,270,0 };
	location startscale = { 1,1,1 };
	// scale of the base of the arm 
	double armscale = 1;
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
	void maptextures();
public:
	// where the piramid is in the world
	location targetpoint = { -600,0,-1700 };
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