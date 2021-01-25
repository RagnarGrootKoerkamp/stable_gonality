#pragma once
#include "header.h"

struct Graph {
	
	struct Edge {
		// v: the index of the target vertex
		// i: the index in the list of the target vertex
		ll v, i;
	};

	ll n, m;	// #vertices, #edges
	vector<vector<Edge>> edges;

	Graph(ll n=0) : n(n), m(0), edges(n) {};
	Graph(istream &i) { read(i); }

	void add_edge(ll u, ll v);

	// print
	ostream &write(ostream &o) const;
	ostream &write_edge_format(ostream &o) const;

	// read
	istream &read(istream &i);

	// remove vertices with degree 1 or 2
	void stabilize();

	// remove edges with v=-1
	// renumber; skip erased vertices
	void sanitize(vi &deg);

	// rooted universal tree
	// containing all trees on k vertices
	static vector<Graph> rooted_universal_trees;
	static Graph rooted_universal_tree(ll k);

	static Graph complete(ll n);
	static Graph complete_bipartite(ll a, ll b);
};

inline
ostream &operator<<(ostream &o, const Graph &g){
	return g.write(o);
}
inline
istream &operator>>(istream &i, Graph &g){
	return g.read(i);
}
