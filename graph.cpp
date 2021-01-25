#include "graph.h"

void Graph::add_edge(ll u, ll v) {
	++m;
	if(u == v) {
		auto &eu = edges[u];
		eu.push_back({u, ll(eu.size() + 1)});
		eu.push_back({u, ll(eu.size() - 1)});
	} else {
		ll i1 = edges[u].size(), i2 = edges[v].size();
		edges[u].push_back({v, i2});
		edges[v].push_back({u, i1});
	}
}

ostream &Graph::write(ostream &o) const {
	o << setw(3) << n << " " << setw(3) << m << "\n";
	for(int v = 0; v < n; ++v) {
		o << setw(3) << v << ":\t";
		for(auto &e : edges[v])
			o << "(" << setw(3) << e.v << "," << setw(3) << e.i << ")";
		o << "\n";
	}
	return o;
}

ostream &Graph::write_edge_format(ostream &o) const {
	o << n << " " << m << "\n";
	for(int v = 0; v < n; ++v) {
		for(ull i = 0; i < edges[v].size(); ++i) {
			auto &e = edges[v][i];
			if(tie(v, i) < tie(e.v, e.i)) o << v << " " << e.v << "\n";
		}
	}
	return o;
}

istream &Graph::read(istream &i) {
	this->m = 0;
	ll m;
	i >> n >> m;
	edges.assign(n, {});
	for(ll e = 0; e < m; ++e) {
		ll u, v;
		i >> u >> v;
		add_edge(u, v);
	}
	return i;
}

// repeatedly remove vertices with degree 1 or 2
void Graph::stabilize() {
	vi deg(n);
	transform(all(edges), deg.begin(), [](const vector<Edge> &v) { return v.size(); });

	queue<ll> q;
	for(ll v = 0; v < n; ++v)
		if(deg[v] == 1 || deg[v] == 2) q.push(v);

	while(!q.empty()) {
		ll v = q.front();
		q.pop();
		if(deg[v] <= 0) continue;

		if(deg[v] == 1) {
			Log("erase leaf ", v);
			// remove edge; decrease degrees
			Edge e;
			for(auto &x : edges[v])
				if(x.v >= 0) {
					e   = x;
					x.v = -1; // erase edge
					break;
				}

			edges[e.v][e.i].v = -1;

			--deg[e.v];
			if(deg[e.v] <= 2) q.push(e.v);

			deg[v] = -1;
			--m;
		}

		if(deg[v] == 2) {
			Log("contract deg 2 vertex ", v);
			array<Edge, 2> es;
			ll i = 0;
			for(auto &x : edges[v])
				if(x.v >= 0) {
					es[i++] = x;
					// skip if this is a loop
					if(x.v == v) goto end;
					x.v = -1; // erase edge from v
					if(i == 2) break;
				}

			auto &e1 = es[0], &e2 = es[1];

			edges[e1.v][e1.i] = e2;
			edges[e2.v][e2.i] = e1;

			deg[v] = -1;
			--m;
		}
	end:;
	}

	Log("after stabilizing:");
	Log(*this);

	Log("sanitizing...");
	sanitize(deg);
}

void Graph::sanitize(vi &deg) {

	vi vertexmap(n, -1); // renumbering of the vertices
	vvi edgemaps;        // renumbering of indices per vertex

	vector<vector<Edge>> new_edges;

	ll i = 0;
	for(ll v = 0; v < n; ++v) {
		auto &es = edges[v];
		ll j     = 0;
		vi edgemap(es.size(), -1);
		vector<Edge> new_es;
		for(ull k = 0; k < es.size(); ++k)
			if(es[k].v >= 0) {
				edgemap[k] = j++;
				new_es.push_back(es[k]);
			}

		if(deg[v] >= 0) {
			vertexmap[v] = i++;
			edgemaps.push_back(edgemap);
			new_edges.push_back(new_es);
		}
	}

	// now rewrite vertex numbers and indices
	for(auto &es : new_edges)
		for(auto &e : es) {
			e.v = vertexmap[e.v];
			e.i = edgemaps[e.v][e.i];
		}

	// and save the result
	n     = i;
	edges = new_edges;
}

vector<Graph> Graph::rooted_universal_trees;

Graph Graph::rooted_universal_tree(ll k) {
	if(rooted_universal_trees.size() > ull(k)) return rooted_universal_trees[k];

	if(k == 0) return Graph();

	Graph g(1); // a single vertex (the root)
	ll root = 0;
	for(ll i = 1; i <= k - 1; ++i) {
		// add U[(k-1)/i] below the root of g.

		auto subtree = rooted_universal_tree((k - 1) / i);

		ll old_size = g.n;
		g.n += subtree.n;
		g.edges.reserve(g.n);
		for(auto &x : subtree.edges) {
			// modify the vertices of x
			for(auto &e : x) e.v += old_size;
			g.edges.push_back(x);
		}
		// join root to root of subtree
		g.add_edge(root, old_size);
	}

	if(rooted_universal_trees.size() <= ull(k)) rooted_universal_trees.resize(k + 1);
	return rooted_universal_trees[k] = g;
}

Graph Graph::complete(ll n) {
	Graph g(n);
	// just add all edges
	for(ll u = 0; u < n; ++u)
		for(ll v = 0; v < u; ++v) g.add_edge(u, v);

	return g;
}

Graph Graph::complete_bipartite(ll a, ll b) {
	Graph g(a + b);
	// just add all edges
	for(ll u = 0; u < a; ++u)
		for(ll v = a; v < a + b; ++v) g.add_edge(u, v);

	return g;
}
