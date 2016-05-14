#include <string>


/*
 * SmartString should also be a template and should make proxies SmartString
 * operator+ to concat strings
 * to make code simpler and not have code maintainence issue, want to do SmartString + SmartString = SmartString
 * In lecture, SmartString is referred to wrapper
 */

/*
* Make operator+ only works with our type and no other types can use our operator+
* Therefore, restrict the types that can use operator+ to only our types
*/

/* Wrapper exists simply to get the function to run */
template<typename S>
struct Wrap : public S { /* Inheritance because Wrap is equivalent to S since no state is stored in Wrap */
	using S::S;
	Wrap(S str) : S(str) {}
};

using string = Wrap<std::string>;


template<typename S1, typename S2>
ConcatString<S1, S2>
operator+(Wrap<S1> const& lhs, Wrap<S2> const& rhs) {
	S1 const& left{ lhs };
	S2 const& right{ rhs };
	ConcatString<S1, S2> {lhs, rhs};

	Wrap<ConcatString<S1, S2>> wrapped_result{ result };
	return wrapped_result;
}

#include <iostream>
int main(void) {
	string first = "Nishil ";
	string last = "Shah ";
	string greeting = "Hello ";

	std::cout << ((greeting + first) + last) << std::endl;


}