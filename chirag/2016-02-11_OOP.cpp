/*
!!!!!!!!We need to consider that the pointer to the function could point to an inherited function.
	- need to adjust this pointer
	- grasp this concept, guaranteed on exam

Nested types are inherited.
*/

/*
"OOP is a programming style. There are languages that support the style and thos ethat don't. You can engage in OOP in any language (even assembly), and you can not write in traditional OOP languages.
Work with a family of related types. when you work with objects, you know little about the types and you don't care. E.g. all shapes can be draw on the screen, but who cares what the shapes are.
in non-OOP language, you have to have switch statement to determine "type" and perform appropriate thing to do. OOP handles this for you. The delegation of type-specific functions from your application code to the language
that's what makes a language OOP." - Dr. Chase's definition of OOP

in c++, correct OOP must use virtual functions.
*/

class Shape {
public:
	void draw(void) {
		cout << "generic shape\n";
	}
};

class Circle : public Shape {
public:
	int circularity; 	// stuff that makes a circle a circle
	void draw(void) {
		cout << "circle \n";
	}
};

class Square : public Shape {
	int squareness; 	// stuff that makes a square a square
	void draw(void) {
		cout << "square\n";
	}
};

void shapefun(Shape shp) {
	shp.draw();
}

int main(void) {
	Shape s;
	Circle c;
	Square r;

	// this is not OOP because you never lose track of the type you are working with
	// does exactly what you'd expect
	s.draw();
	c.draw();
	r.draw();

	// prints generic shape for all of them, because it copies into Shape
	// by default c++ methods don't support OOP by default, and must be preceded by virtual to explicitly use OOP
	shapefun(s);
	shapefun(c);
	shapefun(r);
}

///////////////////////////////////////

class Shape {
public:
	virtual void draw(void) {
		cout << "generic shape\n";
	}

	virtual int doit(void) { return 42; }
	virtual Shape& doit2(void) { return *this; }
	virtual Shape doit3(void) { return *this; }
};

class Circle : public Shape {
public:
	int circularity; 	// stuff that makes a circle a circle
	// by default, just putting virtual or leaving it out makes absolutely no difference unless override (c++11) is added
	// might as well be explicit
	// make sure you understand what override and lack of virtual means????? (see comment below for guidance)
	/* once a parent class has a function declared as virtual, all of the derived class will have an implicit virtual on the overridden functions.
	   override is an optional keyword, but adding it gives you a higher chance of catching mistakes in the compiler.
	   one example of a mistake that will compile and have incorrect results is if you make the draw function const,
	   so it doesn't match the virtual function in the parent class. if you add override then you will get a compiler
	   error in that situation.

	   how does override decide which praent function class you intended on changing?
	*/
	virtual void draw(void) override {
		cout << "circle \n";
	}

	// this will not compile because it is overriding with a different return type. to work, virtual keyword cannot be used.
	virtual double doit(void) { return 4.2; }

	// this works because a circle can be truncated to a shape if necessary
	virtual Circle& doit2(void) { return *this; }
	// this does not work because you cannot fit a Circle into a Shape
	// how come the compiler will truncate a Circle& but not a Circle?
	//    - possible answer by Dr. Chase: a reference to a circle is a reference to a shape, but a circle is not a shape. it kind of is, but we want stronger type safety.
	// 	  - try using unsigned int and unsigned short
	virtual Circle doit3(void) { return *this; }

};

class Square : public Shape {
public:
	int squareness; 	// stuff that makes a square a square
	virtual void draw(void) override {
		cout << "square\n";
	}
};

// even with virtual added to draw function, it still prints generic shape because a copy truncates. must use reference.
// see what happens if you pass by reference and don't declare the virtual
void shapefun(Shape& shp) {
	shp.draw();

	int x = shp.doit(); 	// compiler will look at this and determine whether types can be compiled or not
	Shape s = shp.doit2(); 	// whether it returns a Circle& or Shape&, it will be truncated
}

int main(void) {
	Shape s;
	Circle c;
	Square r;

	shapefun(s);
	shapefun(c);
	shapefun(r);
}

/* as soon as a virtual function is created in a class, an implicit pointer is created in the parent class that is derived in all the children
points toa virtual function table
*/