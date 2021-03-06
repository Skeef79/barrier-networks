#pragma once
#include "capacity_distribution.h"

using namespace std;

class DinicAlgo {
public:

	DinicAlgo(BarrierGraph& g, Individual& individual, int s, int t);
	pair<flowT, vector<vector<flowT>>> getMaxFlow();

private:
	BarrierGraph& g;
	vector<vector<vector<flowT>>>capacity;

	vector<int>d;
	vector<int>pt;
	int s, t;
	bool bfs();
	flowT dfs(int v, flowT currentFlow);
	vector<vector<flowT>>auxFlow;
};
