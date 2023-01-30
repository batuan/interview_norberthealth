Subject:

Dessiner le contour de la personne présente sur chaque image infrarouge (IR)

Bonus: Estimer la température de la personne
(valeur d'un pixel IR = 100 * température, cette dernière exprimée en Kelvin)

Informations utiles:
Le nom de chaque fichier image correspond au timestamp auquel il a été généré. Pour cela, le même nom est utilisé dans les différents dossiers ('IR'. 'screenshots', 'temperature') pour des fichiers associés à la même scène.
Les screenshots (contenus dans le dossier "screenshots") vous permettront de visualiser la correspondance RGB - IR, mais ils ne doivent pas être utilisés !
Pensez bien à spécifier le flag approprié dans la fonction cv::imread() pour lire les images IR.
Vous devez pouvoir visualiser les images IR comme dans les screenshots.
Les fichiers température ont été dénommés d'après les fichiers IR et correspondent à la température mesurée par un appareil de référence (exprimée en Celsius). 
Notez que la température donnée par l'image IR sera toujours inférieure à la température de référence.


1. Contour detection

- For the reading image, Because of the IR image use integer 16bit to store the pixel. 
So the flag to read image with openCV is IMREAD_ANYDEPTH 

Algorithm:
	1. medianBlur with kernel size = 5 of IR image
	2. thresh hold this blur image with THRESH_TOZERO, the threshold is average(IR_image) # mean of the image
	3. normalize the image in to range (0, 255) with NORM_MINMAX
	4. Canny edge detection of this image with threshold (30, 60)
	5. Find the countours of canny image
	6. Find the contour with maximum area in the image (use convexhull to transform contour to poligon)
	5. Draw the max contour

Result:
The algorithm works well with some image, but for the image with a lot of hair the algorithm not work well (because the temperature of hair is lower than the environment => threshold wrong)
Improve: using another edge detection, or find the connected component by DFS, BFS,...

Use:
	- ./findcontour -image=1 /path/to/image
	- python findcontour.py pathtofile

image=0 mean don't show the image 

ex: 
	- ./findcontour ./IR/1633613716.366127.png
	- python openimg.py 1614513421.528661.png


2. Temperature of Image

Idea: Because of the environment has an impact in the core body temperature, 
so I think there exist a linear function mapping form max temperature in the contour of person, 
environment temperature to the human core Temperature.

Function: 

human_temp - max_boundingbox_temp =  w * (max_boundingbox_temp - environment_temp) + bias

Algorithm:

1. Read IR image, find a contour of Human in this picture
2. Find a Rectangle bounding box of this contour => this Rectangle with contain all information about the temperature of human body + environment around
3. Calcualte temperature information: temperature_pixel = IR_pixel / 100 - 273.15
4. Calculate average of first 100 biggest (max_boundingbox_temp) and 100 smallest (environment_temp) temperature in the bounding box
5. Find w and bias by using this information of all images.

max_boundingbox_temp = [30.3061, 30.3308, 35.2618, 34.0613, 33.4172, 34.5742]
environment_temp      = [25.1859, 25.2501, 27.8457, 23.1643, 22.4168, 21.3732]
human_temp           = [35.8000, 36.2000, 36.1000, 36.8000, 36.1000, 37.0000]

Result: w = -0.3558 and bias = 6.4676

=> human_temp = 0.6442 * max_boundingbox_temp +  0.3558 * environment_temp + 6.4676

#############################
Some informations:

- Folder Resources contains some result images and jupyternotebook file.
- linear_mapping.py for solve linear regression problem.
- draw_img.py use matplotlib to visualize IR image.
- openCV c++ run + build with VSCode, configuration file is in .vscode folder, this project build 
with Macpro M1, clang compiler. 
For Linux, you need recompile the source code by modify the flags in build.sh.
