// one way to do type checking is utilize function overloading with templates
// constexpr means it will be a compile time decision
constexpr bool isDouble(double) { return true; }
template <typename T>
constexpr bool isDouble(T const &) { return false; }

// another way is to use template specialization
// since there is no definition unless double, int, or float, the others are undefined
// you can specialize a template just by declaring a template (no need to define)
template <typename T>
struct IsDouble;

template<>
struct IsDouble<double> {
	static constexpr bool result = true;
};

template<>
struct IsDouble<int> {
	static constexpr bool result = false;
};

template<>
struct IsDouble<float> {
	static constexpr bool result = false;
};

template <typename T1, typename T2>
void test(T1 const & x, T2 const & y)
{
	// this is nasty because it relies on string comparison and name(), which differs by compiler
	// if(typeid(T1).name() == "double") {
	// if(isDouble(x)) {
	if(IsDouble<T1>::result) {
		cout << "it's double\n";
	} else {
		cout << "it's not double\n";
	}
}

template<bool>
struct PickDouble;

template <>
struct PickDouble<true> {
	using result = double;
};

template <>
struct PickDouble<false> {
	using result = int;
};

template <typename T>
struct whatType {
	static constexpr bool value = IsDouble<T>::result;
	using result = typename PickDouble<value>::result;
};

// create a template alias so that you don't have to put this mess in the return type
//   of all functions that use the metaprogram
template <typename T>
using WhatType = typename whatType<T>::result;

template <typename T1, typename T2>
WhatType<T1> max(T1 const & x, T2 const & y) {
	if(x < y) { return y; }
	else { return x; }
}