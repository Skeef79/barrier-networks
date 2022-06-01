#include<stdio.h>
#include<iomanip>
#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<random>
#include "Graph/graph.h"
#include "Algo/breakthrough.h"
#include "Algo/utils.h"
#include "Utils/math.h"
#include "Algo/capacity_distribution.h"

#pragma warning(disable : 4996)

using namespace std;

int main(int argc, char* argv[]) {

	//string fname = argv[1];
	string fname = "input.txt";
	ifstream fin(fname);

	int n, h;
	fin >> n >> h;
	BarrierGraph g(n, h);
	g.InputGraph(fin);

	double startTime = clock();
	g.BuildAuxGraph();

	int s = 0, t = g.auxN - 1;

	CapacityDistributionAlgo capacityDistributionAlgo(g);

	auto [flowValue, flow] = capacityDistributionAlgo.getMaxFlow(s, t, 400, 50);

	if (!checkCorrectness(g, flow)) {
		cout << "The flow is incorrect";
		exit(1);
	}

	cout << flowValue << endl;

	/*BreakThroughAlgo breakThroughAlgo(g);

	auto [flowValue, flow] = breakThroughAlgo.getMaxFlow(0, g.auxN - 1);

	if (!checkCorrectness(g, flow)) {
		cout << "The flow is incorrect";
		exit(1);
	}

	double endTime = clock();
	cout << flowValue << ' ' << fixed << setprecision(4) << (endTime - startTime) / 1000;*/



}