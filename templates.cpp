#include <iostream>
#include <cstdint>

using std::cout;
using std::endl;


/* 
 * Use function overloading to force compiler to determine type
 * Compiler will always choose the most specific version of the function
 * Drawbacks:
 * Runtime computation
 * Metaprogramming should allow to do computation at compile time
 * Function overloading requires at LEAST ONE ARGUMENT.
 * What happens if you just want the type information? By passing an argument, you have to run the constructor
 * Now, what if you don't even know what constructors are even available?
 */
bool isDouble_func(double) { return true; }

template <typename T>
bool isDouble_func(T const&) { return false; }

template <typename T>
void isDoubleTest_function_overloading(const T& x) {
	if (isDouble_func(x)) {
		cout << "Double" << endl;
	}
	else {
		cout << "Not Double" << endl;
	}
}

/* 
 * const = immutable 
 * constexpr = forced to be something that the compiler knows the expression
 * also, documents return types on functions
 * if arguments are compile time constants, then the return type is a compiler time constant
 */

/* Template Specialization allows for control statements like if-then-else (i.e. conditional branch) */

/*
 * Only works for the following three defined values: double, int, float
 * Conventional use would be to export a variable called "value" as opposed to "result"
 * This is all done at compile-time
 */

/* Declares that some generic template exists for isDouble, but it isn't actually defined. Compiler is happy as long as we do not use it 
 * If we run this template meta-program on any type other than double, int, or float, we will get a compile time error
*/


template <typename T>
struct isDouble;

/* Complete definition for the specialization using a double */
 template <>
 struct isDouble<double> {
	 static constexpr bool result = true;
 };

 /* Complete definition for the specialization using an int */
 template <>
 struct isDouble<int> {
	 static constexpr bool result = false;
 };

 /* Complete definition for the specialization using a float */
 template <>
 struct isDouble<float> {
	 static constexpr bool result = false;
 };

 template <typename T1>
 void isDoubleTest_template(const T1& x) {
	 if (isDouble<T1>::result) {
		 cout << "Double" << endl;
	 }
	 else {
		 cout << "Not Double" << endl;
	 }
 }


 /*
 * Function overloading cannot return a type!
 * Struct can have a nested type. The nested type can be the result of a template metaprogram.
 * 
 */
 template <bool>
 struct PickDouble;

 template<>
 struct PickDouble<false> {
	 using result = int;
 };

 template<>
 struct PickDouble<true> {
	 using result = double;
 };

 template <typename T>
 struct WhatType {
	 static constexpr bool value = IsDouble<T>::result; //typename not required because result inside of IsDouble is VALUE {true, false}
	 using result = typename PickDouble<value>::result; // typename required because result inside of PickDouble is a TYPE {int, double, float}
 };

 /* Performs a deductive analysis based on T1 */
 template <typename T1, typename T2>
 typename WhatType<T1>::result max______type_on_only_T1(const T1& x, const T2& y) {
	 if (x < y) { return y; }
	 else { return x; }
 }

 /* Code readability trick because of all the stupid use of "typename"s everywhere
 * Very difficulty to read and maintain
 * To increase readability, use template alias to abbreviate the code - gets rid of the :: and typename stupidity
 */
 template <typename T>
 using WhatType_alias = typename WhatType<T>::result;

 /* Another example to determine if a type is a reference */
 template <typename T>
 using IsReference = typename std::is_reference<T>::value;

 template <typename T1, typename T2>
WhatType_alias<T1> max______type_alias_on_only_T1(const T1& x, const T2& y) {
	  if (x < y) { return y; }
	  else { return x; }
 }

/* Now, we move on to actually writing the max function */
#include <complex>
using std::complex;

template<typename>
struct SRank;

template<>
struct SRank<int> { static constexpr int value = 1; };

template<>
struct SRank<float> { static constexpr int value = 2; };

template<>
struct SRank<double> { static constexpr int value = 3; };

template<typename T>
struct SRank <complex<T>> {
	static constexpr int value = SRank<T>::value; // Note that we can use RECURSION in templates!!!
};

template<int>
struct SType;

template<>
struct SType<1> { using type = int; };

template<>
struct SType<2> { using type = float; };

template<>
struct SType<3> { using type = double; };

template<typename T1, typename T2>
struct choose_type {
	static constexpr int t1_rank = SRank<T1>::value;
	static constexpr int t2_rank = SRank<T2>::value;
//	static constexpr int max_rank = (t1_rank > t2_rank) ? t1_rank : t2_rank;
//	static constexpr int max_rank = max(t1_rank, t2_rank); // infinite recursion
	static constexpr int max_rank = mymax(t1_rank, t2_rank);

	using max_type = typename SType<max_rank>::type;
};

template <typename T>
constexpr T mymax(T const& x, const T& y) {
	return (x > y) ? x : y;
}

/* Side note:
* If compiler does not support static variables within a template, then use
* enum {
*	rank1 = SRank<T1>::value,
*	rank2 = SRank<t2::value>,
*	max = (rank1 > rank2) ? rank1 : rank2;  // must use ? : operator here since there's no way the compiler will support the better way to do this as shown above 
* };
*/

template<typename T1, typename T2>
using ChooseType = typename choose_type<T1, T2>::max_type;

template<typename T1, typename T2>
ChooseType<T1, T2> mymax(const T1& x, const T2& y) {
	if (x < y)
		return y;
	return x;
}

//int main(void) {
//	cout << std::boolalpha;
//	auto x = mymax(5, 10);
//	cout << typeid(x).name() << " " << x << endl;
//
//	auto y = mymax(5, 10.0);
//	cout << typeid(y).name() << " " << y << endl;
//
//	auto z = mymax(5.0, 10);
//	cout << typeid(z).name() << " " << z << endl;
//}
