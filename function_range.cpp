#include "function_range.h"

// access values
ll Function::operator[](ll x) const { return f[x]; }

// nonnull-function?
Function::operator bool() const {
	return any_of(all(f),[](ll y){return y!=0;});
}
Function::operator vi() const {
	return f;
}

bool Function::operator++(){
	auto i = f.begin();
	auto e = f.end();
	for(; i != e; ++i){
		++*i;
		if(*i==b) *i=0;
		else break;
	}
	return i != e;
}

ostream &Function::write(ostream &o) const{
	o << name << " : [0,"<<a<<") -> [0,"<<b<<")" << endl;
	//for(ll x = 0; x < a; ++x)
		//o << setw(3) << x << " -> " << setw(3) << f[x] << "\n";
	for(ll x = 0; x < a; ++x) o << setw(3) <<   x  << " ";
	o << "\n";
	for(ll x = 0; x < a; ++x) o << setw(3) << f[x] << " ";
	o << "\n";
	o.flush();
	return o;
}
