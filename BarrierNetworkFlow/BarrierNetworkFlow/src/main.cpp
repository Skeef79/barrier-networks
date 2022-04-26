#include "Graph.h"
#include<stdio.h>
#include<iomanip>
#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<random>

#pragma warning(disable : 4996)

using namespace std;

int main(int argc, char* argv[]) {

	string fname = argv[1];
	ifstream fin(fname);

	int n, h;
	fin >> n >> h;
	BarrierGraph g(n, h);
	g.InputGraph(fin);

	double startTime = clock();
	g.BuildAuxGraph();
	//g.SortEdges();

	int flow = g.GetMaxFlow(0, n * (h + 1));
	double endTime = clock();
	cout << flow << ' ' << fixed << setprecision(4) << (endTime - startTime) / 1000;

}