#include "header.h"
#include "graph.h"

int main(){
	ll n;
	while(cin >> n && n>0){
		cout << "Minimal universal tree " << n <<":\n";
		cout << Graph::rooted_universal_tree(n) << endl;
	}
}
