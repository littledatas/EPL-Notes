// what is the return type of max here?
template<typename T1, typename T2>
T? max(T1 const& x, T2 const& y) {
	if(x < y) { return y; }
	else { return x; }
}

template <typename T>
struct is_ref {
	static constexpr bool value = false;
};

template <typename T>
struct is_ref<T&> {
	static constexpr bool value = true;
};

template <typename T>
struct is_ref<T&&> {
	static constexpr bool value = true;
};