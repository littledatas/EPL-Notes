/* using the delete operator can accidentally violate type safety. for example
 T *p = new T;
 delete p;
 T2 *q = new T2; // this will allocate to the same place as p
 *p; // it's all over
*/

struct WTF {
	char const *msg;
	char const *what(void) const { return msg; }
};

/*
We have two separate blocks per object wrapped in the smart pointer. One is the object itself and another is a block to 
the reference counter. Every time a smart pointer is assigned to another smart pointer, all of the smart pointers point
to the same object and reference counter on the heap.
*/
template<typename T>
class SmartPointer {
private:
	struct ControlBlock {
		uint64_t ref_count = 0;
	};

private:
	// control block contains information about the thing that the pointer is pointing to
	// one use is to keep track of the references to the pointer (i.e. reference counting for garbage collection)
	ControlBlock *block;
	T *object;
public:
	T& operator *(void) const {
		// uniform initialization inherits C style initializers, so it automatically allows you to initialize the char * even without having a WTF constructor
		if(!block) {
			throw WTF{"dereference of uninitialized Smart Pointer"};
			return *object;
		}
	}

	// this assignment operator is dangerous and should not be used more than once
	SmartPointer<T> &operator=(T *object) {
		block = new ControlBlock{};
		this->object = object;
		block->ref_count = 1;
	}

	SmartPointer<T>& operator=(SmartPointer<T> const& rhs) {
		if(this != &rhs) {
			destroy();
			copy(rhs);
		}
		return *this;
	}

	SmartPointer<T>& operator=(SmartPointer<T> const&& rhs) {
		destroy();
		move(std::move(rhs));
		return *this;
	}

	SmartPointer(void) { block = nullptr; object = nullptr; }
	SmartPointer(SmartPointer<T> const& rhs) { copy(rhs); }
	SmartPointer(SmartPointer<T> const& & rhs) { move(std::move(rhs)); }
	~SmartPointer(void) {
		destroy();
	}

private:
	void copy(SmartPointer<T> const& rhs) {
		// copy REFERENCES, since they should all point to the same stuff (makes no sense to make a copy of the object when the user clearly didn't want to)
		object = rhs.object;
		block = rhs.block;
		block->ref_count += 1;
	}
	void destroy(void)
	{
		if(! block) { return; }
		block->ref_count -= 1;
		if(block->ref_count == 0) {
			delete object;
			delete block;
		}
	}
	void move(SmartPointer<T> && rhs) {
		object = rhs.object;
		block = rhs.block;
		rhs.block = nullptr;
	}
};

// returns a temp, so we should add move semantics to smart pointer.
// it's not too important, since smart pointers won't reallocate on a copy, but still no need to increment reference counter and then decrement it immediately after
SmartPointer<Foo_nonPOD> doit(void) {
	SmartPointer<Foo_nonPOD> p;
	p = new Foo_nonPOD{};
	return p;
}

int main(void) {
	/* This is br0ken because p isn't allocated.
	int *p;
	*p = 42;
	*/
	SmartPointer<Foo_nonPOD> p;
	p = new Foo_nonPOD{};
	p = doit();
}

/*
with something like a doubly linked list or circular linked list, there will always be at least one reference to each node
in other words, smart pointers don't work if you have cycles
c++ has something called weak smart pointers that solve the problem, but it is hard to use and it may not solve problems when writing data structures
*/
/*
one way of creating the control block right next to the object is
auto combined = new std::pair<ControlBlock, T>{ ControlBlock{}, T{}};
block = &combined->first;
object = &combined->second;

However, this calls the default constructor on T, which may or may not exist. We can add templated arguments,
but we really should make it so you can send arbitrary number of arguments
Also, we want to pass the arguments through to the object constructor completely untouched (i.e. rvalues remain rvalues and lvalues remain lvalues)
	- called perfect forwarding problem

// library implementation of smart pointer. there is also unique_ptr. the difference is that shared_ptr means every time there is an asignment, the ref count increases.
// with unique_ptr, as assignment just sets ref count to 1 always (since only one thing ever points to the object).
std::shared_ptr<Foo_nonPOD> p;
// make_shared puts the things in the same block? TODO: need to read reference on this one
p = std::make_shared<Foo_nonPOD>();
*/