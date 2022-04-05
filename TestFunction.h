class TestFunction
{
private:
	int dim;
	double* rangeMin;
	double* rangeMax;

public:
	TestFunction();
	double eval(double* x);
	double* getRangeMax();
	double* getRangeMin();
	int d();
};
