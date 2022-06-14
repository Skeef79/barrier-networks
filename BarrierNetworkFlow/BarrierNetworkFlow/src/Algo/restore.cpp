#include "restore.h"

PathRestore::PathRestore(BarrierGraph& graph) :graph(graph) {}


flowT PathRestore::dfs(int v, flowT currentFlow, int t, int vInital) {
	if (!currentFlow)
		return 0;

	path.push_back(vInital);
	used[v] = true;
	if (v == t)
		return currentFlow;

	for (auto id : graph.auxG[v]) {
		int to = graph.auxEdges[id].to;
		int fromInitial = graph.auxEdges[id].fromInitial;
		int toInitial = graph.auxEdges[id].toInitial;
		if (id & 1)
			continue;
		if (!used[to] && flow[v][to] > 0) {
			flowT pushed = dfs(to, min(currentFlow, flow[v][to]), t, toInitial);
			if (pushed) {
				flow[v][to] -= pushed;
				return pushed;
			}
		}
	}

	used[v] = false;
	path.pop_back();
	return 0;
}

vector<pair<vector<int>, flowT>> PathRestore::restore(int s, int t, vector<vector<flowT>>auxFlow) {
	flow = auxFlow;
	used.resize(graph.auxN);

	vector<pair<vector<int>, flowT>>res;
	while (true) {
		fill(used.begin(), used.end(), false);
		path.clear();
		flowT currentFlow = dfs(s, INF, t, s);
		if (currentFlow) {
			path.pop_back();
			res.push_back({ path,currentFlow });
		}
		else
			break;
	}

	for (int i = 0; i < flow.size(); i++)
		for (int j = 0; j < flow.size(); j++)
			if (flow[i][j]) {
				throw exception("incorrect flow");
				return {};
			}

	return res;
}

