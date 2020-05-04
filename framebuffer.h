#pragma once
#include "shaders.h"

Object3D* screen; // static item

void prerender(RenderingContext* rcontext)
{
	glBindFramebuffer(GL_FRAMEBUFFER, rcontext->framebuffer);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}
void postrender(RenderingContext* rcontext, int effect)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE); // saves having to use diffrent code for plane
							 //glBindTexture(GL_TEXTURE_2D, rcontext.texColorBuffer);
	glUseProgram(rcontext->screenprogram);
	setupshader(rcontext, rcontext->screenprogram);
	glUniform1i(rcontext->effect, effect);
	screen->SetTextureMap(rcontext->texColorBuffer);
	//rcontext.RotateX(180);
	screen->Draw(rcontext);
}

void updateframebuffer(RenderingContext* rcontext, int width, int hight)
{
	glBindTexture(GL_TEXTURE_2D, rcontext->texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, hight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glBindRenderbuffer(GL_RENDERBUFFER, rcontext->rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, hight);
}

void makeframebuffer(RenderingContext* rcontext, int width, int hight)
{
	screen = new Object3D();
	screen->SetName("plane");
	screen->makeplane();
	glGenFramebuffers(1, &rcontext->framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, rcontext->framebuffer);

	// generate texture
	glGenTextures(1, &rcontext->texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, rcontext->texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, hight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rcontext->texColorBuffer, 0);

	glGenRenderbuffers(1, &rcontext->rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rcontext->rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, hight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rcontext->rbo);
}

void freeframebuffer(RenderingContext* rcontext)
{
	delete screen;
	glDeleteFramebuffers(1, &rcontext->framebuffer);
	glDeleteRenderbuffers(1, &rcontext->rbo);

}