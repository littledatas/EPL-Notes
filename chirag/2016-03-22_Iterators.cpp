// simple copy version of swap
template <typename T>
void swap(T& x, T& y) {
	T tmp { x };
	x = y;
	y = tmp;
}

// better version of swap (only need this one because the compiler will automatically
// convert to lvalue reference if there is no move constructor)
template <typename T>
void swap(T&& x, T&& y) {
	// T tmp{ std::move(x); }
	std::remove_reference<T>::type tmp { std::move(x) };
	x = std::move(y);
	y = std::move(tmp);
}

template<typename T>
T max(T const& x, T const& y) {
	if(x < y) { return y; }
	else { return x; }
}
/*
4 goals of STL
  1) generic with respect to element type
  2) generic with respect to data structure type
       where java and c++ begin to differ
  3) predictable time complexity
       definitely different than java
  4) comparable/competitive performance with hand-written custom code
       also definitely different than java (c++ inlines and java tends not to)
*/
template <typename T>
void sort(T * x, int n) {
	for(int k = 1; k < n; k += 1) {
		for(int j = 0; j < n - k; j += 1) {
			if(x[j] > x[j + 1]) {
				// could also do swap<int>(x[j], x[j + 1]), but the compiler can figure it out
				// when using the move version of swap, the compiler will deduce T as int& so you
				//   need the fancy remove_reference thing
				swap(x[j], x[j + 1]);
			}
		}
	}
}

/*
this is one way STL could have achieved the 4 points with the sort algorithm
take in a container and then query it to see where first element is, what the value is, etc.
STL did not choose this way
*/
template <typename Container>
void sort(Container c) {
	/* ... */
}

/*
fundamental operators on pointers
  1) dereference (operator*)
  2) check for equality (operator==)
  3) advance (operator++)
  the above are "forward iterators"
  4) retreat (operator--)
  the above are "bidirectional iterators"
  5) advanced by integer amount (operator+)
  6) calculate a distance (operator-)
  the above are "random access iterators"

c++ operators are internally built upon the previous operators
 - p != q is !(p == q)
 - p < q is p - q < 0
 - p[k] is *(p + k)
*/
// this algorithm doesn't work, but the programming style is correct
template <typename T>
void sort(T b, T e) {
	if(b == e) { return; }
	T count { b };
	// TODO: good to use preincrement if you can, because postincrement implies a copy ?
	++count;

	while(count != e) {
		T j { b };
		T jplus1 { j };
		++jplus1;
		while(jplus1 != e) {
			if(*j > *jplus1) {
				swap(*j, *jplus1);
			}
			++j;
			++jplus1;
		}
		++b;
		count++;
	}
}

void printArray(int x[]. int n) {
	const char * pref = "";
	for(int k = 0; k < n; k += 1) {
		cout << pref << x[k];
		pref = ", ";
	}
	cout << endl;
}

/*
in STL the convention is that a container is identified as the range
[b, e)
i.e. e is one AFTER the last element

one benefit of doing this is that you can check if a container is empty if b == e
another benefit is that this is basically how things are done in C
  for(int i = 0; i < n; i += 1)
  for(int i = 0; i != n; i += 1)
*/
template <typename It>
void printContainer(It b, It e) {
	const char * prefix = "";
	while (b != e) {
		cout << pref << x[k];
		pref = ", ";
		++b;
	}
	cout << endl;
}

int main(void) {
	int x[] = { 5, 3, 7, 1, 4, 8, 1, 4, 0, 7};
	int n = sizeof(x) / sizeof([0]);
	cout << "original: "; printArray(x, n);
	sort(x, n);
	sort(&x[0], &x[n]);	// should not be n - 1 because of how sort is written
	cout << "sorted: "; printArray(x, n);
	/* NOTE: on c++ convention for STL (even though it is outdated, you SHOULD know this)
	     1) iterators are passed in with begin and end functions
	     2) iterators must be nested types (not necessarily classes) in the structure
	          must support fundamental operations on pointers
	   the conventional name of the iterator for any data structure
	   (e.g. list<int>) is DS::iterator
	*/
	std::list<int> y = { 5, 3, 7, 1, 4, 8, 1, 4, 0, 7};
	std::list<int>::iterator b = y.begin();
	// we are encouraged to use auto for iterators, even though the convention is to call the nested type iterator
	auto e = y.end();
	cout << "sorted: "; printContainer(b, e);

	// for(auto p = begin(y), int val = *p; p != end(y); ++p, val = *p)
	//   begin and end functions call either y.begin() or &y[0] depending on whether it's a data structure or array (in STL)
	for(int val : y) {
		cout << val;
	}
	// better convention
	for(const auto int & val : y) {
		cout << val;
	}


	max(3, 5);	// compiler deduces int
	max(3.5, 5.5);	// compiler deduces double
	max(3.5, 5);	// compiler deduces double and int, but there is no matching max so it doesn't compile
					// compiler will not automatically cast even though 5 can definitely be promoted to double
}

//////////////////////////////////////////////////
template<typename T>
class GenComp {
	bool operator()
};

template <typename T, typename Comp>
void sort1(T b, T e) {
	Comp fun;
	if(b == e) { return; }
	T count { b };
	// TODO: good to use preincrement if you can, because postincrement implies a copy ?
	++count;

	while(count != e) {
		T j { b };
		T jplus1 { j };
		++jplus1;
		while(jplus1 != e) {
			// convention in c++ is comparison function will return true if x < y and false otherwise
			// convention in c and java is comparison function will return -1 if x < y, 0 if x == y, and 1 if x > y
			if(fun(*j, *jplus1)) {
				swap(*j, *jplus1);
			}
			++j;
			++jplus1;
		}
		++b;
		count++;
	}
}

template<typename T>
void sort2(T b, T e) {
	sort2_helper(b, e, GenComp<T>{});
}

template <typename T, typename Comp>
void sort2_helper(T b, T e, Comp _not_used) {
	Comp fun;
	if(b == e) { return; }
	T count { b };
	// TODO: good to use preincrement if you can, because postincrement implies a copy ?
	++count;

	while(count != e) {
		T j { b };
		T jplus1 { j };
		++jplus1;
		while(jplus1 != e) {
			// convention in c++ is comparison function will return true if x < y and false otherwise
			// convention in c and java is comparison function will return -1 if x < y, 0 if x == y, and 1 if x > y
			if(fun(*j, *jplus1)) {
				swap(*j, *jplus1);
			}
			++j;
			++jplus1;
		}
		++b;
		count++;
	}
}

int main(void) {
	sort1<std::list<int>::iterator, GenComp<int>>(y.begin(), y.end());
	sort2(y.begin(), y.end());
}