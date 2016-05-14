#include <cstdint>

template<typename T>
class Vector {
private:
	T* data;
	uint64_t length;
	uint64_t capacity;

public:
	Vector(void) {
		data = new T[default_capacity];
		length = 0;
		capacity = default_capacity;
	}
};

int main(void) {
	Vector<int> chirag;
}