//
// Created by derig on 2021. 11. 05..
//
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "ppm.h"

#ifndef IMAGEPROCESSING_PPMMOD_H
#define IMAGEPROCESSING_PPMMOD_H

//PPM image (vP6) modifier operations

//Works by adding or subtracting a constant from the image, capping at the min and maximum value of a pixel
//image to modify | percentage to brighten/darken by
void ppm_changeBr(PPM * img, double pr);

//Works by calculating a contrast correction factor
//then using that factor to modify the rgb values
//image to modify | how bright it should be from the original one
// Pl. 110% -> 10% brighter than the original one
//     90%  -> 10% darker than the original one
void ppm_changeCr(PPM * img, double pr);

void ppm_toGrayscale(PPM * img);

//Adding a gaussian blur effect to the picture
//you can control the strength of it with the pr variable
void ppm_blur(PPM * img, double pr);

//Does the same effect as ppm_blur but with a different method
void ppm_gblurWstrip(PPM *img, double pr);


void ppm_edge(PPM * img);

#endif //IMAGEPROCESSING_PPMMOD_H
