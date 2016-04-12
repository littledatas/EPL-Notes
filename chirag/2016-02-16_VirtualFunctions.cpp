/*
Virtual table is like a static member of a class, but the actual virtual table pointer (what points to the table is per class)
TODO: do static members take up space in instantiations
*/

class Base {
public:
    int x;
    
    virtual void doit(void) { cout << "Base\n"; }
	virtual void doit2(void) { cout << "Base doit2\n" };
};

class Derived : public Base {
public:
	/*
		If there are no functions overridden, the compiler will still create two separate tables even though it could use one. It is
		necessary to do so for runtime type information.
	*/
    virtual void doit(void) { cout << "Derived\n"; }
};

int main(void) {
	// size is 4 bytes, but becomes 8 bytes as soon as even one function is declared as virtual (even if there is no inheritance)
	cout < "Base is " << sizeof(Base) << " bytes";
	// implicit constructor doesn't have to initialize variables, but it DOES have to initialize the virtual function table pointer in Base
	Base b;
	// without virtual functions, this function may even be inlined
	b.doit();

	Derived d;
	d.doit();

	// this will initialize the viable for bobj after truncating d
	Base bobj2 = d;
	Base& bobj = d;
	Derived& dobj = bobj; 	// this is not true always, but is in our case because we explicitly made bobj a dobj. compiler error by default! is-a rule does not always apply here
	Derived& dobj = (Derived&) bobj; 	// smd compiler i do what i want (but this is not the best practice...)
    // TODO: Derived& dobj2 = (Derived&) b; b.doit();
	/*
	more work to type, but it's also explicit what you are casting
	fully compile time cast (basically the same as C casting shown above)
	compiler will not allow unrelated types to be converted (e.g. Shape to Circle will work, but Shape to ChocolateCake doesn't)
	*/
	Derived& dobj = static_cast<Derived&>(bobj);
    // TODO: what does this do if it's an incorrect cast?
    // TODO: can you cast actual Shapes instead of Shape&
	/*
	works like java type cast (dynamic). checks the runtime type of the object (requires some code)
	if using references, trying to cast something incorrect will throw an exception
	if using pointers, trying to cast something incorrect will create a nullptr
	determines types using the virtual function table
		if we got rid of all virtual functions (i.e. no vtable), then the bottom line will cause compiler error (static cast will still work)
	*/
    // TODO: is it valid to dynamic cast to something completely random?
	Derived& dobj2 = dynamic_cast<Derived&>(bobj);

	// as a side note, visual studio helps you out in debug mode by initializing the stack to 0xcccccccc
}

// if there are no virtual functions in Base, then even calling with a Derived will print out Base because there is no runtime type info
void whatAmI(Base *obj) {
	cout << "my type is: " << typeid(*obj).name() << endl;
}

/*
With multiple inheritance, the derived class will contain instances of both virtual tables (technically two pointers), but the compiler is smart enough
to make it act like one table

The compiler has to change the this pointer, so the virtual table entries for the same function may be different addresses. this is called a "thunk", and the purpose
of the function is to act as a trampoline that changes the pointer before calling the actual function. another implementation could just send a struct instead of just a
this pointer that contains the offset as well as the original this pointer.
*/