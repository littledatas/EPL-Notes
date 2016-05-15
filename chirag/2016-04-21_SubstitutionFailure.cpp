struct Blah {};

template<typename T>
struct Wrap : public T {
	using T::T;
	Wrap(T const & arg) : T(arg) {}
	using magic = int;
};

template<>
struct Wrap<Blah> : Blah {
	using Blah::Blah;
	Wrap(Blah const& arg) : Blah(arg) {}
};

template<typename T>
void foo(T const &) {
	cout << "general\n";	
}

/*
 * Substitution failure is not an error (SFINAE). When using Wrap<Blah>, typename magic doesn't exist
 *   due to the template specialization. The compiler has no idea what magic is, so it doesn't even
 *   consider this function as a candidate (it's not an error because??)
 */
template<typename T>
void foo(Wrap<T> const &, typename Wrap<T>::magic v = 0) {
	cout << "specific to wrapped stuff\n";
}

void foo(int const &) {
	cout << "specific to int\n";
}

void foo(Wrap<Blah> const &) {

}

template<typename T>
Wrap<T> wrap(T const & x) {
	return Wrap<T>(x);
}

int main(void) {
	foo(42);
	Blah b;
	foo(b);
	foo(wrap(b));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
void bracket(T const &) {
	cout << "general\n";
}

// Substitution failure is related to function overloading, so it needs to be an argument (not just
//   a thing in the body of the function).
// The problem with this is it still depends on the no-arg constructor existing for x[0]
template<typename T>
void bracket(T const &, decltype(x[0]) arg = (decltype(x[0])){}) {
	cout << "specific to types with op[]\n";
}

// Attempt #2
template<typename T, typename X = decltype(T{}[0])>
void bracket(T const & x) {
	cout << "specific to types with op[]\n";
}

// Attempt #3, cast the nullptr wtf m8
template<typename T, typename X = decltype(*((T*)0)[0])>
void bracket(T const & x) {
	cout << "specific to types with op[]\n";
}

template<typename T, typename X = decltype(std::declval<T>()[0])>
void bracket(T const & x) {
	cout << "specific to types with op[]\n";
}

// Something is wrong here, but Dr. Chase doesn't know what yet.

int main(void) {
	bracket(42);
	int x[10];
	// bracket(x);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

template<bool p, typename T>
struct enable_if {
	using type = T;
};
template<typename T> struct enable_if<false, T> {};

template<bool p, typename T>
using EnableIf = typename enable_if<p, T>::type;

template<typename T>
T fred(T const & x) { cout << "general\n"; return x; }
template<typename T>
EnableIf<true, T> fred(T const & x) { cout << "second!\n"; return x; }
