template <typename T>
struct is_ref {
	static constexpr bool value = false;
};

template <typename T>
struct is_ref<T&> {
	static constexpr bool value = true;
};