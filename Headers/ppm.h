//
// Created by derig on 2021. 11. 05..
//

#ifndef IMGPROCESSING_PPM_H
#define IMGPROCESSING_PPM_H

typedef struct PPM {
    char * path;
    char * mnumber;
    int width;
    int height;
    int maxval;
    size_t dataSize;
    unsigned char *** data ; //2d: ima ge resolution 3rd: rgb values (0: red, 1: green, 2: blue)
} PPM;


#endif //IMGPROCESSING_PPM_H
