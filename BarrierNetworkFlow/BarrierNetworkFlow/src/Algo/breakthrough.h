#pragma once

#include<vector>
#include "../Graph/graph.h"
#include<utility>

using namespace std;

class BreakThroughAlgo {
public:
	BreakThroughAlgo(BarrierGraph& graph);
	pair<flowT, vector<vector<flowT>>> getMaxFlow(int s, int t);

private:
	BarrierGraph& graph;
	flowT dfsBreakthrough(int v, flowT currentFlow, int t);
	vector<bool>used;
	vector<vector<flowT>> flow;
	vector<vector<flowT>>auxFlow;
};
