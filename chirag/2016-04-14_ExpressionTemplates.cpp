// https://i.ytimg.com/vi/LLpIMRowndg/maxresdefault.jpg

class SmartString : public std::string {
public:
	using std::string::string;

	void print(std::ostream & out) const {
		out << (std::string const &) (*this);
	}
};

template<typename T> struct choose_ref {
	using type = T;	
};

template<> struct choose_ref<SmartString> {
	using type = SmartString const &;
};

template<typename T> using ChooseRef = typename choose_ref<T>::type;

template<typename S1Type, S2Type>
class ConcatString {
	using LeftType = ChooseRef<S1Type>;
	using RightType = ChooseRef<S2Type>;

	S1Type const & s1;
	S2Type const & s2;
public:
	// The member initialization syntax could use brackets, but some compilers don't
	//   evaluate it properly.
	ConcatString(S1Type const & lhs, S2Type const & rhs) : s1(lhs), s2(rhs) {}

	uint64_t size(void) const { return s1.size() + s2.size(); }

	char operator[](uint64_t k) const {
		if(k < s1.size()) { return s1[k]; }
		else { return s2[k - s1.size()]; }
	}

	void print(std::ostream & out) {
		s1.print(out);
		s2.print(out);
	}
};

/*
 * Using "Hello" + string will cause a problem here because there are ambiguous operator+.
 * Either the compiler can promote to SmartString using the std::string(char const *) constructor
 *   or it can use the operator+(std::string, char const *) operator defined in std::string.
 */
template<typename LHS1Type, typename RHS1Type, typename LHS2Type, typename RHS2Type>
ConcatString<ConcatString<LHS1Type, RHS1Type>, ConcatString<LHS2Type, RHS2Type>>
operator+(ConcatString<LHSType, RHSType> const & lhs, ConcatString<LHS2Type, RHS2Type> const & rhs) {
	return ConcatString<ConcatString<LHSType, RHSType>, ConcatString<LHS2Type, RHS2Type>> { lhs, rhs };
}

template<typename LHSType, typename RHSType>
ConcatString<ConcatString<LHSType, RHSType>, SmartString>
operator+(ConcatString<LHSType, RHSType> const & lhs, SmartString const & rhs) {
	return ConcatString<ConcatString<LHSType, RHSType>, SmartString> { lhs, rhs };
}

template<typename LHSType, typename RHSType>
ConcatString<SmartString, ConcatString<LHSType, RHSType>>
operator+(SmartString const & lhs, ConcatString<LHSType, RHSType> const & rhs) {
	return ConcatString<SmartString, ConcatString<LHSType, RHSType>> { lhs, rhs };
}

ConcatString<SmartString, SmartString>
operator+(SmartString const & lhs, SmartString const & rhs) {
	return ConcatString { lhs, rhs };
}

/* 
 * In Java there are interfaces that enforce a type implements a certain number of things.
 * In C++, the equivalent is called a "concept". There is currently no language construct
 * in C++ to enforce compiler checking on concepts.
 *
 * In the following code, we create a "string concept" that requires the type implements the
 *   size function and the operator[] function.
 * 
 * This operator expects a SmartString, but it should also work with ConcatString. One solution
 *   is to make ConcatString a child of SmartString and virtualize the operator[] and size() functions
 *   but then the compiler loses the ability to inline. Bad!
 */
// std::ostream & operator<<(std::ostream & out, SmartString & str) {
std::ostream & operator<<(std::ostream & out, ConcatString & str) {
	for(uint64_t k = 0; k < str.size(); k + = 1) {
		out << str[k];
	}
	return out;
}

// This operator is promiscuous because it will match with everything that
//   doesn't have a specific operator<<.
template<typename T>
std::ostream & operator<<(std::ostream & out, T const & str) {
	str.print(out);
	return out;
}

using string = SmartString;

int motivation(void) {
	string first = "Craig";
	string last = "Chase";
	string greeting = "Hello ";

	/*
	 * There is a subtle bug here with the destructors. The compiler is free to destroy first and last
	 *   whenever it wants because it is technically a temporary. This introduces an awkward situation
	 *   where the second concatenation has a reference to the first concatenation, but the compiler could
	 *   destroy the first concatenation before the evaulation point is reached. This will cause the program
	 *   to crash.
	 *
	 * We want to make copies of our proxies but not of our SmartStrings. Use a template metafunction.
	 * We assume the compiler will magically inline everything and there's no overhead to any of this.
	 */
	cout << (greeting + first + last) << endl;

	auto t1 = greeting + first;
	auto t2 = t1 + last;
	cout << t2 << endl;
}