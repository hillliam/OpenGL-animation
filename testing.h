#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "Matrix.h"

class Test
{
private:
	static void printmatrix(const float data[], int size);
	static bool compare(const float data1[], const float data2[], int size);
	static void testnormal();
	static void testcprod();
	static void testdot();
	static void testsmatrix();
	static void testmultiply();
	static void testtranslate();
	static void testtranslateu();
	static void testscale();
	static void testscaleu();
	static void testrotx();
	static void testroty();
	static void testrotz();
	static void testbmatrix();
	static void testlookat();
	static void testotho();
	static void testfrustum();
	static void testcomplex();
public:
	static void testmatrixclass();
};
void Test::printmatrix(const float data[], int size)
{
	for (int i = 0; i != size; i++)
	{
		std::cout << data[i] << std::endl;
	}
}
bool Test::compare(const float data1[], const float data2[], int size)
{
	for (int i = 0; i != size; i++)
	{
		if (data1[i] != data2[i])
		{
			return false;
		}
	}
	return true;
}
void Test::testnormal()
{
	float data[3] = { 1,2,3 };
	float rndata[3] = { 0.267261237,0.534522474,0.801783681 };
	printmatrix(data, 3);
	Matrix::Normalise3(data);
	std::cout << "Normalise3 result" << std::endl;
	printmatrix(data, 3);
	if (compare(data, rndata, 3))
	{
		std::cout << "Normalise3 pass" << std::endl;
	}
	else
	{
		std::cout << "Normalise3 fail" << std::endl;
	}
}
void Test::testcprod()
{
	float data[3] = { 0,0,0 };
	float data1[3] = { 1,2,3 };
	float data2[3] = { 3,2,1 };
	float rcdata[3] = { -4,8,-4 };
	Matrix::CrossProduct3(data1, data2, data);
	std::cout << "cross product result" << std::endl;
	printmatrix(data, 3);
	if (compare(data, rcdata, 3))
	{
		std::cout << "cross product pass" << std::endl;
	}
	else
	{
		std::cout << "cross product fail" << std::endl;
	}
}
void Test::testdot()
{
	float data1[3] = { 1,2,3 };
	float data2[3] = { 1,2,3 };
	float result = Matrix::DotProduct3(data1, data2);
	float rresult = 14;
	std::cout << "dot pruduct result: " << result << std::endl;
	if (result == rresult)
	{
		std::cout << "dot pruduct pass" << std::endl;
	}
	else
	{
		std::cout << "dot pruduct fail" << std::endl;
	}
}
void Test::testsmatrix()
{
	testnormal();

	testcprod();

	testdot();
}
void Test::testmultiply()
{
	float data[16] = { 0,0,0,0
		,0,0,0,0
		,0,0,0,0
		,0,0,0,0 };
	float data1[16] = { 1,2,3,4
		,5,6,7,8
		,9,10,11,12
		,13,14,15,16 };
	float data2[16] = { 1,2,3,4
		,5,6,7,8
		,9,10,11,12
		,13,14,15,16 };
	float rdata[16] = { 90,100,110,120
		,202,228,254,280
		,314,356,398,440
		,426,484,542,600 };
	Matrix::MultiplyMatrix(data, data1, data2);
	printmatrix(data, 16);
	if (compare(data, rdata, 16))
	{
		std::cout << "multiply pass" << std::endl;
	}
	else
	{
		std::cout << "multiply fail" << std::endl;
	}
}
void Test::testtranslate()
{
	float data[16] = { 1,2,3,4
		,5,6,7,8
		,9,10,11,12
		,13,14,15,16 };
	float rdata[16] = { 1,2,3,4
		,5,6,7,8
		,9,10,11,12
		,51,58,65,16 };
	Matrix::Translate(data, 1, 2, 3);
	printmatrix(data, 16);
	if (compare(data, rdata, 16))
	{
		std::cout << "translate pass" << std::endl;
	}
	else
	{
		std::cout << "translate fail" << std::endl;
	}
}
void Test::testtranslateu()
{
	float data[16] = { 1,2,3,4
		,5,6,7,8
		,9,10,11,12
		,13,14,15,16 };
	float rdata[16] = { 1,2,3,4
		,5,6,7,8
		,9,10,11,12
		,43,50,57,16 };
	Matrix::Translate(data, 2, 2, 2);
	printmatrix(data, 16);
	if (compare(data, rdata, 16))
	{
		std::cout << "translate u pass" << std::endl;
	}
	else
	{
		std::cout << "translate u fail" << std::endl;
	}
}
void Test::testscale()
{
	float data[16] = { 1,2,3,4
		,5,6,7,8
		,9,10,11,12
		,13,14,15,16 };
	float rdata[16] = { 1,2,3,4
		,10,12,14,16
		,27,30,33,36
		,13,14,15,16 };
	Matrix::Scale(data, 1, 2, 3);
	printmatrix(data, 16);
	if (compare(data, rdata, 16))
	{
		std::cout << "scale pass" << std::endl;
	}
	else
	{
		std::cout << "scale fail" << std::endl;
	}
}
void Test::testscaleu()
{
	float data[16] = { 1,2,3,4
		,5,6,7,8
		,9,10,11,12
		,13,14,15,16 };
	float rdata[16] = { 2,4,6,8
		,10,12,14,16
		,18,20,22,24
		,13,14,15,16 };
	Matrix::Scale(data, 2, 2, 2);
	printmatrix(data, 16);
	if (compare(data, rdata, 16))
	{
		std::cout << "scale u pass" << std::endl;
	}
	else
	{
		std::cout << "scale u fail" << std::endl;
	}
}
void Test::testrotx()
{
	float data[16] = { 1,2,3,4
		,5,6,7,8
		,9,10,11,12
		,13,14,15,16 };
	float rdata[16] = { 1,2,3,4
		,9,10,11,12
		,-5,-6,-7,-8
		,13,14,15,16 };
	Matrix::RotateX(data, 90);
	printmatrix(data, 16);
	if (compare(data, rdata, 16))
	{
		std::cout << "rotate x pass" << std::endl;
	}
	else
	{
		std::cout << "rotate x fail" << std::endl;
	}
}
void Test::testroty()
{
	float data[16] = { 1,2,3,4
		,5,6,7,8
		,9,10,11,12
		,13,14,15,16 };
	float rdata[16] = { -9,-10,-11,-12
		,5,6,7,8
		,1,2,3,4
		,13,14,15,16 };
	Matrix::RotateY(data, 90);
	printmatrix(data, 16);
  	if (compare(data, rdata, 16))
	{
		std::cout << "rotate y pass" << std::endl;
	}
	else
	{
		std::cout << "rotate y fail" << std::endl;
	}
}
void Test::testrotz()
{
	float data[16] = { 1,2,3,4
		,5,6,7,8
		,9,10,11,12
		,13,14,15,16 };
	float rdata[16] = { 5,6,7,8
		,-1,-2,-3,-4
		,9,10,11,12
		,13,14,15,16 };
	Matrix::RotateZ(data, 90);
	printmatrix(data, 16);
	if (compare(data, rdata, 16))
	{
		std::cout << "rotate z pass" << std::endl;
	}
	else
	{
		std::cout << "rotate z fail" << std::endl;
	}
}
void Test::testbmatrix()
{
	testmultiply();
	testtranslate();
	testtranslateu();
	testscale();
	testscaleu();
	testrotx();
	testroty();
	testrotz();
}
void Test::testlookat()
{
	float data[16] = { 1,2,3,4
		,5,6,7,8
		,9,10,11,12
		,13,14,15,16 };
	float eyes[3] = { 0.0f, 1.0f, 3.0f };
	float centres[3] = { 0.0f, 0.0f, 0.0f };
	float ups[3] = { 0.0f, 1.0f, 0.0f };
	float rdata[16] = { 1,0,-0,0
		,-0,0.948683262,0.316227764,0
		,0,-0.316227764,0.948683262,0
		,-0,-0,-3.16227746,1 };
	Matrix::SetLookAt(data, eyes, centres, ups);
	printmatrix(data, 16);
	if (compare(data, rdata, 16))
	{
		std::cout << "look at pass" << std::endl;
	}
	else
	{
		std::cout << "look at fail" << std::endl;
	}
}
void Test::testotho()
{
	float data[16] = { 1,2,3,4
		,5,6,7,8
		,9,10,11,12
		,13,14,15,16 };
	const float FOVY = (60.0f*(float)M_PI / 180.0f);
	const float NEAR_CLIP = 0.01f;  // for example
	const float FAR_CLIP = 100.0f;  // for example
	float fAspect = (float)1024 / 768;
	float top = (float)(tan(FOVY*0.5)*NEAR_CLIP);
	float bottom = -top;
	float left = fAspect * bottom;
	float right = fAspect * top;
	float rdata[16] = { 129.903809,0,0,0
		,0,173.205078,0,0
		,0,0,-0.020002,0
		,-0,-0,-1.00020003,1 };
	Matrix::SetOrtho(data, left, right, bottom, top, NEAR_CLIP, FAR_CLIP);
	printmatrix(data, 16);
	if (compare(data, rdata, 16))
	{
		std::cout << "ortho pass" << std::endl;
	}
	else
	{
		std::cout << "ortho fail" << std::endl;
	}
}
void Test::testfrustum()
{
	float data[16] = { 1,2,3,4
		,5,6,7,8
		,9,10,11,12
		,13,14,15,16 };
	const float FOVY = (60.0f*(float)M_PI / 180.0f);
	const float NEAR_CLIP = 0.01f;  // for example
	const float FAR_CLIP = 100.0f;  // for example
	float fAspect = (float)1024 / 768;
	float top = (float)(tan(FOVY*0.5)*NEAR_CLIP);
	float bottom = -top;
	float left = fAspect * bottom;
	float right = fAspect * top;
	float rdata[16] = { 1.29903793,0,0,0
		,0,1.73205066,0,0
		,0,0,-1.00020003,-1
		,0,0,-0.020002,0 };
	Matrix::SetFrustum(data, left, right, bottom, top, NEAR_CLIP, FAR_CLIP);
	printmatrix(data, 16);
	if (compare(data, rdata, 16))
	{
		std::cout << "frustum pass" << std::endl;
	}
	else
	{
		std::cout << "frustum fail" << std::endl;
	}
}
void Test::testcomplex()
{
	testotho();
	testlookat();
	testfrustum();
}
void Test::testmatrixclass()
{
	std::ofstream out("test.txt");
	std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
	std::cout.rdbuf(out.rdbuf());
	testsmatrix();
	testbmatrix();
	testcomplex();
	//Matrix::MultiplyMatrix(data, data1, data2); 
	//std::cout << "Multiply Matrix result" << std::endl;
	//printmatrix(data, 3);
}