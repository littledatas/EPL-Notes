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

	String(String const& that) { copy(that); }
	~String(void) { destroy(); }
	String& operator=(String const& rhs) {
		if(this != &rhs) {
			destroy();
			copy(rhs);
		}
		return *this;
	}

	String& operator=(String&& rhs) {
		/* one way of doing this
		destroy();
		this->data = rhs.data;
		rhs.data = nullptr;
		*/
		// swaps data so that when rhs gets deleted, it destroys your data instead
		std::swap(this->data, rhs.data);
		this->length = rhs.length;
	}

	// rvalue reference exists when a temporary variable is used in an expression, such as in the main function below
	/* Implements the move semantics using String&& */
	String(String&& tmp) {
		/* need to explicitly cast back to String&& because tmp comes in as a String&&, but as soon as you get in here you can use it as an lvalue
		       because you can do something like tmp.data = nullptr
		   as soon as it can be used as an lvalue, it loses it's rvalue reference property. you need to cast it back to its original type
		*/
		this->move((String&&)tmp);
		/* another way of writing it is as follows
		   std::move literally just does the typecast

		   this->move(std::move(tmp));
		*/
		/* another way of doing this is to change move to accept String&
		   only reason Dr. Chase could come up with was to make it very explicit that move is only working on rvalue references
		*/
	}

	uint32_t size(void) const {
		return this->length;
	}

	char & operator[](uint32_t k) { return data[k]; }
	char operator[](uint32_t k) const { return data[k]; }

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

private:
	void copy(String const& that) {
		this->length = that.length;
		if(length == 0) {
			data = nullptr;
			return;
		}

		data = new char[length];
		for(uint32_t k = 0; k < length; k += 1) {
			data[k] = that.data[k];
		}
	}

	void destroy(void) {
		// best to check if data is null like this instead of data != nullptr, because smart pointers work better this way
		if(data) {
			// delete[] already checks if data is null, but better safe than sorry
			delete[] data;
		}
	}

	void move(String&& tmp) {
		std::cout << "shallow copy would be OK\n";
		this->data = tmp.data;
		this->length = tmp.length;
		tmp.data = nullptr;
	}
};

inline
String operator+(String const& left, String const& right) {
		String res {left};
		res += right;
		return res;
}

void doit(String const& param)
{
	cout << param[1];
}

int main(void) {
	String s1 { "Hello World" };

	// will call non const function
	s1.operator[](0) = 'J';
	s1[1] = 'a';

	/* "!" in C++ is a char *, but we didn't write an operator+(char *).
	    We do have a String constructor with char *, so the compiler will automatically promote "!" to a String.
	    Exactly the same as String s2 = s1 + String{"!"};

	    TODO: try this out with an integer object

	    The + operator creates a res on the heap, and then the = calls the copy constructor, which essentially duplicates and destroys res unnecessarily
	    In this situation, a shallow copy of res would have been fine
	    C++11 introduces rvalue reference to support move semantics

	    What's weird is that the destructor on the temporary variable is still called, since move semantics don't change that aspect. program will crash because of
	        double delete in s2.data. Need to make it so that the destructor on tmp is harmless, so set it to null in the rvalue reference!
	*/
	String s2{s1 + "!"};

	// TODO: this should be using the operator= move semantics, but for some reason visual studio still used the rvalue reference constructor.
	// works in gcc
	s2 = s2 + " cool beans.";

	for(uint32_t k = 0; k < s1.size(); k += 1) {
		cout << s1[k];
	}
	cout << endl;
}