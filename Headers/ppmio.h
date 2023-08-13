//
// Created by derig on 2021. 11. 05..
//


#include <stdlib.h>
#include <stdio.h>
#include "ppm.h"


#ifndef IMAGEPROCESSING_PPMIO_H
#define IMAGEPROCESSING_PPMIO_H


//PPM image (vP6) I/O operations
void ppm_open(PPM * img, char *filename);
void ppm_close(PPM * img);
void ppm_fprintf(const PPM * img, char * path);


#endif //IMAGEPROCESSING_PPMIO_H

