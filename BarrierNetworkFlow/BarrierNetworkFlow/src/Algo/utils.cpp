#include "utils.h"

bool checkCorrectness(const BarrierGraph& graph, const vector<vector<flowT>>& flow) {
	vector<flowT> balance(graph.auxN);
	for (int i = 0; i < graph.auxN; i++)
		for (int j = 0; j < graph.auxN; j++) {
			balance[i] += flow[i][j];
			balance[j] -= flow[i][j];
		}

	for (int i = 1; i < graph.auxN - 1; i++) {
		if (balance[i] != 0)
			return false;
	}

	if (balance[0] != -balance.back())
		return false;

	vector<vector<flowT>>totalFlow(graph.n + 1, vector<flowT>(graph.n + 1));
	for (auto e : graph.auxEdges) {
		totalFlow[e.fromInitial][e.toInitial] += flow[e.from][e.to];
	}

	for (int i = 0; i < totalFlow.size(); i++)
		for (int j = 0; j < totalFlow.size(); j++) {
			if (totalFlow[i][j] > graph.capacity[i][j])
				return false;
		}

	return true;
}

vector<pair<int, vector<int>>> decompose(const BarrierGraph& graph, const vector<vector<flowT>>& flow) {
	return {};
}