#include <iostream>
#include <cstdint>
#include <typeinfo>

using std::cout;
using std::endl;

template <typename T>
auto begin(T x) -> decltype(x.begin()) {
	return x.begin();
}

template <int num, typename T>
T* begin(T array[num]) {
	return &array[0];
}

class less {
public:
	template <typename T>
	bool operator()(T const& x, T const& y) {
		return x < y;
	}
};

template <typename T>
struct Vector {
	T* pointer;
	uint64_t length;

	Vector(void) {
		pointer = nullptr;
		length = 0;
	}

	void push_back(T const& x) {}
	void push_back(T&& x) {}

	/*
	* Why do need emplace_back (i.e. two push_back functions)?
	* Creating a value and then creating a copy of that value to put in the data
	* structure makes no sense. Why not just put it in the data structure directly?
	*
	* So, we can create a push_back function that uses move semantics/r-value
	* references. This is still bad because not all data can be moved easily.
	* For example, complex numbers cannot be moved. You'll just make a copy.
	* Still seems silly that to perform construction and then move. Why not create
	* in place?
	*
	* Easy to do using member templates. Variatic templates allows us to take
	* a variable number of arguments
	*
	* Argument forwarding problem in regards to r-value references
	* Performing std::move(args...) is a bad idea because some arguments are not
	* r-value references. So, only the r-value references should have std::move 
	* called on them while l-value references should not have std::move called on
	* them. Preserve special charateristics of passed arguments
	* std::forward creates an r-value reference to l-value reference (int& &&)
	* for r-values, it does expected behavior (int&&)
	*/
	template<typename... Args>
	void emplace_back(Args&&... args) {
//		new (pointer + length) 
			T{ std::forward<Args>(args)... };
	}

	/* Passing a data structure to functions in the C++ standard is to pass
	 * a pair of iterators: It begin, It end from [begin, end)
	 * Given an invalid argument, the compiler has an issue with the instantiation
	 * templated class not on the actual template -> compiler errors are messy,
	 * can use static_assert to try and generate nice errors
	 * 
	 * Delegating constructors supported in C++11 is Constructor(...): ___constructor___ {code}
	 *
	 */
	template <typename It>
	Vector(It begin, It end) : Vector() {
		while (begin != end) {
			this->push_back(*begin);
			++begin;
		}	
	}

	/* initializer list constructor i.e. Vector<int> list = {1, 2, 3, 4} */
	Vector(std::initializer_list<int> list) : Vector(list.begin(), list.end()) {}
};

struct Foo {
	Foo(void) { cout << "constructor0\n"; }
	Foo(int, const char*, double) { cout << "constructor1\n"; }
	Foo(double, int) { cout << "constructor2\n"; }
};

//template <typename T>
//void doit(T) {
//	cout << "T is " << typeid(T).name() << endl;
//	cout << "one\n";
//}

template <typename T>
struct is_ref {
	static constexpr bool value = false;

};

template <typename T>
struct is_ref<T&> {
	static constexpr bool value = true;
};

template <typename T>
void doit(T&&) {
	cout << "T is " << typeid(T).name();
	if (is_ref<T>::value) {
		cout << "&";
	}
	cout << endl;
}

int main(void) {
	doit(42);
	int x = 42;
	doit(x);
}


void fred(void) {
	less p;
	int array[10] = { 2, 4, 6, 8, 10, 12 };
	char s[] = "Hello World";
	Vector<int> vector(s, s + 12);

	vector.emplace_back();
	vector.emplace_back(42);

	Vector<Foo> vec3{};
	vec3.emplace_back();
	vec3.emplace_back(42, "hello", 3.14159);



	Vector<int> vec2 = { 1, 2, 3, 4, 5, 6 };

	if (p(5, 10)) {
		cout << "hello!";
	}
	 
	if (p(2.5, 3.0)) {
		cout << " world\n";
	}

	int x[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	int n = 42;
}