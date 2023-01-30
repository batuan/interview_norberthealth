import numpy as np
from os import listdir
from os.path import isfile, join
import cv2

max_boundingbox_temp = [30.3061, 30.3308, 35.2618, 34.0613, 33.4172, 34.5742]
evironment_temp =      [25.1859, 25.2501, 27.8457, 23.1643, 22.4168, 21.3732]
real_temps =           [35.8000, 36.2000, 36.1000, 36.8000, 36.1000, 37.0000]


max_boundingbox_temp = np.array([max_boundingbox_temp]).T
evironment_temp = np.array([evironment_temp]).T
real_temps = np.array([real_temps]).T

y = real_temps - max_boundingbox_temp
one = np.ones((evironment_temp.shape[0], 1))
Xbar = np.concatenate((max_boundingbox_temp - evironment_temp, one), axis = 1)

# calculate true w by the function w = (X_T * X )^-1 * X_T * y
A = np.dot(Xbar.T, Xbar)
b = np.dot(Xbar.T, y)
w = np.dot(np.linalg.pinv(A), b)
print('w = {}'.format(w))

## w = [[-0.35582438]
## [ 6.46767075]]