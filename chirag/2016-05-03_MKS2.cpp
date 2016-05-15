/*
 * Projects can be turned in up until Sunday. On Friday we will get all the test cases so we can look at regrades.
 * We can look at regrades ourself, or if we turn it in by Wednesday we can schedule a manual regrade.
 */

// MKS2.h

// You have to remember that every operation of MKS is in the order meters, seconds, grams
template<int ME, int SE, int GE, int NUM, int DEN>
class MKS {
	double val;
public:
};

// This is one way of hiding away ordering of the template arguments
 #include <tuple>
struct DistancePL {};
struct TimePL {};
struct MassPL {};
using MKSExponentOrder = std::tuple<DistancePL, TimePL, MassPL>
//////

// Template specialization on a variadic template can be void to indicate there are 0 variadic templates

template<typename...Terms>
struct TypeSequence {};

template<typename Seq>
struct get_length_TypeSequence;

// main.cpp
#include <iostream>
#include "MKS2.h"

 int main(void) {
 	Meters<double> x(10.0);
 	cout << "x: " << x << endl;
 }