#pragma once

#include<vector>
#include "../Graph/graph.h"
#include<utility>
#include "../Utils/math.h"
#include "../Utils/utils.h"

using namespace std;


class Individual {
public:
	int n;
	vector<vector<vector<flowT>>> chromosome;
	Individual(int n, const BarrierGraph& graph);
	Individual(int n, vector<vector<vector<flowT>>> pChromosome);
	Individual();

private:
};

void logIteration(int iteration, vector<flowT>fitness);
vector<flowT> mutateGeneRandom(vector<flowT> gene, flowT cap);
vector<flowT> mutateGeneOptimize(vector<flowT> caps, vector<flowT> flows, flowT cap);
Individual mutation(const Individual& individual, const vector<vector<flowT>>& flows, const BarrierGraph& graph, int changesCnt = 3);
Individual crossover(const Individual& parent1, const Individual& parent2, const BarrierGraph& graph, double prob = 0.5);
pair<flowT, vector<vector<flowT>>> getFitness(const Individual& individual, const BarrierGraph& graph, int s, int t);

class CapacityDistributionAlgo {
public:
	CapacityDistributionAlgo(BarrierGraph& graph);
	pair<flowT, vector<vector<flowT>>> getMaxFlow(int s, int t,
		int ITERATIONS = 100, int POPULATION_SIZE = 50,
		double CROSSOVER_PROBABILITY = 0.5
	);

private:
	BarrierGraph& graph;
	vector<Individual> population;
};
