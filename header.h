#pragma once
//#define _GLIBCXX_DEBUG
#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <queue>
#include <string>
#include <tuple>
#include <vector>

#define all(v) begin(v), end(v)

using namespace std;

using ll  = long long;
using ull = unsigned long long;
using vi  = vector<ll>;
using vvi = vector<vi>;
using vb  = vector<bool>;
using ii  = pair<ll, ll>;

constexpr bool LOG = true;
inline void Log() {
	if(LOG) cerr << "\n";
}
template <class T, class... S>
inline void Log(T t, S... s) {
	if(LOG) cerr << t << "\t", Log(s...);
}
