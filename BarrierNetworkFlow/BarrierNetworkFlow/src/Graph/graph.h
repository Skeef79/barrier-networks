#pragma once
#include<vector>
#include<iostream>
#include "..\Utils\utils.h"

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
	flowT capacity;
	int edgeIndex;

	auxEdge(int from, int to, int fromInitial, int toInitial, int edgeIndex) :
		from(from), to(to), fromInitial(fromInitial), toInitial(toInitial),
		edgeIndex(edgeIndex) {
		capacity = 0;
	}
};


class BarrierGraph {
public:
	//initialGraph
	vector<vector<Edge>>g;
	vector<vector<int>> auxG;
	vector<Edge>edges;
	vector<auxEdge> auxEdges;
	vector<vector<EdgeType>> edgeTypes;
	vector<vector<flowT>>capacity;
	vector<vector<vector<pair<int, int>>>> initialToAux;

	int n, h, auxN;
	BarrierGraph(int n, int h);

	string StrType(EdgeType edgeType);

	void AddEdge(int from, int to, flowT cap, EdgeType type);

	void InputGraph(ifstream& fin);

	void AddAuxEdge(int from, int to, int fromInitial, int toInitial, int edgeIndex);

	void BuildAuxGraph();


	//int GetMaxFlow(int s, int t);

	//int dfsBreakthrough(int v, int currentFlow, int t);

	//int GetPath(int v, int currentFlow, int t, vector<int>& path, int barrierLevel);

	//decompose initial graph into paths
	//vector<pair<int, vector<int>>> DecomposeIntoPaths(int s, int t);

	//bool CheckAfterDecompose();
};

