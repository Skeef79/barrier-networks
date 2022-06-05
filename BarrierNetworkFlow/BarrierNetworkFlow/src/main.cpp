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
	//bool breakThroughInit = argv[2];

	bool breakThroughInit = true;

	string fname = "input.txt";
	ifstream fin(fname);

	int n, h;
	fin >> n >> h;
	BarrierGraph g(n, h);
	g.InputGraph(fin);

	double startTime = clock();
	g.BuildAuxGraph();

	int s = 0, t = g.auxN - 1;
	BreakThroughAlgo breakThroughAlgo(g);
	auto [flowValue1, flow1] = breakThroughAlgo.getMaxFlow(0, g.auxN - 1);

	if (!checkCorrectness(g, flow1)) {
		cout << "The flow is incorrect";
		exit(1);
	}

	double endTime = clock();
	cout << flowValue1 << ' ' << fixed << setprecision(4) << (endTime - startTime) / 1000;

	cout << endl << endl;


	startTime = clock();
	CapacityDistributionAlgo capacityDistributionAlgo(g);

	flowT flowValue;
	vector<vector<flowT>> flow;
	if (breakThroughInit)
		tie(flowValue, flow) = capacityDistributionAlgo.getMaxFlow(s, t, 10, 2, true, flow1);
	else
		tie(flowValue, flow) = capacityDistributionAlgo.getMaxFlow(s, t, 10, 2);

	if (!checkCorrectness(g, flow)) {
		cout << "The flow is incorrect";
		exit(1);
	}

	endTime = clock();
	cout << flowValue << ' ' << fixed << setprecision(4) << (endTime - startTime) / 1000;




}