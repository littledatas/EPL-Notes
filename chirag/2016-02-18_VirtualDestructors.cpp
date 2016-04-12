#include <iostream>
#include <cstdint>

using std::cout;
using std::endl;

class Base {
public:
	// really no reason to ever have a non-virtual destructor, because why would you not want to destroy the object you created?
	// although....you can get higher performance if you really don't need to call the derived destructors
	virtual ~Base(void) { cout << "base class too\n"; }
};

class Derived : public Base {
public:
	~Derived(void) { cout << "Ok, cleaned up\n"; }
};

int main(void) {
	Base * p = new Derived;
	// if there is no vtable, the incorrect destructor will be called and data will be lost on the heap
	// with vtable, it will destroy derived class and then base class
	delete p;
	/* When a derived class is constructed, the vtable pointer actually changes based on the current thing that is being constructed
	   This is important, because if there are function calls in the constructor, they should NOT call the derived functions (e.g.
	   construct function being called in constructor, where the construct function is virtual).
	   Works the same way for destructors.
	*/
}

// -----------------------------------------

class Foo {
public:
	~Foo(void) { cout << "Destroying Foo\n"; }
};

class Base {
public:
};

class Derived : public Base {
public:
	Foo f;
};

int main(void) {
	Base * p = new Derived;
	// THIS WILL NOT CALL THE FOO DESTRUCTOR, SINCE THERE IS NO DESTRUCTOR CALLED FOR DERIVED, EVEN THOUGH IT DEFINITELY SHOULD BE
	// DELETING f
	// basically if there is no destructor, just explicitly put a damn default destructor
	delete p;
}