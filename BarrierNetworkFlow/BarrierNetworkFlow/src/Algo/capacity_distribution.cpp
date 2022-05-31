#include "capacity_distribution.h"
#include "dinic.h"
using namespace std;


Individual::Individual(int n, const BarrierGraph& graph) :n(n) {
	chromosome = vector<vector<vector<int>>>(n, vector<vector<int>>(n));
	for (int from = 0; from < n; from++) {
		for (int to = 0; to < n; to++) {
			if (graph.edgeTypes[from][to] == EdgeType::barrier)
				chromosome[from][to] = vector<int>(1, graph.capacity[from][to]);
			else
				chromosome[from][to] = Math::genCapacityDistribution(graph.h + 1, graph.capacity[from][to]);
		}
	}
}

Individual::Individual(int n, vector<vector<vector<int>>> pChromosome) :n(n) {
	chromosome = pChromosome;
}

Individual::Individual() {
	chromosome = {};
}


CapacityDistributionAlgo::CapacityDistributionAlgo(BarrierGraph& graph)
	:graph(graph) {}



vector<int> mutateGene(vector<int> gene, int cap) {
	return Math::genCapacityDistribution(gene.size(), cap);
}

Individual mutation(const Individual& individual, const BarrierGraph& graph, int changesCnt) {
	Individual newIndividual = Individual(individual.n, individual.chromosome);
	for (int i = 0; i < changesCnt; i++) {
		int from = Math::genInt(individual.n - 1);
		int to = Math::genInt(from + 1, individual.n - 1);
		newIndividual.chromosome[from][to] = mutateGene(newIndividual.chromosome[from][to], graph.capacity[from][to]);
	}

	return newIndividual;
}

Individual crossover(const Individual& parent1, const Individual& parent2, const BarrierGraph& graph, double prob) {
	Individual newIndividual = Individual(parent1);
	for (int from = 0; from < newIndividual.n; from++)
		for (int to = 0; to < newIndividual.n; to++) {
			if (Math::isProb(prob))
				newIndividual.chromosome[from][to] = parent2.chromosome[from][to];
		}

	return newIndividual;
}

//get maxflow with dinic
flowT getFitness(Individual& individual, BarrierGraph& graph, int s, int t) {
	DinicAlgo dinic(graph, individual, s, t);
	flowT maxFlow = dinic.getMaxFlow();
	return maxFlow;
}

void logIteration(int iteration, vector<flowT>fitness) {
	sort(fitness.rbegin(), fitness.rend());
	cout << "Iteration " << iteration << ": \n";
	cout << "Fitness: ";
	for (int i = 0; i < fitness.size(); i++)
		cout << fitness[i] << ' ';
	cout << "\n";
}

//getMaxFlow using genetic algorithm
pair<flowT, vector<vector<flowT>>> CapacityDistributionAlgo::getMaxFlow(int s, int t,
	int ITERATIONS, int POPULATION_SIZE,
	double CROSSOVER_PROBABILITY) {

	int oldPercent = 0;
	int mutationPercent = 30;
	int oldCnt = double(oldPercent) / 100 * POPULATION_SIZE;
	int mutationCnt = double(mutationPercent) / 100 * POPULATION_SIZE;
	
	//init population
	population.clear();
	population.resize(POPULATION_SIZE);
	for (int i = 0; i < POPULATION_SIZE; i++)
		population[i] = Individual(graph.n, graph);

	for (int iteration = 1; iteration <= ITERATIONS; iteration++) {
		vector<flowT> populationFitness(POPULATION_SIZE);
		for (int i = 0; i < POPULATION_SIZE; i++)
			populationFitness[i] = getFitness(population[i], graph, s, t);

		//log current iteration
		logIteration(iteration, populationFitness);

		double avgFitness = accumulate(populationFitness.begin(), populationFitness.end(), double(0)) / POPULATION_SIZE;
		vector<double>crossoverDistribution(POPULATION_SIZE);
		for (int i = 0; i < POPULATION_SIZE; i++) {
			crossoverDistribution[i] = populationFitness[i] / avgFitness;
		}

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

		for (int i = 0; i < mutationCnt; i++) {
			int idx = Math::genInt(POPULATION_SIZE);
			newPopulation[idx] = mutation(newPopulation[idx], graph,20);
		}

		population = newPopulation;


	}

	return {};
}
