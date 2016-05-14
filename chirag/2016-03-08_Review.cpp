/*
struct layout
implicit parameter "this"

what happens when we introduce inheritance on top of these things
low level hacks
	- placement new
	- move semantics
	- copy semantics
	- allocators?

factory methods
objects changing state (wrappers)
virtual function tables
	- runtime type identification depends on vtable
	- virtual destructors
multiple inheritance
smart pointers
function objects

make sure to use {} when constructing because it will be a typo!


Q: does the rule of 3 (copy constructor, destructor, assignment) or rule of 5 (rule of 3 + move semantics) apply to abstract classes?
A: yes, if the abstract class is managing any external resource or some internal state, it must implement the rule of 3/5

good puzzle: currently Craig, when creating offspring, uses no-arg constructor of Craig. what if it ran its own copy constructor? how can we prevent that

what are the pitfalls of arrays with polymorphic types?
	- array should be of pointers (e.g. std::vector should use pointers because it cannot space elements differently, so things would get truncated)

static_cast, dynamic_cast, const_cast, reinterpret_cast


*/