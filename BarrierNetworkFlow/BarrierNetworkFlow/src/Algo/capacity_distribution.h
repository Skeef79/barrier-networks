#pragma once

#include<vector>
#include "../Graph/graph.h"
#include<utility>
#include "../Utils/math.h"

using namespace std;


class Individual {
public:
	int n;
	vector<vector<vector<int>>> chromosome;
	Individual(int n, const BarrierGraph& graph);
	Individual(int n, vector<vector<vector<int>>> pChromosome);
	Individual();

private:
};

void logIteration(int iteration, vector<flowT>fitness);
vector<int> mutateGene(vector<int> gene, int cap);
Individual mutation(const Individual& individual, const BarrierGraph& graph, int changesCnt = 3);
Individual crossover(const Individual& parent1, const Individual& parent2, const BarrierGraph& graph, double prob = 0.5);
flowT getFitness(const Individual& individual, const BarrierGraph& graph, int s, int t);

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
