#pragma once
#include<random>
#include<vector>
#include<algorithm>
#include<numeric>
#include<assert.h>
#include "utils.h"

using namespace std;
class Math {
public:

	//[l,r]
	static int genInt(int l, int r);

	//[0, n)
	static int genInt(int n);

	//[l,r], [0,1] by default
	static double genDouble(double l = 0, double r = 1);

	//{c_1, c_2, ... , c_n}, sum of c_i equals to cap
	static vector<flowT> genCapacityDistribution(int n, flowT cap);

	static bool isProb(double p);

	static int getRandomIndex(vector<double> distr);

};