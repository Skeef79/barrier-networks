#include "breakthrough.h"

BreakThroughAlgo::BreakThroughAlgo(BarrierGraph& graph) :graph(graph) {}


flowT BreakThroughAlgo::dfsBreakthrough(int v, flowT currentFlow, int t) {
	if (!currentFlow)
		return 0;

	if (v == t)
		return currentFlow;

	used[v] = true;

	for (auto id : graph.auxG[v]) {
		int to = graph.auxEdges[id].to;
		int fromInitial = graph.auxEdges[id].fromInitial;
		int toInitial = graph.auxEdges[id].toInitial;
		if (id & 1)
			continue;

		if (!used[to] && flow[fromInitial][toInitial] < graph.capacity[fromInitial][toInitial]) {
			int pushed = dfsBreakthrough(to, min(currentFlow, graph.capacity[fromInitial][toInitial] - flow[fromInitial][toInitial]), t);
			if (pushed) {
				flow[fromInitial][toInitial] += pushed;
				auxFlow[v][to] += pushed;
				return pushed;
			}
		}
	}

	return 0;

}

pair<flowT, vector<vector<flowT>>> BreakThroughAlgo::getMaxFlow(int s, int t) {
	used.clear();
	flow.clear();
	auxFlow.clear();
	used.resize(graph.n * (graph.h + 1) + 1);
	flow = vector<vector<flowT>>(graph.n + 1, vector<flowT>(graph.n + 1));
	auxFlow = vector<vector<flowT>>(graph.auxN, vector<flowT>(graph.auxN));

	flowT resultFlow = 0;
	flowT currentFlow = 0;

	fill(used.begin(), used.end(), false);
	while (currentFlow = dfsBreakthrough(s, INF, t)) {
		resultFlow += currentFlow;
		fill(used.begin(), used.end(), false);
	}

	return { resultFlow, auxFlow };

}


