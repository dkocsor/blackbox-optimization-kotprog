//export LD_LIBRARY_PATH=/home/daniel/Projects/CPP/modmat_kotprog
//g++ main.cpp ./libTest.so.1

#include <iostream>
#include "TestFunction.h"
#include <iomanip>
#include <stdlib.h>
#include <random>


using namespace std;

static TestFunction* tf = new TestFunction();

//helper functions
void print_arr(const double* arr){
    for (int i = 0; i < 22; ++i) {
        cout << setprecision(20) << arr[i] << endl;
    }
    cout << endl;
}

void copy_arr(double* arr1, const double* arr2){
    for (int i = 0; i < 22; ++i) {
        arr1[i] = arr2[i];
    }
}

void clamp_(double* arr){
    for(int i = 0;i < 22;i++){
        if(arr[i] < tf->getRangeMin()[i]) arr[i] = tf->getRangeMin()[i];
        if(arr[i] > tf->getRangeMax()[i]) arr[i] = tf->getRangeMax()[i];
    }
}

double* new_v(const double* arr1, const double* arr2, const double* arr3, const double* best, double F, double lmbd){
    double* ret = new double[22];
    
    for(int i = 0;i < 22;i++){
    	ret[i] = arr1[i] + F * (arr2[i] - arr3[i]) + lmbd * (best[i] - arr1[i]);
    }

	clamp_(ret);

    return ret;
}

int* choose3(int pos_x0, int length){
	int* ret = new int[3];
	int rnd = rand() % length;
	while(rnd == pos_x0) rnd = rand() % length;
	ret[0] = rnd;
	
	rnd = rand() % length;
	while(rnd == pos_x0 || rnd == ret[0]) rnd = rand() % length;
	ret[1] = rnd;
	
	rnd = rand() % length;
	while(rnd == pos_x0 || rnd == ret[0] || rnd == ret[1]) rnd = rand() % length;
	ret[2] = rnd;
	
	return ret;
}

double* crossover(const double* x0, const double* v, double CR){
	double* ret = new double[22];
	for(int i = 0;i < 22;i++){
		double rnd = ((double) rand() / RAND_MAX);
		if(rnd < CR){
			ret[i] = x0[i];
		} else{
			ret[i] = v[i];
		}
	}
	return ret;
}


double* get_vals(double** arr, int NP){
	double* vals = new double[NP];
	for(int i = 0;i < NP;i++){
		vals[i] = tf->eval(arr[i]);
	}	
	return vals;
}

int main() {
    srand(1234);
    long num_epoch = 2000;
    int NP = 2000;
    double F = 0.6;
	double CR = 0.9;
	double lmbd = 0.8;

    double *min=tf->getRangeMin();
    double *max=tf->getRangeMax();

	//kezdo x0
    double** x0 = new double*[NP];
	for(int n_pop = 0;n_pop < NP;n_pop++){
	
		x0[n_pop] = new double[22];
		for (int i=0;i<tf->d();i++) {
        x0[n_pop][i] = min[i]+((double)rand()/RAND_MAX)*(max[i]-min[i]);

    	}
	}
	
	double* vals = get_vals(x0, NP);

	//intitial best
    double f_best = tf->eval(x0[0]);
    double* best = new double[tf->d()];
    copy_arr(best, x0[0]);
    for(int i = 1;i < NP;i++){
    	if(vals[i] < f_best){
    		copy_arr(best, x0[i]);
    		f_best = vals[i];
    	}
    }


    for (long t = 0; t < num_epoch; t++) {
    
    	if(t % 10 == 9) cout << setprecision(20) << f_best << endl;
    
		for(int i = 0;i < NP;i++){
			int* rnd = choose3(i, NP);
			
			double* v = new_v(x0[rnd[0]], x0[rnd[1]], x0[rnd[2]], best, F, lmbd);
			double* trial = crossover(x0[i], v, CR);
			
			double f_trial = tf->eval(trial);
			
			
			if(f_trial < vals[i]){
				vals[i] = f_trial;
				delete[] x0[i];
				x0[i] = trial;
			} else {
				delete[] trial;
			}
			
			if(vals[i] < f_best){
				f_best = vals[i];
				copy_arr(best, x0[i]);
			}
			
			delete[] v;
			delete[] rnd;
		}
		
    }

    cout << "----------------" << endl;
    cout << setprecision(20) << f_best << ":" << endl << endl;
    print_arr(best);


	delete[] x0; // the elements of x0 are not deleted
	delete[] best;
	delete[] vals;


    return 0;
}
