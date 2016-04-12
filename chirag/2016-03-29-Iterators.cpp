// catch all version of != so you don't need to put it in all your iterators
template<typename T>
bool operator!=(T const& x, T const& y) {
	return !(x == y);
}

// TODO: check what is in here
// for some reason Dr. Chase never gets this work and just writes them all himself
#include <utility>
using namespace std::rel_ops;

int main(void) {
	String s {"Hello World"};
	if(s != "Hello World") {

	}

	// iterators are a logical extension of pointers in C
	char s2[] = "Hello World";
	char *p2 = &s[0];
	// how we cannot make iterators immutable like we make pointers immutable
	char const * const_p2 = &s[0];

	// language provides a const_iterator convention as well
	// if we are using an iterator on a data structure like a tree
	//   (where you shouldn't be changing things randomly to maintain the tree property),
	//   you may only provide the const_iterator
	String::iterator p = s.begin();
	String::const_iterator constp = ((String const) s).begin();
	*p = 'J';

	auto b = s.begin();
	auto e = s.end();
	while(b != e) {
		cout << *b;
		++b;
		// this will blow up the iterator so we can keep version numbers inside the data structure
		s = String{ "gotcha" };
	}

	return 0;
}

class String {
private:
	template <typename Character>
	// 1) this is one way to get the value_type and iterator_category conventions
	// see below for 2)
	iterator_helper : std::iterator<std::random_access_iterator_tag, Character> {
	private:
		Character *ptr;
		using Same = iterator_helper<Character>;

	public:
		// 2) you could also just put the typedefs in there
		using value_type = Character;
		using iterator_category = std::random_access_iterator_tag;

		// write operator * first
		Character& operator*(void) { return ptr[-1]; }
		
		// prefix
		Same& operator++(void) {
			// the strings reverse data to take advantage of little endian, so ptr goes backward
			--ptr;
			return *this;
		}

		bool operator==(Same const & rhs) const {
			return this->ptr == rhs.ptr;
		}

		// postfix (int is hacky way of indicating this)
		Same operator++(int) {
			Same t{*this};
			operator++();
			return t;
		}

		Same operator+(int32_t k) {
			Same result{};
			result.ptr = this->ptr - k;
		}

		friend String;
	};

public:
	using iterator=iterator_helper<char>;
	using const_iterator=iterator_helper<const char>;

	iterator begin(void) {
		iterator p;
		// by default, ptr is private in the iterator, but iterator design is very tightly coupled with
		//   data structure design, so we can make it a friend (we could just make a constructor)
		p.ptr = data;
		return p;
	}

	const_iterator begin(void) const {
		const_iterator p;
		p.ptr = data;
		return p;
	}

	iterator end(void) {
		iterator p;
		p.ptr = (char *) storage;
		return p;
	}

	const_iterator end(void) const {
		const_iterator p;
		p.ptr = (char *) storage;
		return p;
	}
};

///////////////////////////////////////////////////

template<typename T>
auto begin(T x) -> decltype(x.begin()) {
	return x.begin();
}

template<int num, typename T>
T* begin(T array[num]) {
	return &array[0];
}

int main(void) {
	int x[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	int n = 42;
	auto p = begin<10>(x);
	// the next two lines won't work because the parameters to a template must be constant
	int *dyn_array = new int[n];
	auto p = begin<n>(dyn_array);
}

////////////////////////////////////////////////////
class less {
public:
	template<typename T>
	bool operator(void)(T const& x, T const& y) {
		return x < y;
	}
};

int main(void) {
	less p;
	if(p(5, 10)) {
		cout << "Hello\n";
	}
}