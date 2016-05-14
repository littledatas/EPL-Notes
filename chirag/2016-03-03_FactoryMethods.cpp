#include <iostream>
#include <cstdint>
#include <string>
#include <map>
#include <functional>

using std::cout;
using std::endl;
using std::string;
using std::map;

class Base {
public:
};

class D1 : public Base {
public:
};

class D2 : public Base {
public:
};

// first iteration, not object oriented at all
Base * factory(string type) {
	if(type == "D1") { return new D1; }
	if(type == "D2") { return new D2; }
}

// second iteration, use map (called punting)
using CreateFun = std::function<Base*(void)>;
map<string, CreateFun> type_fun;

Base *factory(string type) {
	CreateFun fun = type_fun[type];
	return fun();
}

int main(void) {

}

/*******************************************/

std::map<std::string, CreateFun>& getMap(void) {
	static std::map<std::string, CreateFun> types;
	return types;
}

class _InitD1 {
public:
	_InitD1(void) {
		std::map<std::string, CreateFun>& the_real_map the_real_map = getMap();
		the_real_map["D1"] = &D1::makeD1;
	}
};

class _InitD2 {
public:
	_InitD1(void) {
		std::map<std::string, CreateFun>& the_real_map the_real_map = getMap();
		the_real_map["D2"] = &D2::makeD2;
	}
};

class D1 : public Base {
public:
	static Base * makeD1(void) {
		return new D1;
	}

	// TODO: can we make this private?
	static _InitD1 fred;
};

class D2 : public Base {
public:
	static Base * makeD2(void) {
		return new D2;
	}
};

// two ways of adding to map. we need to do this so that the map is populated at initialization time.
// first way is just to make the variable static so there are no name conflicts
static _InitD1 fred1;
// other way is to scope it in the class and then initialize it out here
_InitD1 D1::fred;

/* global variables are constructed in the order they are declared in a file. in gcc, the first file on the command line will have its globals declared first
   usually if code depends on the order in which the files are put and the objects are constructed, the code sux
   this isn't a problem here, but if you have multiple files with the base classes, the order is significant
   however, the solution is to use a function to get the map, ensuring that the map is constructed first
*/

/*****************************************/

class Base {
public:
	virtual void doit(void) = 0;

	virtual Base * clone(void) {
		return new Base { *this }:
	}
};

class D1 : public Base {
public:
	static Base * makeD1(void) {
		return new D1;
	}

	void doit(void) {
		cout << "D1\n";
	}

	virtual Base * clone(void) {
		return new D1 { *this };
	}

	static _InitD1 fred;
};

class D2 : public Base {
public:
	static Base * makeD2(void) {
		return new D2;
	}

	void doit(void) {
		cout << "D2\n";
	}

	// this is changing the return type of the clone function, but it's alright because D2 "is a" Base, meaning
	// everything in the Base class is available in D2 so static type checking works
	virtual D2 * clone(void) {
		return new D2 { *this };
	}
};

class Wrap {
	Base * ptr;
public:
	Wrap(Base * ptr) { ptr = p; }
	~Wrap(void) { delete ptr; }

	void doit(void) {
		ptr->doit();
	}

	Wrap(Wrap const& rhs) {
		// this will unequivocally create a copy of type Base, even though the ptr could be a derived type
		// there is no language semantic to have virtual constructors, even though copy construcor may be an exception
		ptr = new Base{ *(rhs.ptr) };
	}
};

void doit(Wrap w) {
	w.doit();
}

int main(void) {
	Wrap w { factory("D1") };
	doit(w);
	w.doit();
	w.warp();
}