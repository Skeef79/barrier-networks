#include "Graph.h"
#include<stdio.h>
#include<iomanip>
#include<iostream>
#include<vector>
#include<string>
#include<fstream>

#pragma warning(disable : 4996)

using namespace std;

int main() {

	ofstream fout("out.txt");

	int nTests = 15;
	for (int i = 0; i < nTests; i++) {
		string fileName = "tests\\" + to_string(i) + ".txt";
		ifstream fin(fileName);

		int n, h;
		fin >> n >> h;
		BarrierGraph g(n, h);
		g.InputGraph(fin);

		double start_time = clock();
		g.BuildAuxGraph();
		g.SortEdges();

		cout << "Test: " + to_string(i) << endl;

		int flow = g.GetMaxFlow(0, n * (h + 1));
		cout << "Maximum flow by is: " << flow << endl;

		double end_time = clock();
		cout << "Algo took: " << fixed << setprecision(6) << (end_time - start_time) / CLOCKS_PER_SEC << endl;

		fout << flow << ' ' << fixed << setprecision(6) << (end_time - start_time) / CLOCKS_PER_SEC << endl;

		auto paths = g.DecomposeIntoPaths(0, n * (h + 1));
		if (!g.CheckAfterDecompose()) {
			cout << "Incorrect flow" << endl;
			return 0;
		}
		cout << endl << endl;

	}

}