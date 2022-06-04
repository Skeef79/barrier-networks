#include "capacity_distribution.h"
#include "dinic.h"
#include<set>

using namespace std;

mt19937 shuffleGen(2022);

Individual::Individual(int n, const BarrierGraph& graph, bool randomInit) :n(n) {
	chromosome = vector<vector<vector<flowT>>>(n, vector<vector<flowT>>(n));
	if (!randomInit)
		return;

	for (int from = 0; from < n; from++) {
		for (int to = 0; to < n; to++) {
			if (!graph.capacity[from][to] && !graph.capacity[to][from])
				continue;

			if (graph.edgeTypes[from][to] == EdgeType::barrier)
				chromosome[from][to] = vector<flowT>(1, graph.capacity[from][to]);
			else
				chromosome[from][to] = Math::genCapacityDistribution(graph.h + 1, graph.capacity[from][to]);
		}
	}
}

Individual::Individual(int n, vector<vector<vector<flowT>>> pChromosome) :n(n) {
	chromosome = pChromosome;
}

Individual::Individual() {
	chromosome = {};
}


CapacityDistributionAlgo::CapacityDistributionAlgo(BarrierGraph& graph)
	:graph(graph) {}



vector<flowT> mutateGeneRandom(vector<flowT> gene, flowT cap) {
	return Math::genCapacityDistribution(gene.size(), cap);
}

vector<flowT> mutateGeneOptimize(vector<flowT> caps, vector<flowT> flows, flowT cap) {

	flowT freeCap = 0;
	vector<double>distr(caps.size());
	for (int i = 0; i < caps.size(); i++) {
		freeCap += caps[i] - flows[i];
		distr[i] = caps[i] - flows[i];
	}

	vector<flowT>result(flows.begin(), flows.end());
	for (int i = 0; i < caps.size(); i++) {
		distr[i] = (freeCap - distr[i]);
		distr[i] += Math::genDouble(0, 1) * distr[i] / 2;
	}

	//maybe EPS is not the best)
	double total = accumulate(distr.begin(), distr.end(), double(0)) + EPS;

	flowT capAdded = 0;
	for (int i = 0; i < result.size(); i++) {
		flowT capAdd = (distr[i] / total) * freeCap;
		result[i] += capAdd;
		capAdded += capAdd;
	}

	for (int i = 0; i < freeCap - capAdded; i++) {
		int idx = Math::genInt((int)result.size());
		result[idx]++;
	}

	/*for (int i = 0; i < freeCap; i++) {
		int idx = Math::getRandomIndex(distr);
		result[idx]++;
	}*/

	assert(accumulate(result.begin(), result.end(), flowT(0)) == cap);
	return result;
}

Individual mutation(const Individual& individual, const vector<vector<flowT>>& flows, const BarrierGraph& graph, int changesCnt) {
	Individual newIndividual = Individual(individual.n, individual.chromosome);
	vector<int>edges_indices(graph.edges.size());
	iota(edges_indices.begin(), edges_indices.end(), 0);
	shuffle(edges_indices.begin(), edges_indices.end(), shuffleGen);

	for (int i = 0; i < min(changesCnt, (int)graph.edges.size()); i++) {
		int  id = edges_indices[i];

		int from = graph.edges[id].from;
		int to = graph.edges[id].to;
		if (newIndividual.chromosome[from][to].empty())
			continue;

		if (Math::isProb(0.1))
			newIndividual.chromosome[from][to] = mutateGeneRandom(newIndividual.chromosome[from][to], graph.capacity[from][to]);
		else {
			vector<flowT>setFlows;
			for (auto [fromAux, toAux] : graph.initialToAux[from][to]) {
				setFlows.push_back(flows[fromAux][toAux]);
			}

			//adaptation
			newIndividual.chromosome[from][to] = mutateGeneOptimize(newIndividual.chromosome[from][to], setFlows, graph.capacity[from][to]);
		}
	}

	return newIndividual;
}

Individual crossover(const Individual& parent1, const Individual& parent2, const BarrierGraph& graph) {
	Individual newIndividual = Individual(parent1);
	int crossPoint = Math::genInt(newIndividual.n * newIndividual.n);
	for (int from = 0; from < newIndividual.n; from++)
		for (int to = 0; to < newIndividual.n; to++) {
			if (from * newIndividual.n + to >= crossPoint)
				newIndividual.chromosome[from][to] = parent2.chromosome[from][to];
		}

	return newIndividual;
}

//get maxflow with dinic
pair<flowT, vector<vector<flowT>>> getFitness(Individual& individual, BarrierGraph& graph, int s, int t) {
	DinicAlgo dinic(graph, individual, s, t);
	return dinic.getMaxFlow();

}

void logIteration(int iteration, vector<flowT>fitness) {
	sort(fitness.rbegin(), fitness.rend());
	cout << "Iteration " << iteration << ": \n";
	cout << "Fitness: ";
	for (int i = 0; i < min((int)fitness.size(), 10); i++)
		cout << fitness[i] << ' ';
	cout << "\n";
}


//getMaxFlow using genetic algorithm
pair<flowT, vector<vector<flowT>>> CapacityDistributionAlgo::getMaxFlow(int s, int t,
	int ITERATIONS, int POPULATION_SIZE, bool breakThroughInit, vector<vector<flowT>> initFlow) {

	int oldPercent = 20;
	int mutationPercent = 30;
	int mutationChangesPercent = 20;

	int oldCnt = max(1, int(double(oldPercent) / 100 * POPULATION_SIZE));
	int mutationCnt = max(1, int(double(mutationPercent) / 100 * POPULATION_SIZE));
	int mutationChangesCnt = max(1, int(double(mutationChangesPercent) / 100 * graph.edges.size()));

	double randomIndividualProb = 0.3;

	//init population
	population.clear();
	population.resize(POPULATION_SIZE);

	if (breakThroughInit) {
		for (int i = 0; i < POPULATION_SIZE; i++) {
			if (Math::isProb(randomIndividualProb)) {
				population[i] = Individual(graph.n + 1, graph, true);
				continue;
			}

			population[i] = Individual(graph.n + 1, graph);
			for (int from = 0; from < population[i].n; from++) {
				for (int to = 0; to < population[i].n; to++) {
					vector<flowT>setFlows;
					for (auto [fromAux, toAux] : graph.initialToAux[from][to]) {
						setFlows.push_back(initFlow[fromAux][toAux]);
					}
					flowT remCap = graph.capacity[from][to] - accumulate(setFlows.begin(), setFlows.end(), flowT(0));
					auto added = mutateGeneRandom(setFlows, remCap);
					vector<flowT>result(setFlows.begin(), setFlows.end());
					for (int i = 0; i < result.size(); i++)
						result[i] += added[i];
					population[i].chromosome[from][to] = result;
				}
			}
		}
	}
	else {
		for (int i = 0; i < POPULATION_SIZE; i++)
			population[i] = Individual(graph.n + 1, graph, true);
	}

	flowT bestFitness = 0;
	vector<vector<flowT>>bestFlows = {};

	for (int iteration = 1; iteration <= ITERATIONS; iteration++) {
		vector<flowT> populationFitness(POPULATION_SIZE);
		vector<vector<vector<flowT>>>populationFlows(POPULATION_SIZE);

		for (int i = 0; i < POPULATION_SIZE; i++) {
			tie(populationFitness[i], populationFlows[i]) = getFitness(population[i], graph, s, t);
			if (populationFitness[i] > bestFitness) {
				bestFitness = populationFitness[i];
				bestFlows = populationFlows[i];
			}
		}

		//log current iteration
		logIteration(iteration, populationFitness);

		double avgFitness = accumulate(populationFitness.begin(), populationFitness.end(), double(0)) / POPULATION_SIZE;
		vector<double>crossoverDistribution(POPULATION_SIZE);
		for (int i = 0; i < POPULATION_SIZE; i++) {
			crossoverDistribution[i] = populationFitness[i] / (avgFitness + 1) + EPS;
		}

		//add old individual without mutation
		vector<int> indices(POPULATION_SIZE);
		iota(indices.begin(), indices.end(), 0);
		shuffle(indices.begin(), indices.end(), shuffleGen);

		/*for (int i = 0; i < mutationCnt; i++) {
			int idx = indices[i];
			population[idx] = mutation(population[idx], populationFlows[idx], graph, mutationChangesCnt);
		}*/

		vector<Individual>newPopulation;
		for (int i = 0; i < POPULATION_SIZE - oldCnt; i++) {
			int idx1 = -1, idx2 = -1;
			while (idx1 == idx2) {
				idx1 = Math::getRandomIndex(crossoverDistribution);
				idx2 = Math::getRandomIndex(crossoverDistribution);
			}
			newPopulation.emplace_back(crossover(population[idx1], population[idx2], graph));
		}

		vector<int>populationIndices(POPULATION_SIZE);
		iota(populationIndices.begin(), populationIndices.end(), 0);
		sort(populationIndices.begin(), populationIndices.end(), [&](int i, int j) {
			return populationFitness[i] > populationFitness[j];
			});


		for (int i = 0; i < oldCnt; i++)
			newPopulation.emplace_back(population[populationIndices[i]]);


		// now mutate, and also recalc fitness
		for (int i = 0; i < mutationCnt; i++) {
			int idx = indices[i];
			tie(populationFitness[idx], populationFlows[idx]) = getFitness(newPopulation[idx], graph, s, t);
			newPopulation[idx] = mutation(newPopulation[idx], populationFlows[idx], graph, mutationChangesCnt);
		}

		population = newPopulation;

	}

	return { bestFitness, bestFlows };
}
