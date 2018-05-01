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
	static void testlookat2();
	static void testotho();
	static void testfrustum();
	static void testcomplex();
public:
	static void testmatrixclass();
};