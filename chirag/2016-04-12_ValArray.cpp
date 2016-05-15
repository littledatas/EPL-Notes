#include <iostream>
#include <cstdint>
#include <vector>
#include <algorithm>

using std::cout;
using std::endl;

#define valarray valarray1

// T can be figured out by the compiler, but Op will need to be passed in
// Templates that can be deduced should be at the end of the list
template<typename Op, typename T>
void apply_op(valarray<T> & lhs, valarray<T> const & x, valarray<T> const & y, Op op = Op {}) {
	uint64_t = std::min(x.size(), y.size());
	size = std::min(size, lhs.size());
	for(uint64_t k = 0; k < size; k += 1) {
		lhs = op(x[k], y[k]);
	}
}

// valarray is an interface wrapper. It changes some functionality of vector, but it
//   doesn't add any state
template<typename T>
class valarray : public std::vector<T> {
	using Same = valarray<T>;	// type alias to the same type (Dr. Chase preferred)
public:
	// inherit the constructors from std::vector<T>
	using std::vector<T>::vector;

	Same & operator=(Same const & rhs) {
		Same & lhs { *this };
		uint64_t size = std::min(lhs.size(), rhs.size());
		for(uint64_t k = 0; k < size; k += 1) {
			lhs[k] = rhs[k];
		}
		return lhs;
	}

	// TODO: may not be correct
	//   1. Do I need to change the move semantics to get rid of the move constructor?
	//   2. Is the syntax correct?
	Same & operator=(Same && rhs) {
		return operator=(rhs);
	}

	// This has the overhead of calling the operator+ function call, but it demonstrates code reuse
	// The other alternative is 
	Same & operator+=(Same const & rhs) {
		Same &lhs { *this };
		// Chase hates this but the return type exists so you might as well use it
		// return lhs = lhs + rhs;
		apply_op<std::plus<T>>(lhs, lhs, rhs);
		return lhs;
	}
};

template<typename T>
valarray<T> operator+(valarray<T> const & lhs, valarray<T> const & rhs) {
	// You could templatize away these first 2 lines as well, but Chase didn't want to in this example
	uint64_t size = std::min(lhs.size(), rhs.size());
	valarray<T> result(size);
	// The syntax here is weird because std::plus just has <> without an actual type
	apply_op<std::plus<>>(result, lhs, rhs);
	return result;
}

template<typename T>
valarray<T> operator-(valarray<T> const & lhs, valarray<T> const & rhs) {
	uint64_t size = std::min(lhs.size(), rhs.size());
	valarray<T> result(size);
	apply_op<std::minus<>>(result, lhs, rhs);
	return result;
}

template<typename T>
std::ostream & operator<<(std::ostream & out, const valarray<T> & vec) {
	const char * pref = "";
	for(const auto & val : vec) {
		out << pref << val;
		pref = ", ";
	}
	return out;
}

int main1(void) {
	valarray<int> x { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	valarray<int> y { 2, 4, 6, 8, 10 };
	cout << "x: " << x << endl;
	cout << "y: " << y << endl;
	cout << "x + y: " << (x + y) << endl;
	x += y;
	cout << "x: " << x << endl;
}

// This was the original C++ plus, but then people got mad that you had to do std::plus<
template<typename T>
struct plus {
	constexpr T operator()(T const & x, T const & y) const {
		return x + y;
	}
};

int main3(void) {
	std::plus<int> add_op;
	cout << add_op(5, 7) << endl;
	cout << add_op(3.5, 2) << endl;

	std::plus<void> gen_add;
	cout << gen_add(5, 7) << endl;
	cout << gen_add(3.5, 2) << endl;
	cout << gen_add(2, 3.5) << endl;
}

/*
 * A problem arises with w = x + y + z; because the compiler sticks in (x + y) + z, which means x + y _must_ be computed before the + z can be done.
 * Our solution to this is to not do the x + y until its result is used (i.e. lazy evaluation). We can achieve this by returning a note in the operators
 * that indicates they have not been completed yet, but should be when an evaluation point (operator=, operator<<, etc.) is reached.
 */