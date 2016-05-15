#include <iostream>
#include <cstdint>
#include <complex>

using std::cout;
using std::endl;

using std::complex;

/* Compiler will run the code and inline the result if a function is labelled as
 * a constexpr
 * isDouble is overloaded because the function argument types and/or number vary
 * 
 * Drawback is that you have to create a variable to pass it, but what if I only 
 * want to perform operations on a type. Runnning a constructor is ambiguous so
 * this solution isn't that great
 *
 * Declare a template without definining is valid as long as you don't use it. You
 * can still speciliaze it and still run it on the specialized.
 * function overloading and template specialization basically do the same thing, but
 * template specialization is more powerful and flexible
 */
constexpr bool isDouble(double) { return true;  }

template <typename T>
constexpr bool isDouble(T const&) { return false;  }

template <typename>
struct SRank;

template <> struct SRank<int> { static constexpr int value = 1;  };
template <> struct SRank<float> { static constexpr int value = 2; };
template <> struct SRank<double> { static constexpr int value = 3; };
template <typename T> struct SRank<complex<T>> {
	static constexpr int value = SRank<T>::value;
};

template <int>
struct SType;

template <> struct SType<1> { using type = int; };
template <> struct SType<2> { using type = float; };
template <> struct SType<3> { using type = double; };

template <typename T, bool is_complex>

template <typename T1, typename T2>
struct choose_type {

	static constexpr int t1_rank = SRank<T1>::value;
	static constexpr int t2_rank = SRank<T2>::value;
//	static constexpr int max_rank = (t1_rank > t2_rank) ? t1_rank : t2_rank;
	static constexpr int max_rank = bax(t1_rank, t2_rank);

	using type = typename SType<max_rank>::type;
};

template <typename T1, typename T2>
using ChooseType = typename choose_type<T1, T2>::type;

template <typename T1, typename T2>
ChooseType<T1, T2> bax(T1 const& x, T2 const& y) {
	if (x < y) { return y; }
	else { return x; }
}

template <typename T>
constexpr T bax(T const& x, T const& y) {
	return (x < y) ? y : x;
}

template <typename T>
struct is_lreference {
	static constexpr bool value = false;
};

template <typename T>
struct is_lreference<T&> {
	static constexpr bool value = true;
};

template <typename T>
struct is_rreference {
	static constexpr bool value = false;
};

template <typename T>
struct is_rreference<T&&> {
	static constexpr bool value = true;
};

template <typename T>
struct is_reference {
	static constexpr bool value = is_lreference<T>::value 
		|| is_rreference<T>::value;
};

//template <typename T>
//struct is_reference<T&&> {
//	static constexpr bool value = true;
//};

template <typename T>
void doit(T&& x) {
	if (! is_reference<T>::value) {
		cout << "can't change it!\n";
	}
	x = 42;
}

int main(void) {
	cout << std::boolalpha;
	auto x = bax(5.5, 10);
	cout << typeid(x).name() << endl;
}