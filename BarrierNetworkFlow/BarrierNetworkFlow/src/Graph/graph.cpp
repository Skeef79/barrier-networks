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
}


BarrierGraph::BarrierGraph(int n, int h) : n(n), h(h) {
	g.resize(n);
	auxN = n * (h + 1) + 1;
	auxG.resize(auxN);
	capacity = vector<vector<flowT>>(n + 1, vector<flowT>(n + 1));
	capacity[n - 1][n] = INF;
	edgeTypes = vector<vector<EdgeType>>(n + 1, vector<EdgeType>(n + 1, EdgeType::neutral));

}

void BarrierGraph::InputGraph(ifstream& fin) {
	int m;
	fin >> m;
	for (int i = 0; i < m; i++) {
		int from, to;
		flowT capacity;
		int edgeType;
		fin >> from >> to >> capacity >> edgeType;
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


//void  BarrierGraph::SortEdges() {
//	//sort edges
//	for (int i = 0; i < auxG.size(); i++) {
//
//		sort(auxG[i].begin(), auxG[i].end(), [&](int id1, int id2) {
//			int from1 = auxEdges[id1].fromInitial;
//			int to1 = auxEdges[id1].toInitial;
//
//			int from2 = auxEdges[id2].fromInitial;
//			int to2 = auxEdges[id2].toInitial;
//
//			EdgeType edgeType1 = edgeTypes[from1][to1];
//			EdgeType edgeType2 = edgeTypes[from2][to2];
//
//			if (edgeType1 == edgeType2) {
//				return capacity[from1][to1] > capacity[from2][to2];
//			}
//			else {
//				if (edgeType1 == EdgeType::increase)
//					return true;
//				else
//					if (edgeType2 == EdgeType::increase)
//						return false;
//					else
//						return capacity[from1][to1] > capacity[from2][to2];
//			}
//			});
//	}
//
//}



//int BarrierGraph::GetPath(int v, int currentFlow, int t, vector<int>& path, int barrierLevel = 0) {
//	if (!currentFlow)
//		return 0;
//
//	if (v == t)
//		return currentFlow;
//
//	for (auto id : auxG[v]) {
//		int to = auxEdges[id].to;
//		int fromInitial = auxEdges[id].fromInitial;
//		int toInitial = auxEdges[id].toInitial;
//		if (id & 1)
//			continue;
//
//		if (!used[to] && flow[fromInitial][toInitial] > 0) {
//			if (edgeTypes[fromInitial][toInitial] == EdgeType::barrier) {
//				if (barrierLevel >= h) {
//					int pushed = GetPath(to, min(currentFlow, flow[fromInitial][toInitial]), t, path, 0);
//					if (pushed) {
//						path.push_back(toInitial);
//						flow[fromInitial][toInitial] -= pushed;
//						return pushed;
//					}
//				}
//			}
//			else
//				if (edgeTypes[fromInitial][toInitial] == EdgeType::increase) {
//					int pushed = GetPath(to, min(currentFlow, flow[fromInitial][toInitial]), t, path, barrierLevel + 1);
//					if (pushed) {
//						path.push_back(toInitial);
//						flow[fromInitial][toInitial] -= pushed;
//						return pushed;
//					}
//				}
//				else {
//					int pushed = GetPath(to, min(currentFlow, flow[fromInitial][toInitial]), t, path, barrierLevel);
//					if (pushed) {
//						path.push_back(toInitial);
//						flow[fromInitial][toInitial] -= pushed;
//						return pushed;
//					}
//				}
//		}
//	}
//
//	return 0;
//}
//
//vector<pair<int, vector<int>>> BarrierGraph::DecomposeIntoPaths(int s, int t) {
//	vector<pair<int, vector<int>>>paths;
//
//	int currentFlow = 0;
//	vector<int>path;
//
//	fill(used.begin(), used.end(), false);
//	while (currentFlow = GetPath(s, INF, t, path)) {
//		reverse(path.begin(), path.end());
//		paths.push_back({ currentFlow, path });
//		path.clear();
//
//		fill(used.begin(), used.end(), false);
//	}
//
//	return paths;
//}
//
//bool BarrierGraph::CheckAfterDecompose() {
//	for (int i = 0; i < n; i++)
//		for (int j = 0; j < n; j++)
//			if (flow[i][j] != 0) {
//				return false;
//			}
//	return true;
//}
