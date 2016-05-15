/*
 * ME: meter exponent
 * SE: second exponent
 * GE: gram exponent
 * NUM: numerator type (when dividing)
 * DEN: denominator type (when dividing)
 *
 * Look at the examples/recording.
 */
template<typename T, int ME, int SE, int GE, int NUM, int DEN>
class MKS {
	T value;
public:
	using Same = MKS<T, ME, SE, GE, NUM, DEN>;
	static char unit_name[8];

	//explicit MKS(double )

	Same & operator=(const Same & that) {
		value = that.value;
		return *this;
	}

	Same operator+()

	template<int NUM2, int DEN2>
	operator
};

int main(void) {
	Meter<double> x(10.0);
	cout << "x: " << x << endl;
}