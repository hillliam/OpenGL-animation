#pragma once

Object3D* lsbox;
Object3D* rsbox;
Object3D* fsbox;
Object3D* bsbox;
Object3D* usbox;

void freeskybox()
{
	delete lsbox;
	delete rsbox;
	delete fsbox;
	delete bsbox;
	delete usbox;
}

void setupskybox()
{//right
	rsbox = new Object3D();
	rsbox->SetName("plane");
	rsbox->makeplane();
	rsbox->bindtexture("textures\\negx.jpg");
	//left
	lsbox = new Object3D();
	lsbox->SetName("plane");
	lsbox->makeplane();
	lsbox->bindtexture("textures\\posx.jpg");
	//forward
	fsbox = new Object3D();
	fsbox->SetName("plane");
	fsbox->makeplane();
	fsbox->bindtexture("textures\\negz.jpg");
	//back
	bsbox = new Object3D();
	bsbox->SetName("plane");
	bsbox->makeplane();
	bsbox->bindtexture("textures\\posz.jpg");
	//up
	usbox = new Object3D();
	usbox->SetName("plane");
	usbox->makeplane();
	usbox->bindtexture("textures\\posy.jpg");
}

void drawupskybox(RenderingContext* rcontext)
{
	rcontext->PushModelMatrix();
	rcontext->Translate(0, 5, 0);
	rcontext->RotateX(270);
	rcontext->Scale(5, 5, 5);
	usbox->Draw(rcontext);
	rcontext->PopModelMatrix();
}

void drawbackskybox(RenderingContext* rcontext)
{
	rcontext->PushModelMatrix();
	rcontext->Translate(0, 0, 5);
	rcontext->Scale(5, 5, 5);
	bsbox->Draw(rcontext);
	rcontext->PopModelMatrix();
}

void drawforwardskybox(RenderingContext* rcontext)
{
	rcontext->PushModelMatrix();
	rcontext->Translate(0, 0, -5);
	rcontext->RotateX(180);
	rcontext->Scale(5, 5, 5);
	fsbox->Draw(rcontext);
	rcontext->PopModelMatrix();
}

void drawleftskybox(RenderingContext* rcontext)
{
	rcontext->PushModelMatrix();
	rcontext->Translate(-5, 0, 0);
	rcontext->RotateX(90);
	rcontext->RotateY(270);
	rcontext->Scale(5, 5, 5);
	lsbox->Draw(rcontext);
	rcontext->PopModelMatrix();
}

void drawrightskybox(RenderingContext* rcontext)
{
	rcontext->PushModelMatrix();
	rcontext->Translate(5, 0, 0);
	rcontext->RotateX(90);
	rcontext->RotateY(90);
	rcontext->Scale(5, 5, 5);
	rsbox->Draw(rcontext);
	rcontext->PopModelMatrix();
}

void drawskybox(RenderingContext* rcontext)
{
	drawrightskybox(rcontext);
	drawleftskybox(rcontext);
	drawforwardskybox(rcontext);
	drawbackskybox(rcontext);
	drawupskybox(rcontext);
}