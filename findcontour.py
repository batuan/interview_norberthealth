import cv2
import numpy as np
import random as rng
import sys
import matplotlib.pyplot as plt

def read_image(file):
    img = cv2.imread(file, cv2.IMREAD_ANYDEPTH)
    thresh = cv2.medianBlur(img, 5)
    thresh_hold = np.mean(thresh)
    ret, thresh = cv2.threshold(src=thresh, thresh=thresh_hold, maxval=0, type=cv2.THRESH_TOZERO)
    normed = cv2.normalize(thresh, None, 0, 255, cv2.NORM_MINMAX, cv2.CV_8U)
    out_canny = cv2.Canny(normed, 30, 60)
    return out_canny, img


def find_contour(image):
    contours, hierarchy = cv2.findContours(image=image, mode=cv2.RETR_TREE, method=cv2.CHAIN_APPROX_NONE)
    max_val = -1
    max_are = -1
    # print(len(contours))
    for i in range(len(contours)):
        area = cv2.contourArea(cv2.convexHull(contours[i]))
        # print(area)
        if area > max_are:
            max_val = i
            max_are = area
    return contours, max_val, hierarchy

img_path = sys.argv[1]
print(img_path)

img = cv2.imread(img_path, cv2.IMREAD_ANYDEPTH)
if img is None:
    print('not found img')
    exit()

canny, ori = read_image(img_path)
contours, max_val, hierarchy = find_contour(canny)

normed = cv2.normalize(ori, None, 0, 255, 32, cv2.CV_8U)
normed_rgb = cv2.cvtColor(normed, cv2.COLOR_GRAY2BGR)

cv2.drawContours(normed_rgb, contours, max_val, (0, 255, 0), 2, cv2.LINE_8, hierarchy, 0)
fig = plt.figure(figsize=(12, 8))
fig.add_subplot(1,2,1)
plt.imshow(ori, cmap="hot");
plt.title("origin image")
fig.add_subplot(1,2,2)
plt.imshow(normed_rgb)
plt.title("find contour image")
plt.show()

# cv2.imshow("contour", cv2.resize(normed_rgb, dsize=(640, 480), interpolation=cv2.INTER_LINEAR))
# cv2.waitKey(0)
# cv2.destroyAllWindows()
