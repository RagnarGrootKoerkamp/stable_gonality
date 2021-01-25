#include "header.h"
#include "graph.h"

int main(){
	Graph g(cin);
	cout << g << endl;
	cout << "stabilizing..." << endl;
	g.stabilize();
	cout << g << endl;
}
