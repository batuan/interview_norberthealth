import cv2
import matplotlib.pyplot as plt
import sys

img_path = sys.argv[1]

img = cv2.imread(img_path, cv2.IMREAD_ANYDEPTH)
plt.imshow(img/100-273.15)
plt.show()