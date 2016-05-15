#include <iostream>
#include <cstdint>

/* Iterator Goals:
	1. generic container
	2. generic with respect to element type
	3. predictable time complexity
	4. performance should be comparable to custom, hand-written code
			- expectations of inline and optimizations
			- Problem: A sorting function for a linked-list, doubly linked-list, and an 
						array all have different optimizations that can be made. How do 
						we write multiple implmentations and decide between them?
				* We cannot write only one implmentation because we lose optimizations!
				Solution 1: if-else/swtich case that decides where to turn on optimizations
						if (FI::iterator_category == Iterator::FORWARD) {
							//use optimized version here
						} else {
							//do not use optimized version here
						}
					Issues:
						1. Code is not very generic, deciding between multiple types of
							iterators is messy
						2. I'd like to know at compile time if code will work
				
				Solution 2: Tags
					FI partition(FI b, FI e, Comp comp, std::forward_iterator_tag _unused) {
						...
					}
					Notes:
						1. Added tags to allow compiler to differentiate between various 
							functions because function signatures are different!
						
						2. How do we get these types? 
						
							We store them in the iterator class
							themself. For example, forward_iterator class will have a 
							using iterator_category = std::forward_iterator_tag;
						
						3. How do we do add this functionality to pointers? We can't
							directly. 
							
							We introduce a level of indirection through a 
							container called iterator_traits that stores the iterator 
							information like what kind of data it is pointing to and
							what type of iterator is it describing. We then specialize 
							this container for pointers (because we can't actually
							modify what a pointer is.). Gives us backward compatibility
							with primitive data types!

						4. value_type in iterator_traits is introduced so we know what
							type of data we are !**pointing to**!

						Partial and full template specialization
						Full: 
							template <typename T> 
							struct Foo {}
						
						Partial:
							template <>
							struct Foo<int> {}

							template <typename T>
							struct Foo<T*> {}
 */


/*
 * I need you to know iterator_traits and why it exists.
 * NOTE that the standard iterator_traits includes several nested components
 * (e.g., pointer_type, reference_type and a few others)
 * I ONLY care about iterator_category and value_type
 */
template <typename Iterator>
struct iterator_traits {
	using iterator_category =
		typename Iterator::iterator_category;
	using value_type =
		typename Iterator::value_type;
};

template <typename T>
struct iterator_traits<T*> {
	using iterator_category = std::random_access_iterator_tag;
	using value_type = T;
};

template <>
struct iterator_traits<int*> {
	using iterator_category = std::random_access_iterator_tag;
	using value_type = int;
};


/* 
 * The partition function is used by the quicksort algorithm 
 * the purpose of the function is to move larger values towards
 * the end of the container and smaller values towards the front
 *
 * In this version of partition, FI must satisfy the forward iterator concept
 * that means FI::operator++, FI::operator*, FI::operator== must exist and
 * have O(1) time complexity and provide the expected semantics
 * comp must be a function object. The function must 
 * provide a strict weak ordering for elements stored in the container
 * i.e., if x and y are elements in the container then exactly one of the following must hold
 *   either {
          comp(x, y) is true -- signifying x is less than y
		  comp(y, x) is true -- signifying y is less than x
		  neither comp(x, y) nor comp(y, x) is true -- signifying x is equivalent to y
 *
 *
 * NOTE: the convention in writing generic code is to pass the iterators by value, not by
 * constant reference. In the parameters below [b, e) is a container (or sub-range of a container)
 * and comp is a comparison function object as described above. Note that in most cases, comp
 * is state-less, and the only real information in contained in the type Comp. However, it's 
 * possible that comp is an actual pointer to a function (yuck -- no inlining).
 */

struct forward_iterator_tag {};
struct bidrectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidrectional_iterator_tag {};

template <typename FI, typename Comp>
FI partition(FI b, FI e, Comp comp, std::forward_iterator_tag _notused) {
	/* this implementation assumes only forward iteration
	 * to keep the implementation simple, I will use *b as the pivot value */

	 /* loop invariant: the range [b, piv_pos) consists of elements strictly less than the pivot
	  * the range [piv_pos, next) is not empty and consists of elements not less than the pivot
	  * the pivot value is *piv_pos 
	  * thus, when next == e, we're done with partition */

	if (b == e) { return b; } // probably invalid case, not sure the returned value is meaningful
	cout << "forward\n";

//	using I = std::remove_reference<FI>::type; // our iterator type
	FI piv_pos{ b };
	FI next{ b };
	++next; 

	/* invariant established [b, piv_pos) is empty (contains no elements)
	 * [piv_pos, next) contains one element (the pivot),
	 *              and that element is not less than the piv value ('cause it IS the piv value)
	 * (note that the piv value is always at *piv_pos) */
	while (!(next == e)) {
		if (comp(*next, *piv_pos)) { // *next is a small value, needs to be moved to the front
			std::swap(*next, *piv_pos); // small value moved to the front (it's now the last of the smalls)
			++piv_pos; // number of small values increases by one: [b, piv_pos) increased by one elem.
			std::swap(*next, *piv_pos); // put pivot value back where it belongs at *piv_pos
		}
		/* at this point [b, piv_pos) is less than *piv_pos
		 * the pivot value is stored at *piv_pos
		 * all values in the range [piv_pos, next] are not less than piv_value
		 * NOTE: [piv_pos, next) may be empty, so increment next to restore invariant and
		 * make progress towards our goal */
		++next;
	}

	return piv_pos;
}

template <typename BI, typename Comp>
BI partition(BI begin, BI end, Comp comp, 
std::random_access_iterator_tag _notused) {
	/* invariant: [b, lo) are all strictly less than pivot
	* [hi, e) are all not less than pivot
	* *piv_pos is the pivot value (we put piv_pos at the end
	* for convenience) */

	if (begin == end) { return begin; } // ill defined
	--end;
	cout << "bidirectional\n";

	/* I'm being lazy here, we'll use the last element as pivot value */
	BI piv_pos{ end };
	BI lo{ begin };
	BI hi{ end };

	using T = typename iterator_traits<BI>::value_type;
	T piv = *piv_pos;

	while (lo != hi) {
		while (lo != hi && comp(*lo, *piv_pos)) {
			++lo;
		}
		while (lo != hi && comp(*piv_pos, *hi)) {
			--hi;
		}
		if (lo != hi) {
			std::swap(*lo, *hi);
		}
	}

	std::swap(*lo, *piv_pos);
	return lo;
}


/* see comments for partition regarding the requiremetns for FI and Comp */
template <typename FI, typename Comp>
void quickSort(FI b, FI e, Comp comp) {
	if (b == e) { return; } // empty container, already sorted

	typename iterator_traits<FI>::iterator_category tag_var{};
	FI piv_pos = partition(b, e, comp, tag_var);

	quickSort(b, piv_pos, comp);
	++piv_pos;
	quickSort(piv_pos, e, comp);
}


