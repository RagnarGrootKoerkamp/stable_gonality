#include "header.h"

// function from [0,a) to [0,b)
// ++f to increment the function
struct Function {
	ll a,b;
	vi f;
	string name;
	Function(ll a, ll b, string name="") :
		a(a), b(b), f(a,0), name(name) {}

	// access values
	ll operator[](ll x) const;

	// nonnull-function?
	operator bool() const;
	operator vi() const;
	bool operator++();

	ostream &write(ostream &o) const;
};

inline
ostream &operator<<(ostream &o, const Function &f){
	return f.write(o);
}
