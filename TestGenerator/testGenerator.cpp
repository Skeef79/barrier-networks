#include<iostream>
#include<vector>
#include<random>
#include<stdio.h>
#include<utility>
#include<time.h>
#include<string>
#include<fstream>
#include<string>

#pragma warning:disable(4996)

using namespace std;

double edgeProbability = 0.7;
int capacityLb = 1;
int capacityUb = 200;
vector<int> edgeTypesDistribution = {7,2,1};
const int NMAX = 10000;

mt19937 gen;
mt19937 edgeGen;

bool isNeed(double probability) {
    uniform_int_distribution<> uid(1,1000);
    int intProbability = (int)(probability*1000);
    return uid(edgeGen) < intProbability;
}

pair<int, int> randEdge() {
	discrete_distribution<>edgeTypeDist(edgeTypesDistribution.begin(), edgeTypesDistribution.end());
	int edgeType = edgeTypeDist(gen);
	uniform_int_distribution<> edgeCapDist(capacityLb, capacityUb);
	int edgeCap = edgeCapDist(gen);
	return { edgeCap, edgeType };
}


//gen n h fname
int getSeed(int n, int h) {
    string s;
    s+=to_string(n);
    s+=to_string(h);
    s+=to_string(edgeProbability);
    s+=to_string(capacityLb);
    s+=to_string(capacityUb);
    s+=to_string(edgeTypesDistribution[0]);
    s+=to_string(edgeTypesDistribution[1]);
    s+=to_string(edgeTypesDistribution[2]);

    int seed =0;
    int pw = 1;
    int p = 33;
    for(int i = 0;i<s.length();i++) {
        seed+=pw*s[i];
        pw*=p;
    }
    return seed;
}


//n h edge_prob capLb capUb edgeDistr0 edgeDistr1 edgeDistr2
int main(int argc, char* argv[]) {
	//freopen("test.txt","w",stdout);

	int n = atoi(argv[1]);
    int h = atoi(argv[2]);
    double edgeProbability = stod(argv[3]);
    capacityLb = stoi(argv[4]);
    capacityUb = stoi(argv[5]);
    edgeTypesDistribution[0] = stoi(argv[6]);
    edgeTypesDistribution[1] = stoi(argv[7]);
    edgeTypesDistribution[2] = stoi(argv[8]);

    string fname(argv[9]);
	
    gen.seed(getSeed(n,h));
    edgeGen.seed(n);

    vector<vector<int>>edges;
    for(int i = 0;i<n;i++) {
        for(int j = i+1;j<n;j++){
            if(isNeed(edgeProbability)) {
                auto [edgeCapacity, edgeType] = randEdge();
                edges.push_back({i,j,edgeCapacity,edgeType});
            }
            else {
                edges.push_back({i,j,0,2});
            }   
        }
    }

    ofstream fout(fname);

    fout << n << endl << h << endl << edges.size() << endl;
    for(auto edge:edges) {
        fout << edge[0] << ' ' << edge[1] << ' ' << edge[2] << ' ' << edge[3] << endl;
    }
}