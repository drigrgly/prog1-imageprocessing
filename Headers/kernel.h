//
// Created by derig on 2021. 11. 10..
//

#ifndef IMGPROCESSING_KERNEL_H
#define IMGPROCESSING_KERNEL_H

#include <stdbool.h>
#include <math.h>

#include "ppm.h"

typedef enum KernelType {
    blur,
    veredge, //detecting vertical edges
    horedge  //detecting horizontal edges
} KernelType;

//As the Kernel is an n*n matrix
//Only create one using the kernel_set() function!!!!
typedef struct Kernel {
    int size; //In case of edge detection only 3x3 kernel is allowed (yet)
    int maxval;
    KernelType type;
    int ** grid;
} Kernel;

//produces vector with pattern such as
//1 2 1 | 1 2 4 2 1 | 1 2 4 8 4 2 1... depending on the size argument
//Should be used only with odd sizes
//The invoker is in charge of calling the free() function
int * vekt_gauss_set(int size);


Kernel * kernel_set(int size, KernelType type);

//Only works with Kernels that were created by kernel_set
void kernel_free(Kernel * kernel);

//Run the kernel operation on a pixel
//img    <- the image to do the operation on
//kernel <- the kernel to run teh operation with
//px     <- the pixels x coordinate in the img
//py     <- the pixels y coordinate in the img
double kernel_add(PPM * img, Kernel * kernel, int pxRow, int pxCol, int rgb);


//used in the blur operation
//The invoker is in charge of calling the free() function
double kernel_avg(PPM * img, Kernel * kernel, int pxRow, int pxCol, int rgb);

#endif //IMGPROCESSING_KERNEL_H
