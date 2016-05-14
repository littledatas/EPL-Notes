template <typename T>
struct Vector {
	T* pointer;
	uint64_t length;

	Vector(void) {
		pointer = nullptr;
		length = 0;
	}

	void push_back(T) {}

	// convention in C++ is to send data structures as iterators instead of the actual
	//   data structure
	template <typename It>
	// the : Vector() is called a delegating constructor (equivalent to putting super() in Java)
	Vector(It begin, It end) : Vector() {
		while(begin != end) {
			// sometimes it is necessary to introduce this-> in front of function calls
			//   because the compiler may not be able to figure out that push_back is
			//   part of this class
			this->push_back(*begin);
			++begin;
		}
	}

	// TODO: check if convention says to make this constructor a member template (so that 
	//   you can initialize an int vector with char, for example) or not
	template <typename
	Vector(std::initializer_list<int> list) : Vector(list.begin(), list.end()) {}

	// it sucks to have to send in a thing using push_back and then move it into place,
	//   so just construct it in place
	/*void emplace_back(void) {
		new (pointer + length) T {};
	}*/

	// this is also nice because the user probably doesn't want to call the default constructor,
	//   so we can send in an Arg. what happens if the constructor has 2 or 3 args instead of 1?
	/*template <typename Arg>
	void emplace_back(Arg x) {
		new (pointer + length) T{ x };
	}*/

	// variadic! (can be 0 or more args, so we just need this one)
	template <typename... Args>
	void emplace_back(Args... args) {
		new (pointer + length) T{ args... };
	}

	// if Arg is an rvalue reference, it gets turned into an lvalue reference when T{arg} is called
	// this is the perfect forwarding problem for rvalues
	template <typename Arg>
	void doit(Arg arg) {
		T{ arg };
	}

	// TODO: what the actual fuck is going on here
	template <typename... Args>
	void emplace_back(Args&&... args) {
		T { std::forward<Args>(args)... };
	}
};

// TODO: REWATCH THE LAST 15 MINUTES OF LECTURE BECAUSE WAT

struct Foo {
	Foo(void) { cout << "construcor0\n" };
	Foo(int, const char *, double) { cout << "constructor1\n"; }
	Foo(double, int) { cout << "constructor2\n"; }
};

template <typename T>
void doit() {
	cout << "T is" << typeid(T).name() << endl;
	cout << "one\n";
}

template <typename T>
void doit(T&&) {
	cout << "T is " << typeid(T).name() << endl;
	cout << "two\n";
}

int main(void) {
	doit<int>(42);	// two
	int x = 42;
	doit<int>(x);	// one
}

int main(void) {
	int array[10];
	char s[] = "Hello World";
	Vector<int> vector(s, s + 12);
	Vector<int> vec2 = { 1, 2, 3, 4, 5, 6 };

	vector.emplace_back();
	vector.emplace_back(42);

	Vector<Foo> vec3{};
	vec3.emplace_back();
	vec3.emplace_back(42, "hello", 3.14159);
}