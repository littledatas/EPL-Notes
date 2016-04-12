// this will make a sane c++ program cri 
void qsort(void * data, int num_elems, int size_elem, int (*pfun)(void *, void *)) {
	int result_value = (*pfun)(nullptr, nullptr);
}

int compare(void * x, void * y) {
	double x_val = *((double *) &x);
	double y_val = *((double *) &y);
	return (x_val < y_val) ? -1 : (x_val == y_val ? 0 : 1);
}

void myApp(void) {
	qsort(&data, 500, 8, &compare);
}
// ------------------------------------------

// one advantage of a function object is that the compiler can try to inline the function
class FunObject {
public:
	int operator()(double x, double y) {
		if(x < y) { return -1; }
		if(y < x) { return 1; }
		return 0;
	}
};

template <typename SomeFun>
void qsort(double x[], int n, SomeFun) {
	if(SomeFun{}(x[0], x[1]) < 0) {
		cout << "whoopie!\n";
	}
}

void myApp(void) {
	FunObject fun;
	double x[1000];
	int n = 1000;
	qsort(x, n, fun);
}