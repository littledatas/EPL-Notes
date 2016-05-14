template<typename>
struct SRank;

template<> struct SRank<int>    { static constexpr int value = 1; };
template<> struct SRank<float>  { static constexpr int value = 2; };
template<> struct SRank<double> { static constexpr int value = 3; };
template<typename T> struct SRank<complex<T>> {
	static constexpr int value = SRank<T>::valu	
};

template<int>
struct SType;

template<> struct SType<1> { using type = int; };
template<> struct SType<2> { using type = float; };
template<> struct SType<3> { using type = double; };

template<typename T1, typename T2>
struct choose_type {
	static constexpr int t1_rank = SRank<T1>::value;
	static constexpr int t2_rank = SRank<T2>::value;
	// The following is an example of how you could do static data members before the compiler
	//   was smart enough to understand metaprogramming
	/*enum {
		rank1 = SRank<T1>::value,
		rank2 = SRank<T2>::value,
		max = (rank1 > rank2) ? rank1 : rank2
	}*/
	// static constexpr int max_rank = (t1_rank > t2_rank) ? t1_rank : t2_rank;

	// c++ black magic: we can use a max function instead of the ? thing
	//   we don't want to use the 2 type max function though, because that depends on the template
	//   and would cause a recursive loop in the compiler
	static constexpr int max_rank = bax(t1_rank, t2_rank);

	using type = typename SType<max_rank>::type;
};

template<typename T1, typename T2>
using ChooseType = typename choose_type<T1, T2>::type;

template<typename T1, typename T2>
ChooseType<T1, T2> bax(T1 const & x, T2 const & y) {
	if(x < y) { return y; }
	else { return x; }
}

template<typename T>
constexpr T bax(T const & x, T const & y) {
	// since this is used at compile time, the compiler puts a lot of constraints on what can be here
	// some examples:
	//   1) no conditionals (should work actually...)?
	//   2) only one return statement
	return (x < y) ? y : x;
}

int main(void) {
	cout << std::boolalpha;
	auto x = bax(5.5, 10);
	cout << typeid(x).name() << endl;	// prints out double
}