//
// Created by derig on 2021. 11. 05..
//
#include <stdbool.h>
#include <ctype.h>

#ifndef IMAGEPROCESSING_SEGEDFVK_H
#define IMAGEPROCESSING_SEGEDFVK_H
double capval(double value, int min, int max);

bool isNumber(char * input);

//Creates a height * width grid ouf doubles
double ** grid2d_init(int height, int width);

//frees a 2d matrix
void grid2d_free(double *** grid, int height);


char * strinsert(char * str, int index, char * plus);

#endif //IMAGEPROCESSING_SEGEDFVK_H

