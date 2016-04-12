class Foo {
public:
	Foo(void) { cout << "no-arg construction\n"; }
	Foo(int) { cout << "int constructor\n"; }
	Foo(Foo const&) { cout << "copy constructor\n"; }
	Foo(Foo&&) { cout << "move constructor\n"; }
	~Foo(void) { cout << "destructor\n"; }
};

int core_cpp_syntax(void) {
	Foo *p = (Foo *) malloc(sizeof(Foo)); 	// just allocates space, so nothing prints out
	Foo *q = new Foo; 	// no-arg constructor prints out and nothing else (no deallocation)
	/*
		::operator is the new operator in the global namespace (function call with a space for some reason)
		using function call syntax for the new operator actually has a different result
		this line is equivalent to calling malloc expoiting the intentional loophole in c++
	*/
	Foo *r = (Foo *) ::operator new(sizeof(Foo));
	// calls just the constructor without allocating space (called plcaement new)
	new (r) Foo{};

	r->~Foo();
	free(r);
	/*
		Following line may work, but is not required to
		delete r;
		Following line is the same as free
		::operator delete(p);
	*/

	// creates an unnamed temporary variable of type Foo
	Foo{};
    // we don't want the developers to call our constructors willy nilly, so instead we introduce
    // placement new syntax, which kinda combines the gross thing on top and the bottom 
    p->Foo();

	// initializes 10 Foos with default constructor (can only use default constructor)
	Foo *s = new Foo[10];
}

// does not need to be templatized, and should use void * instead
template <typename T>
struct allocator {
	T *allocate(uint64_t num) const {
		return (T *) ::operator new(num * sizeof(T));
	}

	void deallocate(T *p) const {
		::operator delete(p);
	}
	void construct(T *p) const {
		new (p) T{};
	}
	void construct(T *p, T const& ref) const {
		new (p) T { ref }:
	}
	void destroy(T *p) const {
		p->~T();
	}
};

void via_std_allocator(void) {
	/* standard allocator can be used to separate the allocator from your data structure
	   useful if you want to do something like switch from allocating in dram to sram - just change your allocator
	*/
	allocator<Foo> my_alloc{};
	Foo *p = my_alloc.allocate(sizeof(Foo));
	allocator<Foo>.construct(p);
	my_alloc.destroy(p);
	allocator<Foo>.deallocate(p);
}

void doit(int k) {
	if(k < 0 || k >= 10) {
		throw std::out_of_range{"oops"};
	}
}

int main(void) {
	try {
		doit(42);
	} catch(std::out_of_range& x) {
		cout << "you messed up " << x.what() << std::endl;
	} catch(...) { 	// ... is catch all
		cout << "you messed up\n";
	}
}
