// Interesting question about this case study. There is a lot of overhead of
//   storing small things on the heap, so 

class Thing {
	ThingHelperInterface * obj;
public:
	template <typename T>
	Thing& operator=(T const & obj) {
		delete ptr;
		ptr = new ThingHelper<T> { obj };
		return *this;
	}

	// The problem here is that at this point you've lost the original T that you did the
	//   assignment with. The solution is to make a non-templatized interface that has a
	//   virtual function table pointer to a copy of the element on the heap (in a templatized)
	//   helper object).
	void print(std::ostream & out) {
		ptr->print(out);
	}

	// ur a wizard, craig
	template <typename T>
	void operator T(void) const {
		ThingHelper<T> * cast_ptr = dynamic_cast<ThingHelper<T> *>(ptr);
		if(cast_ptr) {
			return (T) cast_ptr->obj;
		} else {
			return T{};
		}
	}

	~Thing(void) {
		delete ptr;
	}
};

class ThingHelperInterface {
public:
	virtual void print(std::ostream &) const = 0;
	virtual ~ThingHelperInterface(void) = default;
};

template <typename T>
class ThingHelper : public ThingHelperInterface {
	T obj;
public:

	virtual void print(std::ostream & out) const {
		out << obj;
	}
};

std::ostream & operator<<(std::ostream & out, Thing const & thing) {
	thing.print(out);
	return out;
}

int main(void) {
	Thing x;
	x = 42;
	cout << x << endl;
	cout << x << endl;

	x = ObservableString { "Hello World" };
	cout << "Ok, we've assigned a string now...\n";
	cout << x << endl;
}