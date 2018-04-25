#pragma once

void setupshader(RenderingContext* rcontext, int program)
{
	glUseProgram(program);
	// Light
	rcontext->lighthandles[0] = glGetUniformLocation(program, "u_l_direction");
	rcontext->lighthandles[1] = glGetUniformLocation(program, "u_l_halfplane");
	rcontext->lighthandles[2] = glGetUniformLocation(program, "u_l_ambient");
	rcontext->lighthandles[3] = glGetUniformLocation(program, "u_l_diffuse");
	rcontext->lighthandles[4] = glGetUniformLocation(program, "u_l_specular");
	// Material
	rcontext->mathandles[0] = glGetUniformLocation(program, "u_m_ambient");
	rcontext->mathandles[1] = glGetUniformLocation(program, "u_m_diffuse");
	rcontext->mathandles[2] = glGetUniformLocation(program, "u_m_specular");
	rcontext->mathandles[3] = glGetUniformLocation(program, "u_m_shininess");
	// Matrices
	rcontext->mvhandle = glGetUniformLocation(program, "u_mvmatrix");
	rcontext->mvphandle = glGetUniformLocation(program, "u_mvpmatrix");

	// Attributes
	rcontext->verthandles[0] = glGetAttribLocation(program, "a_position");
	rcontext->verthandles[1] = glGetAttribLocation(program, "a_normal");
	rcontext->verthandles[2] = glGetAttribLocation(program, "a_uv");

	// texture flag
	rcontext->textureflag[0] = glGetUniformLocation(program, "u_textured");

	// screen effect
	rcontext->effect = glGetUniformLocation(program, "u_effect");

	//glBindFragDataLocation(program, 0, "result");
}