#include "dinic.h"
#include<queue>

DinicAlgo::DinicAlgo(BarrierGraph& g, Individual& individual, int s, int t) :
	g(g), s(s), t(t) {
	d.resize(g.auxN);
	pt.resize(g.auxN);
	capacity = individual.chromosome;
	auxFlow = vector<vector<flowT>>(g.auxN, vector<flowT>(g.auxN));
}

bool DinicAlgo::bfs() {
	queue<int>q;
	fill(d.begin(), d.end(), -1);
	q.push(s);
	d[s] = 0;
	while (!q.empty()) {
		int v = q.front();
		q.pop();
		for (auto id : g.auxG[v]) {
			int to = g.auxEdges[id].to;

			int fromInitial = g.auxEdges[to].fromInitial;
			int toInitial = g.auxEdges[to].toInitial;
			int edgeIndex = g.auxEdges[to].edgeIndex;

			if (d[to] == -1 && auxFlow[v][to] < capacity[fromInitial][toInitial][edgeIndex]) {
				d[to] = d[v] + 1;
				q.push(to);
			}
		}
	}

	return d[t] != -1;
}

flowT DinicAlgo::dfs(int v, flowT currentFlow) {
	if (currentFlow == 0)
		return 0;
	if (v == t)
		return currentFlow;
	for (; pt[v] < g.auxG[v].size(); pt[v]++) {
		int id = g.auxG[v][pt[v]];
		int to = g.auxEdges[id].to;

		int fromInitial = g.auxEdges[to].fromInitial;
		int toInitial = g.auxEdges[to].toInitial;
		int edgeIndex = g.auxEdges[to].edgeIndex;

		if (d[to] == d[v] + 1 && auxFlow[v][to] < capacity[fromInitial][toInitial][edgeIndex]) {
			flowT pushed = dfs(to, min(currentFlow, capacity[fromInitial][toInitial][edgeIndex] - auxFlow[v][to]));
			if (pushed) {
				auxFlow[v][to] += pushed;
				auxFlow[to][v] -= pushed;
				return pushed;
			}
		}
	}
	return 0;
}

flowT DinicAlgo::getMaxFlow() {
	flowT resultFlow = 0;
	flowT currentFlow;
	while (bfs()) {
		fill(pt.begin(), pt.end(), 0);
		while (currentFlow = dfs(s, INF))
			resultFlow += currentFlow;
	}
	return resultFlow;
	//maybe there I shoult return also flows for each edge 
}