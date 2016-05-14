#include <cstdint>
#include <vector>

template <typename Op, typename T>
void apply_op(valarray<T>& lhs, const valarray<T>& x, const valarray<T>& y, Op op = Op{}) {
	uint64_t size = std::min(x.size(), y.size());
	size = std::min(lhs.size(), size);
	for (uint64_t k = 0; k < size; ++k) {
		lhs[k] = op(x[k], y[k]);
	}
}

template <typename T>
class valarray : public std::vector<T> {
	using Same = valarray<T>;

public:
	using std::vector<T>::vector; // inherits base class constructors

	/* 
	* Constructors from vector are what we want because this class has no state
	*/

	/*
	* Mathematical vector operator=, do not want to change the size of the lhs.
	*/
	Same& operator=(Same const& rhs) {
		Same& lhs = *this;
		uint64_t size = std::min(lhs.size(), rhs.size());
		for (uint64_t k = 0; k < size; ++k) {
			lhs[k] = rhs[k];
		}
		return *this;
	}

	/* left as an exercise for the student. Is the move constructor necessary? */
	//Same& operator=(Same const&& rhs) {
	//	return operator=(rhs);
	//}

	/*
	* += operator must be a member function
	*/
	Sam& operator+=(valarray<T> const& rhs) {
		Same& lhs{ *this };
		return lhs = lhs + rhs;

	}
};

template <typename T>
valarray<T> operator+(valarray<T> const& lhs, valarray<T> const& rhs) {
	uint64_t size = std::min(lhs.size(), rhs.size());
	//valarray<T> result{ size }; // Wrong because initializer list with one int
	valarray<T> result(size);

	for (uint64_t k = 0; k < size; ++k) {
		result[k] = lhs[k] + rhs[k];
	}

	return result;
}