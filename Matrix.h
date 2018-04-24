#pragma once
#include <math.h> // remember to add _USE_MATH_DEFINES to the project settings
#include <memory.h>

#define DEGSTORADS(x)     ((x*M_PI)/180)

/*
  The matrix is assumed to be stored as illustrated below:
  m[0] m[4] m[8]  m[12]
  m[1] m[5] m[9]  m[13]
  m[2] m[6] m[10] m[14]
  m[3] m[7] m[11] m[15]
  */
class Matrix
{
public:
  static void SetIdentity(float* matrix);
  static void Normalise3(float* vector);
  static void CrossProduct3(const float* vec1, const float* vec2, float* dst);
  static float DotProduct3(const float* vec1, const float* vec2);
  static void MultiplyMatrix(float* result, const float* lhs, const float* rhs);
  static void Translate(float* matrix, const float tx, const float ty, const float tz);
  static void Scale(float* matrix, const float sx, const float sy, const float sz);
  static void RotateX(float* matrix, const float degs);
  static void RotateY(float* matrix, const float degs);
  static void RotateZ(float* matrix, const float degs);
  static void SetOrtho(float* matrix, const float left, const float right, const float bottom, const float top, const float znear, const float zfar);
  static void SetFrustum(float* matrix, const float left, const float right, const float bottom, const float top, const float znear, const float zfar);
  static void SetLookAt(float* matrix, const float* eye, const float* centre, const float* up);
};

#ifndef _MIKE

inline void Matrix::SetLookAt(float* matrix, const float* eye, const float* centre, const float* up)
{
	float nup[3] = { up[0], up[1], up[2] };
	Matrix::Normalise3(nup);
	float* forward = new float[3];
	forward[0] = centre[0] - eye[0];
	forward[1] = centre[1] - eye[1];
	forward[2] = centre[2] - eye[2];
	Matrix::Normalise3(forward);
	float* s = new float[3];
	CrossProduct3(forward, nup, s);
	Matrix::Normalise3(s);
	float* u = new float[3];
	CrossProduct3(s, forward, u);
	Matrix::Normalise3(u);
	float ns[3] = { -s[0], -s[1], -s[2] };
	float nu[3] = { -u[0], -u[1], -u[2] };
	Matrix::SetIdentity(matrix);
	matrix[0] = s[0];
	matrix[4] = s[1]; // not negative
	matrix[8] = s[2];

	matrix[1] = u[0];
	matrix[5] = u[1]; // not right
	matrix[9] = u[2];

	matrix[2] = -forward[0];
	matrix[6] = -forward[1]; //not negative
	matrix[10] = -forward[2];
	matrix[12] = DotProduct3(ns, eye);
	matrix[13] = DotProduct3(nu, eye); // FIX 
	matrix[14] = DotProduct3(forward, eye);
	free(forward);
	free(s);
	free(u);
}

inline float Matrix::DotProduct3(const float* vec1, const float* vec2)
{
	float value = 0;
	value += vec1[0] * vec2[0];
	value += vec1[1] * vec2[1];
	value += vec1[2] * vec2[2];
	return value;
}

inline void Matrix::CrossProduct3(const float* vec1, const float* vec2, float* dst)
{
	dst[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
	dst[1] = vec1[2] * vec2[0] - vec1[0] * vec2[2];
	dst[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0];
}

// We can inline SetIdentity because it is such a small method
// This method is done for you

inline void Matrix::SetIdentity(float* matrix)
{
  memset(matrix, 0, sizeof(float)*16);
  matrix[0]=matrix[5]=matrix[10]=matrix[15]=1;
}

inline void Matrix::Normalise3(float* vector)
{
	float x = vector[0];
	float y = vector[1];
	float z = vector[2];
	x = pow(x, 2);
	y = pow(y, 2);
	z = pow(z, 2);
	float mag = sqrt(x + y + z);
	vector[0] = vector[0] / mag;
	vector[1] = vector[1] / mag;
	vector[2] = vector[2] / mag;
}

inline void Matrix::SetFrustum(float* matrix, float left, float right, float bottom, float top, float neara, float fara)
{
	memset(matrix, 0, sizeof(float) * 16);
	matrix[0] = (2 * neara) / (right - left);
	matrix[5] = (2 * neara) / (top - bottom);
	matrix[8] = (right + left) / (right - left);
	matrix[9] = (top + bottom) / (top - bottom);
	matrix[10] = -(fara + neara) / (fara - neara);
	matrix[11] = -1;
	matrix[14] = -(2* fara * neara) / (fara - neara);
}

inline void Matrix::SetOrtho(float* matrix, float left, float right, float bottom, float top, float neara, float fara)
{
	memset(matrix, 0, sizeof(float) * 16);
	matrix[0] = 2 / (right - left);
	matrix[5] = 2 / (top - bottom);
	matrix[10] = (top + bottom) - (right + left);//-(2 / (top - bottom)); // fix me
	matrix[12] = ((right + left) / (right - left));
	matrix[13] = ((top + bottom) / (top - bottom));
	matrix[14] = -(fara + neara) / (fara - neara);
	matrix[15] = 1;
}

inline void Matrix::MultiplyMatrix(float* result, const float* lhs, const float* rhs)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result[i * 4 + j] = 0;
			for (int a = 0; a < 4; a++)
			{
				result[i * 4 + j] += lhs[a * 4+j] * rhs[i*4 + a];
			}
		}
	}
}

inline void Matrix::Translate(float* matrix, const float tx, const float ty, const float tz)
{
	matrix[12] = matrix[0] * tx + matrix[4] * ty + matrix[8] * tz + matrix[12];
	matrix[13] = matrix[1] * tx + matrix[5] * ty + matrix[9] * tz + matrix[13];
	matrix[14] = matrix[2] * tx + matrix[6] * ty + matrix[10] * tz + matrix[14];
	//matrix[15] = matrix[3] * tx + matrix[7] * ty + matrix[11] * tz + matrix[15];
}

inline void Matrix::Scale(float* matrix, const float sx, const float sy, const float sz)
{
	matrix[0] *= sx;
	matrix[4] *= sy;
	matrix[8] *= sz;
	matrix[1] *= sx;
	matrix[5] *= sy;
	matrix[9] *= sz;
	matrix[2] *= sx;
	matrix[6] *= sy;
	matrix[10] *= sz;
	matrix[3] *= sx;
	matrix[7] *= sy;
	matrix[11] *= sz;
}

inline void Matrix::RotateX(float* matrix, const float degs)
{
	const float sine = sin(DEGSTORADS(degs));
	const float cost = cos(DEGSTORADS(degs));
	float old4 = matrix[4];
	float old5 = matrix[5];
	float old6 = matrix[6];
	float old7 = matrix[7];

	matrix[4] = matrix[8] * sine + matrix[4] * cost;
	matrix[5] = matrix[9] * sine + matrix[5] * cost;
	matrix[6] = matrix[10] * sine + matrix[6] * cost;
	matrix[7] = matrix[11] * sine + matrix[7] * cost;

	matrix[8] = -old4 * sine + matrix[8] * cost;
	matrix[9] = -old5 * sine + matrix[9] * cost;
	matrix[10] = -old6 * sine + matrix[10] * cost;
	matrix[11] = -old7 * sine + matrix[11] * cost;
}

inline void Matrix::RotateY(float* matrix, const float degs)
{
	const float sine = sin(DEGSTORADS(degs));
	const float cost = cos(DEGSTORADS(degs));
	float old0 = matrix[0];
	float old1 = matrix[1];
	float old2 = matrix[2];
	float old3 = matrix[3];
	matrix[0] = -matrix[8] * sine + matrix[0] * cost;
	matrix[1] = -matrix[9] * sine + matrix[1] * cost;
	matrix[2] = -matrix[10] * sine + matrix[2] * cost;
	matrix[3] = -matrix[11] * sine + matrix[3] * cost;

	matrix[8] = old0* sine + matrix[8] * cost;
	matrix[9] = old1* sine + matrix[9] * cost;
	matrix[10] = old2 * sine + matrix[10] * cost;
	matrix[11] = old3 * sine + matrix[11] * cost;
}

inline void Matrix::RotateZ(float* matrix, const float degs)
{
	const float sine = sin(DEGSTORADS(degs));
	const float cost = cos(DEGSTORADS(degs));
	float old0 = matrix[0];
	matrix[0] = matrix[0] * cost + matrix[4] * sine;
	matrix[4] = matrix[4] * cost + -old0 * sine;
	float old1 = matrix[1];
	matrix[1] = matrix[1] * cost + matrix[5] * sine;
	matrix[5] = matrix[5] * cost + -old1 * sine;
	float old2 = matrix[2];
	matrix[2] = matrix[2] * cost + matrix[6] * sine;
	matrix[6] = matrix[6] * cost + -old2 * sine;
	float old3 = matrix[3];
	matrix[3] = matrix[3] * cost + matrix[7] * sine;
	matrix[7] = matrix[7] * cost + -old3 * sine;
}
#endif