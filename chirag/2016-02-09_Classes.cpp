/*
Structs are aggregation of data
	- Compiler must determine the size and layout of the struct data
Member functions are no different than normal functions, but they just have access to private
members of the class
	- "this" pointer is implicitly defined as "T *const this"
	- The const is after the  *, meaning this cannot be reassigned

Inheritance
	- Allows a new type to be defined as an extension of an existing type (inheritance)
	- Creates a subtype/supertype relationship (subtyping)
		- Subtyping creates an "is a" relationship

Public inheritance (where the parent's public members stay public in derived class) should be default
because it is the most logical, but C++ defaults to private (parent's public members become private
in derived class)

Constructors can be inherited as of c++11
Most methods are inherited, including assignment operator (but return types do not change so if you cascade
equals operators, the type changes in the middle)
Destructors are not inherited
	- sequence: 1) parent constructor, 2) derived constructor, 3) derived destructor, 4) parent destructor
*/

int main(void) {
	// there is also a vector initialization that accepts a list of values to initialize value with
	// by default, using {a, b, c, ...}. C++ defaults to treating {10} as a single-element list instead
	// of uniform initialization. we must use (10) here.
	vector<int> x(10);
	x.push_back(42);
	x.pop_back();
	// this should cause an error from buffer overflow but std::vector doesn't do bounds checking
	// we want to create a vector that does do bounds checking
	cout << x[10] << endl;
}

class myvec : public vector<int> {
public:
	/* this is pre c++11 syntax for calling parent constructors. it works, but if the parent constructors change,
	   we have to match a corresponding constructor in the derived class
	myvec(int sz) : vector<int>(sz) {

	}*/
	// c++11 way of inheriting all constructors from parent class
	using vector<int>::vector;
	// you can still override a constructor if you want
	myvec(int sz) : vector<int>(sz) {}
	// add bounds checking to operator[]
	int & operator[](int k) {
		if(k < 0 || k >= size()) {
			cout << "WTF dude?\n";
		}
		/* following line won't work because it will call its own operator[]
		return (*this)[k];
		*/
		// one way of calling parent class's method
		return vector<int>::operator[](k);
		// another way of calling parent class's method
		vector<int> & base = *this;
		return base[k];
	}
};

int main(void) {
	myvec x(10);

	x.push_back(42);
	x.pop_back();
	cout << x[10] << endl;
}

/* Base class becomes part of the inherited class.
C++ puts the base class first in memory in the inherited class so that it doesn't need to change offsets

subtyping in c++ is free because the first bytes are of the base class, so all old code still works
*/
class Base1 {
public:
	int x;
	int y;
};

class Derived : public Base1 {
public:
	int z;
};

// if you sent a Derived class, it will construct a new Base1 (since b is a copy)
// types may change in function call
void doit(Base1 b) {

}
// better to use a reference, since it doesn't change the type
void doit(const Base1 & b) {

}

int main(void) {
	// offsetof is a directive (like sizeof) that tells you the offset of a field in a class. doesn't work on all types, but does work on POD.
	int x_off = offsetof(Base1, x);
	int y_off = offsetof(Base1, y);

	cout << "in Base1 x has offset " << x_off; 	// at 0
	cout << " and y has offset " << y_off; 		// at 4
	cout << endl;

	x_off = offsetof(Derived, x);
	y_off = offsetof(Derived, y);
	int z_off = offsetof(Derived, z);

	cout << "in Derived x has offset " << x_off; 	// at 0
	cout << " and y has offset " << y_off; 		// at 4
	cout << " and z has offset " << z_off << endl; 	// at 8

	Base1 b;
	Derived d;

	// DOESN'T WORK. how can it possibly produce the extra fields for Derived?
	d = b;
	// asking compiler to copy all of the Base1 state from Derived (which is a larger object)
	b = d;
}

// similar example to above, but with multiple inheritance
class Base1 {
public:
	int x;
	int y;

	void doit() {
		cout << "doit at " << this << " in Base1\n";
		whoa();
	}

	virtual void whoa(void) { cout << "Base1\n"; }
};

class Base2 {
public:
	int z;

	void doit() {
		cout << "doit at " << this << " in Base2\n";
		whoa();
	}

	virtual void whoa(void) { cout << "Base2\n"; }
};

class Derived : public Base1, public Base2 {
public:
	int z;

	void doit(void) {
		cout << "doit at " << this << " in Derived\n";
	}

	virtual void whoa(void) { cout << "derived\n"; }
};

void inheritMemberFunctions(void) {
	Derived d;
	cout << "d is at " << &d << endl;
	d.Base1::doit(); 	// prints out same address as d
	d.Base2::doit();	// prints out offsetted version of address (8 bytes) because it needs to adjust offsets to emulate location of "this"
						// also prints is "derived", because it is calling the virtual function whoa which is overridden
	// do not need to explicitly say Derived:: for this one, since compiler will automatically bound to methods in Derived class
	d.Derived::doit(); 	// prints out address at d because compiler knows about entire class
}

int main(void) {
	int x_off = offsetof(Base1, x);
	int y_off = offsetof(Base1, y);

	cout << "in Base1 x has offset " << x_off; 	// at 0
	cout << " and y has offset " << y_off; 		// at 4
	cout << endl;

	int z_off = offsetof(Base2, z);
	cout << "in Base2 z has offset " << z_off; 	// at 0
	cout << endl;

	x_off = offsetof(Derived, x);
	y_off = offsetof(Derived, y);
	z_off = offsetof(Derived, z);

	// Visual Studio puts inherited objects back to back
	cout << "in Derived x has offset " << x_off; 	// at 0
	cout << " and y has offset " << y_off; 		// at 4
	cout << " and z has offset " << z_off << endl; 	// at 8
}