#pragma once
#include<vector>
#include<iostream>
#include "utils.h"

using namespace std;

struct Edge {
	int from, to;
	EdgeType edgeType;
	Edge(int from, int to, EdgeType edgeType) :
		from(from), to(to), edgeType(edgeType) {
	}

};

struct auxEdge {
	int from, to;
	int fromInitial, toInitial;
	int capacity, flow;

	auxEdge(int from, int to, int fromInitial, int toInitial) :
		from(from), to(to), fromInitial(fromInitial), toInitial(toInitial) {
		flow = 0;
		capacity = 0;
	}
};


class BarrierGraph {
public:
	vector<vector<Edge>>g;
	vector<vector<int>> auxG;
	vector<auxEdge> auxEdges; 
	vector<vector<int>> flow;
	vector<vector<int>>capacity;
	vector<vector<EdgeType>> edgeTypes;
	vector<bool>used;

	int n, h;
	BarrierGraph(int n, int h) : n(n), h(h) {
		g.resize(n);
		auxG.resize(n * (h + 1) + 1);
		flow = vector<vector<int>>(n + 1, vector<int>(n + 1));
		capacity = vector<vector<int>>(n + 1, vector<int>(n + 1));
		capacity[n - 1][n] = INF;
		edgeTypes = vector<vector<EdgeType>>(n + 1, vector<EdgeType>(n + 1));
		used.resize(n * (h + 1) + 1);
	}

	string StrType(EdgeType edgeType);

	void AddEdge(int from, int to, int cap, EdgeType type);

	void InputGraph(ifstream& fin);

	void OutputGraph();

	void AddAuxEdge(int from, int to, int fromInitial, int toInitial);

	void BuildAuxGraph();

	void OutputAuxGraph();

	int GetMaxFlow(int s, int t);

	void SortEdges();

	int dfsBreakthrough(int v, int currentFlow, int t);

	int GetPath(int v, int currentFlow, int t, vector<int>& path, int barrierLevel);

	//decompose initial graph into paths
	vector<pair<int, vector<int>>> DecomposeIntoPaths(int s, int t);

	bool CheckAfterDecompose();
};

