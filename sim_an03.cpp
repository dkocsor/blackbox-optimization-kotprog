#include <iostream>
#include "TestFunction.h"
#include <iomanip>
#include <stdlib.h>
#include <random>


#define NEIGHBOR_RANGE 0.002

using namespace std;

static TestFunction* tf = new TestFunction();

void print_arr(const double* arr){
    for (int i = 0; i < tf->d(); ++i) {
        cout << setprecision(20) << arr[i] << endl;
    }
    cout << endl;
}

void copy_arr(double* arr1, const double* arr2){
    for (int i = 0; i < tf->d(); ++i) {
        arr1[i] = arr2[i];
    }
}

void clamp_(double* arr){
    for(int i = 0;i < tf->d();i++){
        if(arr[i] < tf->getRangeMin()[i]) arr[i] = tf->getRangeMin()[i];
        if(arr[i] > tf->getRangeMax()[i]) arr[i] = tf->getRangeMax()[i];
    }
}

void normalize_(double* arr){
	double sum = 0;
	for(int i = 0;i < 22;i++){
		sum += pow(arr[i], 2);
	}
	sum = sqrt(sum);
	for(int i = 0;i < 22;i++){
		arr[i] = arr[i] / sum;
	}
}

double* rand_arr(){
	double* ret = new double[22];
	for(int i = 0;i < 22;i++){
		ret[i] = (((double) (rand() - RAND_MAX/2) ) / (RAND_MAX/2) );
	}
	return ret;
}



double* neighbor(double* arr, double* range){
    double* ret = new double[22];
    
    double* direction = rand_arr();
    	
    normalize_(direction);
    	
    	
    double len = ((double) rand() / RAND_MAX) * NEIGHBOR_RANGE;
    
    for (int i = 0; i < 22; ++i) {
    	
    	//arr + uniform rand
        ret[i] = arr[i] + len * direction[i] * range[i];
    }
    
    delete[] direction;

    clamp_(ret);

    return ret;
}


double temp(long t, long max_steps){
	if(t < 10000) return 0;
    return pow(((double)(t - 10000) / (double)max_steps), 1);
}

int main() {
    srand ( 1234);
    long steps = 1000000;

    double *min=tf->getRangeMin();
    double *max=tf->getRangeMax();

	//kezdo x0
    double *x0 = new double[tf->d()];

    for (int i=0;i<tf->d();i++) {
        x0[i]=min[i]+((double)rand()/RAND_MAX)*(max[i]-min[i]);
    }

	//range
    double* range = new double[tf->d()];

    for (int i = 0; i < tf->d(); ++i) {
        range[i] = tf->getRangeMax()[i] - tf->getRangeMin()[i];
    }


    double f_best = tf->eval(x0);
    double* best = new double[tf->d()];
    copy_arr(best, x0);
    

    for (long t = steps; t > 0; --t) {
        double* nbh = neighbor(x0, range);

        double f_nbh = tf->eval(nbh);
        double f_x0 = tf->eval(x0);

		//saving best
        if(f_x0 < f_best){
            copy_arr(best, x0);
            f_best = f_x0;
        }

        if(t % 1000 == 999) cout << f_x0 << endl;

        if(f_nbh < f_x0 || ((double)rand()/RAND_MAX) < temp(t, steps)){
            delete[] x0;
            x0 = nbh;
        } else{
            delete[] nbh;
        }
    }

    cout << "----------------" << endl;
    cout << setprecision(20) << f_best << ":" << endl << endl;
    print_arr(best);


	delete[] x0;
	delete[] range;
	delete[] best;


    return 0;
}
