class Foo {
public:
	~Foo(void) { cout << "Destroying Foo\n"; }
};

class Base {
public:
	virtual ~Base(void) = default;
	// mark the function as abstract, meaning there is no implementation of the function in this class
	// derived classes must implement it
	virtual void draw(void) = 0;
};

class Derived : public Base {
public:
	Foo f;
	virtual void draw(void) override {}
};

int main(void) {
	// this will cause compiler error because you can't construct it if it's non-functional
	// constructor can technically run, but it must be in the constructor of a derived class (when the derived class is constructing the base object)
	Base b;

	Derived d;
	Base *b = new Derived;
}

// --------------------------------------
// following is a classic example of why you would declare a function as pure virtual
// you implement a sick sorting function but don't care about the comparison, so everyone who is using your sorter should implement the comparator
class Sorter {
public:
	virtual bool lessThan(int a1, int a2) = 0;

	void sort(int x[], int n) {
		/* ... */
		if(lessThan(x[k], x[j])) {

		}
	}
};

class SortAscending : public Sorter {

};