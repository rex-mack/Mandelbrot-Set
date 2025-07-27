Mandelbrot Set Project
======================
Hello my name is Rex.
Origonally, this project was for a freshman level final project, but I have always wanted to improve it.

## What it does
Prints a PPM image of the mandelbrot set.

## Use
Compile using:
 - g++ -std=c++17 -O2 -o mandelbrot main.cpp complex.cpp mandelbrot.cpp imageRenderer.cpp  

Run using:
 - ./mandelbrot

## To Do
 - Implement a move function for class MandelbrotGraph
 - Implement a zoom function for class MandelbrotGraph
 - Implement a changeImageSize for class MandelbrotGraph
 - Implement filters for class ImageRenderer
 - Implement a print "save" function for class ImageRenderer (saves images to a file and doesn't overwrite previous images)
 - Create a UI for main to move, zoom, change filters, changing image size

 ### Optimization
 - reuse values for move
 - reuse values for zoom
 - reuse values for changeImageSize
 - change double to long double or other type

## Original Project
The original program is in the folder "Origonal_Project". This was the project that I completed in school, which I will leave here as inspiration for the next version. The current images are from that project, but hopefully soon I will have the next version working!

## Images
<img src="Origonal_Project\3D_ExtraCredit.png" alt="drawing" width="400"/>
<img src="Origonal_Project\3D_ExtraCredit2.png" alt="drawing" width="400"/>

## Ideas
Create a UI that:
 - Allows click and drag to move around
 - Allows zoom in and out
 - Real time update 
 - Ajustable colors
 - Ajust precission (how many recursions)

 ## GF
 Thanks to my partner Kate for giving me support 💕

