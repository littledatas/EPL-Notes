// normal partition function for quicksort (may not necessarily work)
// partition BI only works with random access iterators
template <typename B1, typename Comp>
BI partition_bi(BI begin, BI end, Comp comp) {
	if(begin == end) { return begin; }
	// you can't do this on a singly linked list
	--end;

	BI piv_pos { end };
	BI lo { begin };
	BI hi { end };

	while(lo < hi) {
		// while(*lo < *piv_pos) is not generic
		while(lo < hi && comp(*lo, *piv_pos)) {
			++lo;
		}
		while(lo < hi && comp(*piv_pos, *hi)) {
			--hi;
		}
		// you can't do this on a doubly linked list
		if(lo < hi) {
			swap(*lo, *hi);
		}
	}

	swap(*lo, *piv_pos);
}

// see GenericProgramming2 for the version that uses just forward iterators
// the problem with the forward iterators is that there are twice as many swaps
//   what we want to do is pick the forward iterator version if we need to,
//   otherwise pick the faster backward iterator version

template <typename T>
struct less {
	bool operator()(T const& x, T const& y) const {
		return x < y;
	}
};

// rename the signatures so that overloading can be used
FI partition(FI b, FI e, Comp comp, std::forward_iterator_tag _not_used);
FI partition(FI b, FI e, Comp comp,
	std::bidirectional_iterator_tag);

// std tags look like this
struct forward_iterator_tag {};
struct bidirectional_tag : public forward_iterator_tag {};
struct random_access_tag : public bidirectional_iterator_tag {};

template <typename FI, typename Comp>
void quickSort(FI b, FI e, Comp comp) {
	if (b == e) { return; }
	// one solution is to implement iterator_category in the iterator that tells the type
	// this has the overhead of a branch, though, so STL chose a better solution
	/*switch (FI::iterator_category) {
		case Iterator::BIDIRECTIONAL: piv_pos = partition_bi(b, e, comp); break;
		case Iterator::FORWARD: piv_ps = partition_fi(b, e, comp); break;
	}*/
	// the convention is that iterator_category is a nested type in iterators
	// this is going to push tag_var on the stack if partition isn't inlined; if we expect it to be inlined, there is no stack
	// using the following line only works if FI is an actual iterator, not a primitive type
	//   FI::iterator_category tag_var{};
	iterator_traits<FI>::iterator_category tag;
	FI piv_pos = partition(b, e, comp, tag_var);
	quickSort(b, piv_pos, comp);
	++piv_pos;
	quickSort(piv_pos, e, comp);
}

template <typename T>
void showSort(T& container) {
	cout << "test for " << typeid(T).name() << endl;
	cout << "\tprior to sorting: ";
	const char* pref = "";
	for (auto const & val : container) {
		cout << pref << val;
		pref = ", ";
	}
	cout << endl;

	auto b = std::begin(container);
	auto e = std::end(container);

	// we can't use T as the type for the function object, since that is the entire container
	// decltype is the inverse of auto (only works in c++11 and further)
	using Elem = decltype(T);
	quickSort(b, e, less<Elem>{});

	cout << "\t   after sorting: ";
	pref = "";
	for (auto const & val : container) {
		cout << pref << val;
		pref = ", ";
	}
	cout << endl;
	cout << endl;
}

int main(void) {
	int x[] = { 5, 3, 9, 2, 1, 0, 4, 8, 7, 6 };
	std::vector<int> y{ std::begin(x), std::end(x) };
	std::list<int> z{ std::begin(x), std::end(x) };

	// the first one won't compile because there is on iterator_category in an array iterator (duh)
	showSort(x);
	showSort(y);
	showSort(z);
}

template <typename Iterator>
struct iterator_traits {
	/*
	the typename is necessary here, because compilers syntax check templates as much as they can even if they aren't initialized
	by default, in a typedef, the compiler doesn't know if Iterator::iterator_category is a static value or a type so it picks a value
	we are using it as a type, so we have to indicate it with the typename keyword

	if we ever have a dependent type (iterator inside an iterator using ::, we must use typename if it's a type)
	*/
	using iterator_category = typename Iterator::iterator_category;
	using value_type = using Iterator::value_type;	// this is the pre-c++11 version of using decltype to figure out the type
};

// note that iterator_traits has a <T*> in the struct definition below (in the specialization)
// TODO: this is called a full speciailization
template <typename T>
struct iterator_traits<T*> {
	using iterator_category = std::random_access_tag;
	using value_type = T;
};

// TODO: this is a partial specialization
template <>
struct iterator_traits<int *> {
	using iterator_category = std::random_access_tag;
	using value_type = int;
}