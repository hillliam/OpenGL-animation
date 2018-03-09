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
  static void MultiplyMatrix(float* result, const float* lhs, const float* rhs);
  static void Translate(float* matrix, const float tx, const float ty, const float tz);
  static void Scale(float* matrix, const float sx, const float sy, const float sz);
  static void RotateX(float* matrix, const float degs);
  static void RotateY(float* matrix, const float degs);
  static void RotateZ(float* matrix, const float degs);
  static void SetOrtho(float* matrix, float left, float right, float bottom, float top, float neara, float fara);
  static void SetFrustum(float* matrix, float left, float right, float bottom, float top, float neara, float fara);
  static void SetLookAt(float* matrix, float* eye, float* centre, float* up);
  static float dotproduct(float* a, float* b);
  static float* crosproduct(float* a, float* b);
};

inline void Matrix::SetLookAt(float* matrix, float* eye, float* centre, float* up)
{
	Matrix::SetIdentity(matrix);
	Matrix::Normalise3(up);
	float* forward = new float[3];
	forward[0] = centre[0] - eye[0];
	forward[1] = centre[1] - eye[1];
	forward[2] = centre[2] - eye[2];
	Matrix::Normalise3(forward);
	float* s = crosproduct(forward, up);
	Matrix::Normalise3(s);
	matrix[0] = s[0];
	matrix[4] = s[1];
	matrix[8] = s[2];
	float* u = crosproduct(s, forward);
	Matrix::Normalise3(u);
	matrix[1] = u[0];
	matrix[5] = u[1];
	matrix[9] = u[2];
	matrix[2] = forward[0];
	matrix[6] = forward[1];
	matrix[10] = forward[2];
	matrix[12] = dotproduct(s, eye);
	matrix[13] = dotproduct(u, eye);
	matrix[14] = dotproduct(forward, eye);

}

inline float Matrix::dotproduct(float* a, float* b)
{
	float value = 0;
	value += a[0] * b[0];
	value += a[1] * b[1];
	value += a[2] * b[2];
	return value;
}

inline float* Matrix::crosproduct(float* a, float* b)
{
	float* cros = new float[3];
	cros[0] = a[1] * b[2] - a[2] * b[1];
	cros[1] = a[2] * b[0] - a[0] * b[2];
	cros[2] = a[0] * b[1] - a[1] * b[0];
	return cros;
}

// We can inline SetIdentity because it is such a small method
// This method is done for you
#ifndef _MIKE

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
	matrix[1] = matrix[2] = matrix[3] = matrix[4] = matrix[6] = matrix[7] = matrix[8] = matrix[9] = 0;
	matrix[0] = 2 / (right - left);
	matrix[5] = 2 / (top - bottom);
	matrix[10] = -(2 / (top - bottom));
	matrix[12] = -((right + left) / (right - left));
	matrix[13] = -((top + bottom) / (top - bottom));
	matrix[14] = -((fara + neara) / (fara - neara));
	matrix[15] = 1;
}

inline void Matrix::MultiplyMatrix(float* result, const float* lhs, const float* rhs)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
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
	matrix[15] = matrix[3] * tx + matrix[7] * ty + matrix[11] * tz + matrix[15];
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
	matrix[1] = matrix[5] * cos(DEGSTORADS(degs)) + matrix[9] * -sin(DEGSTORADS(degs));
	float old5 = matrix[5];
	matrix[5] = matrix[5] * cos(DEGSTORADS(degs)) + matrix[9] * -sin(DEGSTORADS(degs));
	float old9 = matrix[9];
	matrix[9] = old5 * cos(DEGSTORADS(degs)) + matrix[9] * -sin(DEGSTORADS(degs));
	matrix[13] = old5 * cos(DEGSTORADS(degs)) + old9 * -sin(DEGSTORADS(degs));

	matrix[2] = matrix[6] * sin(DEGSTORADS(degs)) + matrix[10] * cos(DEGSTORADS(degs));
	float old6 = matrix[6];
	matrix[6] = matrix[6] * sin(DEGSTORADS(degs)) + matrix[10] * cos(DEGSTORADS(degs));
	float old10 = matrix[10];
	matrix[10] = old6 * sin(DEGSTORADS(degs)) + matrix[10] * cos(DEGSTORADS(degs));
	matrix[14] = old6 * sin(DEGSTORADS(degs)) + old10 * cos(DEGSTORADS(degs));

}

inline void Matrix::RotateY(float* matrix, const float degs)
{
	float old0 = matrix[0];
	matrix[0] = matrix[0] * cos(DEGSTORADS(degs)) + matrix[2] * -sin(DEGSTORADS(degs));
	float old8 = matrix[8];
	matrix[8] = matrix[8] * sin(DEGSTORADS(degs)) + matrix[10] * cos(DEGSTORADS(degs));

	matrix[1] = old0 * cos(DEGSTORADS(degs)) + matrix[2] * -sin(DEGSTORADS(degs));
	matrix[9] = old8 * sin(DEGSTORADS(degs)) + matrix[10] * cos(DEGSTORADS(degs));
	float old2 = matrix[2];
	matrix[2] = old0 * cos(DEGSTORADS(degs)) + old2 * -sin(DEGSTORADS(degs));
	float old10 = matrix[10];
	matrix[10] = old8 * sin(DEGSTORADS(degs)) + matrix[10] * cos(DEGSTORADS(degs));

	matrix[3] = old0 * cos(DEGSTORADS(degs)) + old2 * -sin(DEGSTORADS(degs));
	matrix[11] = old8 * sin(DEGSTORADS(degs)) + old10 * cos(DEGSTORADS(degs));
}

inline void Matrix::RotateZ(float* matrix, const float degs)
{
	float old0 = matrix[0];
	matrix[0] = matrix[0] * cos(DEGSTORADS(degs)) + matrix[4] * sin(DEGSTORADS(degs));
	matrix[4] = old0 * cos(DEGSTORADS(degs)) + matrix[4] * sin(DEGSTORADS(degs));
	float old1 = matrix[1];
	matrix[1] = matrix[1] * cos(DEGSTORADS(degs)) + matrix[5] * sin(DEGSTORADS(degs));
	matrix[5] = old1 * cos(DEGSTORADS(degs)) + matrix[5] * sin(DEGSTORADS(degs));
	float old2 = matrix[2];
	matrix[2] = matrix[2] * cos(DEGSTORADS(degs)) + matrix[6] * sin(DEGSTORADS(degs));
	matrix[6] = old2 * cos(DEGSTORADS(degs)) + matrix[6] * sin(DEGSTORADS(degs));
	float old3 = matrix[3];
	matrix[3] = matrix[3] * cos(DEGSTORADS(degs)) + matrix[7] * sin(DEGSTORADS(degs));
	matrix[7] = old3 * cos(DEGSTORADS(degs)) + matrix[7] * sin(DEGSTORADS(degs));
}
#endif