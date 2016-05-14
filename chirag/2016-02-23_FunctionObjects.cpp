using PFun = void (*)(void);
using PMemFun = void (LifeForm::*)(void);

class FunObj {
	/* Could use union here to decide whether we are using PFun or PMemFun, but unions suck so use inheritance */
	PFun pfun;
	PMemFun pmfun;
	LifeForm& obj;

public:
	// void operator()(void) {
	// 	(*pfun)();
	// }

	void operator()(void) {
		(obj.*pmfun)();
	}
};

// ------------------------------------------

class LifeForm {
public:
	void doit(void) {
		cout << "I did it!\n";
	}
};

using PFun = void (*)(void);
template<typename T>
using PMemFun = void (T:*)(void);

// we could send arguments to operator(), but we are keeping it simple
class FunObjBase {
public:
	uint32_t refs = 0;

	virtual void operator()(void) = 0;
	virtual ~FunObjBase(void) = default;
};

class BasicFunObj : public FunObjBase {
	PFun pfun;
public:
	/* in this case pfun can be initialized in the body, since it's just a pointer. this is just to prove a point */
	BasicFunObj(PFun p) : pfun(p) {}

	void operator()(void) {
		(*pfun)();
	}
};

template<typename T>
class MemFunFunObj : public FunObjBase {
	PMemFun<T> pmfun;
	T& obj;
public:
	/* must initialize obj in this space because it happens before the body of the function. we have a reference, so if we do an assignment
	   in the body, it would overwrite what is in the reference. to initialize the actual reference to point to something, it must be here*/
	MemFunObj(T& o, PMemFun<T> f) : obj(o) {
		pmfun = f;
	}

	void operator()(void) {
		(obj.*pmfun)();
	}
};

/* essentially a smart pointer, but this is a wrapper. the difference between a wrapper and a smart pointer is that a wrapper must
   wrap every function in the object that is being wrapped so that it can pass through the call */
class Fun {
	FunObjBase * ptr;
public:
	// have every expectation that this is inlined
	void operator(void) {
		(*ptr)();
	}

	Fun(PFun f) {
		ptr = new BasicFunObj {f};
		ptr->refs = 1;
	}

	template<typename T>
	Fun(T& obj, PMemFun<T> f) {
		ptr = new MemFunFunObj<T>{obj, f};
		ptr->refs = 1;
	}

	Fun(Fun const & rhs) {
		ptr = rhs.ptr;
		ptr->refs += 1;
	}

	~Fun(void) {
		ptr->refs -= 1;
		if(ptr->refs == 0) { delete ptr; }
	}
};

void doit(void) {
	cout << "the global doit\n";
}

int simpleFunctionCall(void) {
	/* this must be a pointer, because you cannot initialize FunObjBase and you lose your polymorphic behavior.
	   but who owns the pointers? we don't want to own it because then it's overhead on our part.
	*/
	std::vector<Fun> queue;

	LifeForm bug;
	/* we want to be able to construct things of any subtype and still use Fun as a wrapper, but we also don't want to tightly couple
	   the design of the subtype with the wrapper. this is called a "factory", which is a method that constructs objects. in this case,
	   out factory is the Fun constructor */
	Fun f1 { bug, &LifeForm::doit };
	Fun f2 { &::doit }
	queue.push_back(f1);
	queue.push_back(f2);

	while(!queue.empty()) {
		/* not sure if auto is actually a good thing. auto lets the compiler decide what the variable type should be. */
		auto& fun = queue.back();
		// the syntax (*fun)() is gross, so there are two solutions: wrapper or smart pointer
		fun();
		queue.pop_back();
	}
}

class Foo {
public:
	int x = 42;
	void doit(void) {
		cout << "x is " << x << endl;
	}
};

class Silly {
	Foo& myFoo;
public:
	Silly(Foo& f) : myFoo(f) {}

	void operator()(void) {
		myFoo.doit();
		cout << "Hello world!\n";
	}
};

template<typename Fun>
void doSomething(Fun f) {
	f();
}

int main(void) {
	// this is annoying because you have to make a class just for a function, so use lambdas instead
	// this is before Foo was added to Silly
	Silly blah1;
	doSomething(blah);

	auto blah2 = [](void) { cout << "Hello World!\n"; }
	doSomething(blah2);

	// this type of lambda creates a class like the new Silly, where it adds state to the class
	Foo2 f;
	auto blah3 = [f](void) { cout << "Hello world\n"; }
	doSomething(blah3);
}

/* smart pointers are useful for internally managing things. in fact they can even (somehow) call the correct destructor if you forget your virtual destructors.
   if the client is going to be using operator overloading, wrappers are probably nicer so they don't have to put dereference operator */
