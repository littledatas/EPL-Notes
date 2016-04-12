void doit(Foo const & x) {
	cout << "lvalue ref\n";
}

void doit(Foo const && x) {
	cout << "rvalue fun\n";
}

int main(void) {
	Foo f;
	Foo && ref = (Foo &&) f;
	Foo const&& cref = (Foo const &&) f;

	/* all of these print lvalue ref because you have given a name to the rvalue references,
	   thus no longer making them temporaries, even though the name is declared as rvalue. */
	doit(f);
	doit(ref);
	doit(cref);

	// first one calls lvalue, second two call rvalue
	doit(f);
	doit(std::move(ref));
	doit(std::move(cref));
}

// let's say we changed the second doit to remove const
// makes sense, since move function will destroy original
void doit(Foo && x) {

}

int main(void) {
	Foo f;
	Foo const&& cref = (Foo const&&) f;

	cout << typeid(f).name() << endl;

	/* calls lvalue reference because there is no const rvalue function
	   makes sense because there are two options 1) make type less restrictive by converting to lvalue reference
	   2) remove const modifier and allow function to potentially change object
	   option 1) makes more sense, because you _never_ want a const to become a non-const
	*/
	doit(std::move(cref));
}