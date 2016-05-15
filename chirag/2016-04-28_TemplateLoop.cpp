template<typename T>
void print(T const & t) {
	cout << t << endl;
}

// This one is unnecessary since it will eventually map to the top one anyway.
template<typename T1, typename T2>
void print(T1 const & t1, T2 const & t2) {
	cout << t1 << endl;
	print<T2>(t2);
}

template<typename T, typename...Args>
void print(T const & t, Args const & ... args) {
	cout << t << endl;
	print<Args...>(args...);
}

int main(void) {
	print(10.5, 42, std::string{"hello"}, "world");
}

//////////////////////////////////

template<int index, typename...Args>
struct retrieve_index {
	using type = typename Args[index];
};

template<int index, typename T, typename...Args>
struct retrieve_index {
	using type = typename retrieve_index<index - 1, Args...>::type;
};

template<typename T, typename...Args>
struct retrieve_index<0, T, Args...> {
	using type = T;
};