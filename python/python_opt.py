//export LD_LIBRARY_PATH=/home/daniel/Projects/CPP/modmat_kotprog

import ctypes
import numpy as np
import scipy
from scipy.optimize import differential_evolution, shgo, dual_annealing

lib_cpp = ctypes.CDLL('./pyhelper.so')
lib_cpp.eval.restype = ctypes.c_double

def eval(pyarr):
	seq = ctypes.c_double * len(pyarr)
	arr = seq(*pyarr)
	return lib_cpp.eval(arr)


bounds = [(-1000., 0.), 
(3., 5.), 
(0., 1.), 
(0., 1.), 
(100., 400.),
(100., 500.),
(30., 300.),
(400., 1600.),
(800., 2200.),
(0.01, 0.9),
(0.01, 0.9),
(0.01, 0.9),
(0.01, 0.9),
(0.01, 0.9),
(1.05, 6.),
(1.05, 6.),
(1.15, 6.5),
(1.7, 291.),
(-3.14159265358979, 3.14159265358979),
(-3.14159265358979, 3.14159265358979),
(-3.14159265358979, 3.14159265358979),
(-3.14159265358979, 3.14159265358979)
]

np.random.seed(1234)

res = differential_evolution(eval, bounds, disp=True, popsize=30, maxiter=1000)
#res = shgo(eval, bounds, options={'maxiter':1000, 'disp':True}) #teszteletlen
#res = dual_annealing(eval, bounds, maxiter=10000)
print(res)
print(res.x, res.fun)

