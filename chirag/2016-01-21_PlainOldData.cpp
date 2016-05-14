/*
SVN setup at https://lennon.ece.utexas.edu/ee380l.5/sp2016

In C you cannot create structs on the fly. Example:
*/
Complex foo(void) {
	double real_val = 32;
	double imag_val = 10;
	return { real_val, imag_val }; // cannot do this in C; added with "uniform initialization" in C++11
}
/*
Uniform initialization only works if members are public; works in the order that the data is laid out
Can use default constructor/destructor by doing Complex(void) = default, which differs from Complex(void) {}
Copy constructor: Complex(Complex const & that) {}
	- Better convention to put "const" after the type - makes no difference
*/
void operator=(Complex const & rhs) {
	this->~Complex();
	new (this) Complex { rhs }; // placement new syntax; tells 'new' where to allocate space
}
/*
Unary * (dereference) and = are right-associative

Need to protect against idiots doing something like x = x;
	- If you delete the rhs in the copy, you deleted the original thing you sourced from
Thus, default constructor is like this:
*/
Complex& operator=(Complex const & rhs) {
	if(this != &rhs) {
		this->real = rhs.real;
		this->imag = rhs.imag;
	}
	return *this;
}

/*
In-progress string implementation
*/

#include <iostream>
#include <cstdint>

using std::cout;
using std::endl;

class String {
	char *data;
	uint32_t length;

public:
	String(void) {
		data = nullptr;
		length = 0;
	}

	String(char const * src) {
		length = 0;
		while(src[length] != 0) {
			length += 1;
		}
		if(length > 0) {
			data = new char[length];
			for(int k = 0; k < length; k++) {
				data[k] = src[k];
			}
		} else {
			data = nullptr;
		}
	}

	~String(void) { delete[] data; }
};