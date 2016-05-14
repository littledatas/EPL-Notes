#include <string>
#include <iostream>
#include <cstdint>

using std::cout;
using std::endl;

class SmartString : public std::string {
public:
  using std::string::string;

  /*
  * Bad because compiler cannot inline function so lose performance so do not want to create virtual functions
  */
//  virtual char operator[](uint64_t k) const {
//	  return this->std::string::operator[](k);
//  }

  void print(std::ostream& out) const {
	  out << (std::string const&)(*this);
  }
};

template <typename S1Type, typename S2Type>
class ConcatString : public SmartString {
	/* Don't want std::string because have to create every combination of SmartString and ConcatString */
	//std::string s1;
	//std::string s2;
	S1Type const& s1;
	S2Type const& s2;
public:
	ConcatString(S1Type const& left, S2Type const& right) : s1(left), s2(right) {}
	char operator[](uint64_t k) const {
		if (k < s1.size())
			return s1[k];
		else
			return s2[k - s1.size()];
	}
};

ConcatString<SmartString, SmartString> 
operator+(SmartString const& lhs, SmartString const& rhs) {
	return ConcatString<SmartString, SmartString>{ lhs, rhs };
}
 template<typename LHSType, RHSType>
ConcatString<ConcastString<LHSType, RHSType>, SmartString> 
operator+(ConcatString<ConcastString<LHSType, RHSType> const& lhs, SmartString const& rhs) {
	return ConcatString<ConcastString<LHSType, RHSType>, SmartString>{ lhs, rhs };
}

/*
* Should the language embrace concept checking?
* What is a concept? Concept => like an interface?
* Required: [] concept and size() concept
*/
std::ostream& operator<<(std::ostream& out, SmartString& str) {
	/*
	* Requires virutal function to work which is bad
	*/
	//for (uint64_t k = 0; k < str.size(); ++k) {
	//	out << str[k];
	//}
	//return out;

	str.print(out);
	return out;
}

/* --- client code here ---*/
using string = SmartString;

int main(void) {
  string first = " Craig";
  string last = " Chase";
  string greeting = "Hello ";

  auto t1 = greeting + first;
  auto t2 = t1 + last;
  cout << t2 << endl;
}
