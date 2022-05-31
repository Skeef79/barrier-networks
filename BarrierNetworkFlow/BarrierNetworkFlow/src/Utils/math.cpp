#include "math.h"

static mt19937 rndGen(2022);

int Math::genInt(int l, int r) {
	uniform_int_distribution<int>uid(l, r);
	return uid(rndGen);
}

int Math::genInt(int n) {
	uniform_int_distribution<int>uid(0, n - 1);
	return uid(rndGen);
}

double Math::genDouble(double l, double r) {
	uniform_real_distribution<double>uid(l, r);
	return uid(rndGen);
}

vector<int> Math::genCapacityDistribution(int n, int cap) {
	//generate vector of random doubles first
	vector<double> c(n);
	for (int i = 0; i < n; i++)
		c[i] = genDouble();

	vector<int>result(n);
	double sum = accumulate(c.begin(), c.end(), double(0));
	//normalize and multiply by cap and lower bound double
	for (int i = 0; i < n; i++) {
		result[i] = c[i] / sum * cap;
	}

	//add remaining sum to some values
	int nsum = accumulate(result.begin(), result.end(), 0);
	for (int it = 0; it < cap - nsum; it++) {
		int i = genInt(n);
		result[i]++;
	}

	assert(accumulate(result.begin(), result.end(), 0) == cap);
	return result;
}

bool Math::isProb(double p) {
	uniform_real_distribution<double>uid(0, 1);
	return uid(rndGen) < p;
}


int Math::getRandomIndex(vector<double> distr) {
	discrete_distribution<>dist(distr.begin(), distr.end());
	return dist(rndGen);
}
