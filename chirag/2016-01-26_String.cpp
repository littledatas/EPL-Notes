/*
3 things happen when you declare methods
	1. function is renamed to namespace of the class
	2. method is allowed to access private members of class
	3. implicit argument is added called 'this'
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

	~String(void) {
		/* need to use delete[] because it is an array. actually, using just delete works the same as delete[] on POD
		   the key difference between delete and delete[] is whether one or multiple destructors are called
		   when creating an array, if the object is not POD, the size of the array is appended to the beginning of the array and the pointer is fudged by the compiler
		     if the object is POD, the compiler knows it does not need to run a destructor, so it does not record the size of the array
		   using delete on a non-POD array means it will try deallocating using the incorrect pointer (the size of the array), and will most likely crash
		*/
		delete[] data;
		/* setting fields in destructor is silly because object is going away
		could be useful for debugging, but a better technique is to have conditional breakpoint in the destructor
		data = nullptr;
		length = 0;
		*/
		// 'delete this' in a destructor is stupid because 1) you don't know if this is on the heap and 2) it will just call the destructor, causing infinite looping
	}

	// const after the name of the function means the implicit parameter is const
	uint32_t size(void) const {
		/* NOTE: this is a parameter with the following effective declaration
			String const * this
		*/
		return this->length;
	}

	/* the first function returns an lvalue reference to the array
	   the second one is called if the string is declared as const String
	   compiler always favors the non-const one unless it must

	   functions can be overloaded if they have different arguments. if arguments are same and return type is different, it will not work?

	   operator[] and operator= must be member functions; other operator functions can be anywhere
	*/
	char & operator[](uint32_t k) { return data[k]; }
	char operator[](uint32_t k) const { return data[k]; }

	// Dr. Chase usually writes operator+= instead of operator+ because it is easier to use += in + than the other way around
	String& operator+=(String const& rhs) {
		if(rhs.size() == 0) { return *this; }

		char const *old_data = data;
		uint32_t old_length = length;

		length += rhs.length;
		data = new char[length];
		for(uint32_t k = 0; k < old_length; k += 1) {
			data[k] = old_data[k];
		}

		for(uint32_t k = 0; k < rhs.length; k += 1) {
			data[k + old_length] = rhs.data[k];	
		}

		if(old_data) { delete[] old_data; }
		return *this;
	}
};

inline 	// prevents multiple copies of the function from being created if the file is included multiple times?
String operator+(String const& left, String const& right) {
		// not efficient, because it requires 2 allocations, but it shows that + is easier once you've implemented +=
		String res {left};
		res += right;
		return res;
}

/* if param was changed to String, it would create a copy. since Stirng doesn't have a copy constructor, it will make a shallow copy with 
   two strings pointing to the same array on the heap. when doit finishes, it calls the destructor on param, which deletes the array off the heap.
   then, main will die when it tries to print s1
*/
void doit(String const& param)
{
	cout << param[1];
}

int main(void) {
	String s1 { "Hello World" };

	// will call non const function
	s1.operator[](0) = 'J';
	s1[1] = 'a';

	doit(s1);

	for(uint32_t k = 0; k < s1.size(); k += 1) {
		cout << s1[k];
	}
	cout << endl;
}