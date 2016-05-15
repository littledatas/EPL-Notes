// TODO: watch the video lecture for this one

#include <iostream>
#include <cstdint>
#include <string>

using std::declval;

struct TRUE {
	static constexpr bool value = true;
};

struct FALSE {
	static constexpr bool value = false;
};

// Let's say you try has_op_bracket<int>(). The compiler will try matching with this function
//   and then realize it can't use the default value for the test argument. It throws away
//   this function as a candidate.
template<typename T, typename test = decltype(declval<T>()[0])>
TRUE has_op_bracket(void) { return TRUE{}; }

// As it stands right now, when trying has_op_bracket<int*>(), there are 2 candidates that look
//   exactly the same and there is a compiler error on multiple definitions.
// template<typename T>
// FALSE has_op_bracket(void) { return FALSE{}; }
// If you use a variadic template, the compiler will deduce any empty set, which is apparently
//   different than void. The variadic template one is less specific, so the compiler uses the
//   other one if it can.
template<typename T, typename...not_used>
FALSE has_op_bracket(not_used...) { return FALSE{}; }

template<typename T>
using HasBracket = decltype(has_op_bracket<T>());

template<typename T>
void check_bracket(void) {
	cout << std::boolalpha;
	cout << HasBracket<T>::value << endl;
}

struct Hack {
	// When you put in 2 foo functions, &foo is ambiguous so the TRUE has_foo is a substitution failure
	//   and it selects the FALSE one.
	void foo(int) {}
	void foo(void) {}
};

// TODO: try using decltype(declval<T>().foo(...)) as a variadic template to match against all overloads of foo.
template<typename T, typename test = decltype(&T::foo)>
TRUE has_foo(void) { return TRUE{}; }

template<typename T, typename...not_used>
FALSE has_foo(not_used...) { return FALSE{}; }

int main(void) {
	check_bracket<int>();
	check_bracket<int*>();
	check_bracket<std::string>();

	cout << has_foo<Hack>().value << endl;
}

///////////////////////////////////////////////////////

template<typename... T>
struct Tuple;

// This is the base case when there is exactly one type left. You could also make this specialization a void
//   to support empty tuples.
template<typename T>
struct Tuple<T> {
	T val;
};

// When doing Tuple<OtherArgs...>, it matches with this struct again and automatically extracts the first thing
//  as T. This happens until there is only one thing in OtherArgs, at which point it matches with the single template
//  arg version.
template<typename T, typename... OtherArgs>
struct Tuple<T, OtherArgs...> : public Tuple<OtherArgs...> {
	T val;
};

// This piece of magic allows you to write arbitrary (with some constraints) recursive functions in the compiler.
// In particular, this computes the factorial.
template<int index, typename T>
struct loop {
	static constexpr int value = index * loop<index - 1, T>::value;
};

template<typename T> struct loop<0, T> {
	static constexpr int value = 1;
};

