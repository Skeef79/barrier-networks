#include "Graph.h"
#include<iomanip>
#include<iostream>
#include<queue>
#include<algorithm>
#include<map>
#include<utility>
#include<fstream>
#include<random>

string BarrierGraph::StrType(EdgeType edgeType) {
	if (edgeType == EdgeType::neutral)
		return "neutral";
	if (edgeType == EdgeType::increase)
		return "increasing";
	if (edgeType == EdgeType::barrier)
		return "barrier";
}

void BarrierGraph::AddEdge(int from, int to, flowT cap, EdgeType type) {
	g[from].emplace_back(from, to, type);
	capacity[from][to] = cap;
	edges.emplace_back(Edge(from, to, type));
}


BarrierGraph::BarrierGraph(int n, int h) : n(n), h(h) {
	g.resize(n);
	auxN = n * (h + 1) + 1;
	auxG.resize(auxN);
	capacity = vector<vector<flowT>>(n + 1, vector<flowT>(n + 1));
	capacity[n - 1][n] = INF;
	edgeTypes = vector<vector<EdgeType>>(n + 1, vector<EdgeType>(n + 1, EdgeType::neutral));
	initialToAux = vector<vector<vector<pair<int, int>>>>(n + 1, vector<vector<pair<int, int>>>(n + 1));
}

void BarrierGraph::InputGraph(ifstream& fin) {
	int m;
	fin >> m;
	for (int i = 0; i < m; i++) {
		int from, to;
		flowT capacity;
		int edgeType;
		fin >> from >> to >> capacity >> edgeType;
		if (capacity != 0)
			BarrierGraph::AddEdge(from, to, capacity, EdgeType(edgeType));
		edgeTypes[from][to] = edgeTypes[to][from] = EdgeType(edgeType);
	}
}


void BarrierGraph::AddAuxEdge(int from, int to, int fromInitial, int toInitial, int edgeIndex) {
	auxEdges.push_back(auxEdge(from, to, fromInitial, toInitial, edgeIndex));
	auxEdges.back().capacity = 0;
	auxG[from].push_back((int)auxEdges.size() - 1);
	auxEdges.push_back(auxEdge(to, from, toInitial, fromInitial, edgeIndex));
	auxEdges.back().capacity = 0;
	auxG[to].push_back((int)auxEdges.size() - 1);
	initialToAux[fromInitial][toInitial].push_back({ from,to });
	initialToAux[toInitial][fromInitial].push_back({ to,from });
}

void BarrierGraph::BuildAuxGraph() {
	for (int from = 0; from < n; from++) {
		for (int to = 0; to < n; to++) {
			if (!capacity[from][to])
				continue;
			if (edgeTypes[from][to] == EdgeType::barrier) {
				int fromAux = from * (h + 1) + h;
				int toAux = to * (h + 1);
				AddAuxEdge(fromAux, toAux, from, to, 0);
			}
			else {

				for (int j = 0; j <= h; j++) {
					int fromAux, toAux;
					if (edgeTypes[from][to] == EdgeType::neutral) {
						fromAux = from * (h + 1) + j;
						toAux = to * (h + 1) + j;
					}
					else {
						fromAux = from * (h + 1) + j;
						toAux = to * (h + 1) + j + 1;
						if (j == h)
							toAux--;
					}
					AddAuxEdge(fromAux, toAux, from, to, j);
				}
			}
		}
	}

	//add fictive sink (n-1 is initial sink)
	int t = auxN - 1;

	for (int j = 0; j <= h; j++) {
		int fromAux = (n - 1) * (h + 1) + j;
		int toAux = t;
		AddAuxEdge(fromAux, toAux, n - 1, n, j);
	}

}

/*
void  BarrierGraph::SortEdges() {
	//sort edges
	for (int i = 0; i < auxG.size(); i++) {

		sort(auxG[i].begin(), auxG[i].end(), [&](int id1, int id2) {
			int from1 = auxEdges[id1].fromInitial;
			int to1 = auxEdges[id1].toInitial;

			int from2 = auxEdges[id2].fromInitial;
			int to2 = auxEdges[id2].toInitial;

			EdgeType edgeType1 = edgeTypes[from1][to1];
			EdgeType edgeType2 = edgeTypes[from2][to2];

			if (edgeType1 == edgeType2) {
				return capacity[from1][to1] > capacity[from2][to2];
			}
			else {
				if (edgeType1 == EdgeType::increase)
					return true;
				else
					if (edgeType2 == EdgeType::increase)
						return false;
					else
						return capacity[from1][to1] > capacity[from2][to2];
			}
			});
	}

}
*/

