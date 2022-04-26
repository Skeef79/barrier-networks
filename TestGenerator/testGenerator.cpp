#include<iostream>
#include<vector>
#include<random>
#include<stdio.h>
#include<utility>
#include<time.h>
#include<string>
#include<fstream>

#pragma warning:disable(4996)

using namespace std;

const double edgeProbability = 0.7;
const int capacityLb = 1;
const int capacityUb = 200;
const vector<int> edgeTypesDistribution = {7,2,1};
const int seed = 2022;
const int NMAX = 10000;

mt19937 gen;

bool isNeed(double probability) {
    uniform_int_distribution<> uid(1,1000);
    int intProbability = (int)(probability*1000);
    return uid(gen) < intProbability;
}

pair<int, int> randEdge() {
	discrete_distribution<>edgeTypeDist(edgeTypesDistribution.begin(), edgeTypesDistribution.end());
	int edgeType = edgeTypeDist(gen);
	uniform_int_distribution<> edgeCapDist(capacityLb, capacityUb);
	int edgeCap = edgeCapDist(gen);
	return { edgeCap, edgeType };
}


//gen n h fname
int main(int argc, char* argv[]) {
	//freopen("test.txt","w",stdout);

	int n = atoi(argv[1]);
    int h = atoi(argv[2]);
    string fname(argv[3]);
	
    gen.seed(n*NMAX + h);

    vector<vector<int>>edges;
    for(int i = 0;i<n;i++) {
        for(int j = i+1;j<n;j++){
            if(isNeed(edgeProbability)) {
                auto [edgeCapacity, edgeType] = randEdge();
                edges.push_back({i,j,edgeCapacity,edgeType});
            }   
        }
    }

    ofstream fout(fname);

    fout << n << endl << h << endl << edges.size() << endl;
    for(auto edge:edges) {
        fout << edge[0] << ' ' << edge[1] << ' ' << edge[2] << ' ' << edge[3] << endl;
    }
}