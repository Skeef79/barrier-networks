#pragma once
#include "../Graph/graph.h"
#include<vector>
#include<utility>

bool checkCorrectness(const BarrierGraph& graph, const vector<vector<flowT>>& flow);

vector<pair<int, vector<int>>> decompose(const BarrierGraph& graph, const vector<vector<flowT>>& flow);
