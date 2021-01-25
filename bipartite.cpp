#include "header.h"
#include "graph.h"

int main(int argc, char** args){
	ll a, b;
	if(argc<3)
		cin >> a >> b;
	else {
		a = stoi(args[1]);
		b = stoi(args[2]);
	}
	Graph::complete_bipartite(a,b).write_edge_format(cout);
}
