#include "header.h"
#include "graph.h"

int main(int argc, char** args){
	ll n;
	if(argc<2)
		cin >> n;
	else {
		n = stoi(args[1]);
	}
	Graph::complete(n).write_edge_format(cout);
}
