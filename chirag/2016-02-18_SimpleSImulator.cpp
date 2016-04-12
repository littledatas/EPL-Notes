using PFun = void (*)(void);
using PMemFun = void (LifeForm::*)(void);

class LifeForm {
public:

};

class FunObj {
	PFun pfun;
	PMemFun pmfun;
	LifeForm & obj;
public:
	// pick one of the two
	void operator()(void) {
		(*pfun)();
	}
	void operator()(void) {
		(obj.*pmfun)();
	}
};

void doThing(void) {
	cout << "I did it\n";
}