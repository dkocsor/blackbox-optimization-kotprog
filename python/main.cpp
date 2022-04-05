//export LD_LIBRARY_PATH=/home/daniel/Projects/CPP/modmat_test
//g++ -shared -o helper.so -fPIC main.cpp ./libTest.so.1 


#include "TestFunction.h"
#include <iostream>
#include <iomanip>
#include <stdlib.h>

using namespace std;


static TestFunction* tf = new TestFunction();

extern "C" {

double eval(double* arr){
	//printf("c: %lf\n", tf->eval(arr));
	return tf->eval(arr);
}


}
