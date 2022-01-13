#include<iostream>
#include<vector>
#include<random>
#include<stdio.h>
#include<utility>
#include<time.h>
#pragma warning:disable(4996)

using namespace std;

int capLower = 1, capUpper = 100;
int hLower = 0, hUpper = 10;

mt19937 gen(time(0));

pair<int, int> randEdge() {
	vector<int> prob = { 70,20,10 };
	
	discrete_distribution<>dist(prob.begin(), prob.end());
	int edgeType = dist(gen);
	uniform_int_distribution<> uid(capLower, capUpper);
	int edgeCap = uid(gen);

	return { edgeCap, edgeType };
}

void addPath(int v, vector<bool>&used, vector<vector<pair<int,int>>>& g,  int n) {
	used[v] = true;
	if (v == n - 1)
		return;

	vector<int> unused;
	for (int i = 0; i < n; i++)
		if (!used[i] && i>v)
			unused.push_back(i);

	uniform_int_distribution<>uid(0, unused.size() - 1);
	int to = unused[uid(gen)];

	g[v][to] = randEdge();
	addPath(to, used, g, n);
}

void genNetwork(int n, int steps) {
	vector<vector<pair<int, int>>>g(n, vector<pair<int, int>>(n, { -1,-1 }));

	for (int it = 0; it < steps; it++) {
		vector<bool>used(n);
		addPath(0, used, g, n);
	}


	vector<vector<int>>edges;
	for(int i = 0;i<n;i++)
		for (int j = 0; j < n; j++) {
			if (g[i][j].second!=-1) {
				edges.push_back({ i,j,g[i][j].first,g[i][j].second });
			}
		}

	uniform_int_distribution<>uid2(hLower, hUpper);
	int h = uid2(gen);
	cout << n << ' ' << h << endl;
	cout << edges.size() << endl;
	for (auto to : edges) {
		cout << to[0] << ' ' << to[1] << ' ' << to[2] << ' ' << to[3] << endl;
	}
}

int main(int argc, char* argv[]) {

	freopen("test.txt","w",stdout);

	int n = atoi(argv[1]);
	int steps = atoi(argv[2]);

	genNetwork(n, steps);
}