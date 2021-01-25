#include "header.h"
#include "graph.h"
#include "function_range.h"

void dfs_edgeindex(ll r, ll u, ll p, const Graph &U, vvi &edgeindex){
	for(ull i = 0; i < U.edges[u].size(); ++i){
		auto &e = U.edges[u][i];
		ll v = e.v;
		if(v==p) continue;
		edgeindex[v][r] = e.i;
		dfs_edgeindex(r,v,u,U,edgeindex);
	}
}

ll sgon(Graph G){
	cerr << "input:\n" << G << endl;
	G.stabilize();
	cerr << "after stabilizing:\n" << G << endl;

	cerr << "vertices: " << G.n << ", edges: "<<G.m<<endl;

	if(G.m == 0){
		cerr << "G is a tree; returning 1" << endl;
		return 1;
	}
	if(G.n == 1){
		cerr << "G has 1 vertex with loops; returning 2" << endl;
		return 2;
	}

	cerr << "Building rooted_universal_tree("<<G.n<<")"<<endl;
	Graph U = Graph::rooted_universal_tree(G.n);
	cerr << U << endl;


	cerr << "Start iterating over maps f:V(G)->V(U)" << endl;


	// we first do some preprocessing
	// to determine for all (U,v) in V(U),
	// which edge from U leads to v
	
	vvi edgeindex(U.n);
	for(auto &x : edgeindex) x.assign(U.n,-1);
	for(ll r = 0; r < U.n; ++r)
		dfs_edgeindex(r, r, -1, U, edgeindex);

	for(auto &x : edgeindex){
		copy(all(x), ostream_iterator<ll>(cerr," "));
		cerr << endl;
	}

	ll sgon = 1000000000;

	// the image of vertex 0 is the root of U
	// because U contains all rooted trees
	Function f(G.n, U.n,"f");
	do {
		cerr << f << endl;
		// now loop over assignments of indices
		// each of the m edges has index < m
		
		// we skip edges uv with f[u]=f[v]
		ll edge_count = 0;
		for(ll u = 0; u < G.n; ++u)
			for(const auto &e : G.edges[u])
				if(f[e.v] < f[u]) ++edge_count;
		Function r(edge_count, min(sgon,(G.m-G.n+4)/2),"r");
		do {
			//cerr << r << endl;

			// keep a counter for every (v,e') tuple,
			// with v in V(G),
			// and e' an edge adjacent to f(v)
			
			vector<vi> c(G.n);
			for(ll u = 0; u < G.n; ++u)
				c[u].assign(U.edges[f[u]].size(),0);

			// minimal value of m[v], for f[u]=f[v] edges
			vi min_m(G.n,0);

			// we calculate the degree of the FHM by
			// counting the number of times we cover
			// vertex 0 in U
			ll degree = 0;

			// now; process all of G's edges
			ll ri = 0;
			for(ll u = 0; u < G.n; ++u){
				for(const auto &e : G.edges[u]){
					ll v = e.v;
					ll fu = f[u];
					ll fv = f[v];
					if(fu==fv){
						if(u==v) min_m[u]=max(min_m[u],2ll);
						else {
							min_m[u]=max(min_m[u],1ll);
							min_m[v]=max(min_m[v],1ll);
						}
						continue;
					}
					// we process each edge only in 1 direction
					if(fv>fu)
						continue;

					ll index = r[ri]+1;
					++ri;
					
					// otherwise, increment the counter (u,e'),
					// where e' is the edge from fu in the direction
					// of fv.
					c[u][edgeindex[fu][fv]] += index;
					c[v][edgeindex[fv][fu]] += index;

					// if fu-fv covers 0 internally, we add r[ri] to the degree
					if(fu!=0 && fv!=0 &&
					   edgeindex[fu][0] == edgeindex[fu][fv] &&
					   edgeindex[fv][0] == edgeindex[fv][fu])
						degree += index;

				}
			}

			// now, calculate the index of all vertices,
			// and add subtrees if needed.
			// We don't actually add the subtrees, but we do
			// count the subtrees covering 0
			
			for(ll u = 0; u < G.n; ++u){
				assert(c[u].size() > 0);
				ll mu = *max_element(all(c[u]));
				mu = max(mu, min_m[u]);
				ll fu = f[u];
				if(fu != 0) {
					degree += mu - c[u][edgeindex[fu][0]];
				} else {
					degree += mu;
				}
			}

			if(degree < sgon){
				cerr << "New best: " << degree << "\n";
				cerr << f << r << endl;
				sgon = degree;
			}
		} while(++r);
		// the image of vertex 0 is the root of U
		// because U contains all rooted trees
	} while(++f && f[G.n-1]==0);


	return sgon;
}

int main(){
	//Graph g = Graph::complete(n);
	Graph g(cin);
	ll s = sgon(g);
	cout<<"stable gonality of input: "<<s<<endl;
	return 0;
}
