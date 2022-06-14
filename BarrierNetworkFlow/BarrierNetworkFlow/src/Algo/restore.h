#pragma once

#include "../Graph/graph.h"
#include<vector>
#include<utility>

class PathRestore {
public:
	PathRestore(BarrierGraph& graph);
	vector<pair<vector<int>, flowT>> restore(int s, int t, vector<vector<flowT>>auxFlow);

private:
	BarrierGraph& graph;
	vector<vector<flowT>>flow;
	flowT dfs(int v, flowT currentFlow, int t, int vInital);
	vector<bool>used;
	vector<int>path;
};