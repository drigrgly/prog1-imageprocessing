//
// Created by derig on 2021. 11. 10..
//

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "kernel.h"
#include "segedfvk.h"
#include "debugmalloc.h"

//Convolves an n wide and an n tall vector -> creates an n*n matrix
//Only visible in kernel.c
int ** vekt_conv (const int * v1, const int * v2, int n) {
    int ** matrix = (int **) malloc(n*sizeof(int*));
    for(int row = 0;row < n; row++) {
        matrix[row] = (int *) malloc(n*sizeof(int));
        for(int col = 0; col < n; col++)
            matrix[row][col] = v1[row] * v2[col];
    }

    return matrix;
}


//produces vector with pattern such as
//1 2 1 | 1 2 4 2 1 | 1 2 4 8 4 2 1... depending on the size argument
//Should be used only with odd sizes
//used in
int * vekt_gauss_set(int size) {
    int * vect = (int *) malloc(size*sizeof(int));
    //initializing first element to fill the rest
    vect[0] = 1;
    vect[size-1] = 1;

    for(int i = 1; i <= size/2;i++) {
        int val = vect[i-1]*2;
        vect[i] = val;
        vect[size-1-i] = val;
    }

    return vect;
}

Kernel * kernel_set(int size, KernelType type) {
    Kernel * new = (Kernel *) malloc(sizeof(Kernel));

    new->size = type == veredge || type == horedge ? 3 : size; //In case of edge detection only 3x3 kernel is allowed (yet)
    new->type = type;

    new->maxval = 0;

    if(type == blur)
    {
        int * vect = vekt_gauss_set(size);

        new->grid = vekt_conv(vect, vect, size);
        free(vect);

    } //IF BLUR
    else {
        int vect1[3] = {1,2,1};
        int vect2[3] = {-1,0,1};

        new->grid = vekt_conv(vect1, vect2, 3);
        if(type == veredge)
        {
            //Mirror on the main axis
            for(int row = 0; row < size; row++) {
                for(int col = row; col < size; col++) {
                    int tmp = new->grid[row][col];
                    new->grid[row][col] = new->grid[col][row];
                    new->grid[col][row] = tmp;
                }
            }
        }


    }
    return new;
}

//Only works with Kernels that were created by kernel_set
void kernel_free(Kernel * kernel) {
    for(int i = 0; i < kernel->size;i++)
        free(kernel->grid[i]);
    free(kernel->grid);
    free(kernel);
}

//Run the kernel operation on a pixel
//img    <- the image to do the operation on
//kernel <- the kernel to run teh operation with
//px     <- the pixels x coordinate in the img
//py     <- the pixels y coordinate in the img
double kernel_add(PPM * img, Kernel * kernel, int pxRow, int pxCol, int rgb) {
    double sum = 0;

    int globRow = pxRow - kernel->size /2;
    int globCol = pxCol - kernel->size /2;

    int startRow, endRow;
    int startCol, endCol;

    startRow = globRow < 0 ? kernel->size - globRow : 0;
    startCol = globCol < 0 ? kernel->size - globCol : 0;

    endRow = globRow + kernel->size > img->height ? (globRow - kernel->size) - img->height : kernel->size - 1;
    endCol = globCol + kernel->size > img->width ? (globCol - kernel->size) - img->width : kernel->size - 1;
    for(int row = startRow; row <= endRow; row++)
        for(int col = startCol; col <= endCol; col++)
                sum += img->data[globRow + row][globCol + col][rgb] * kernel->grid[row][col];

    return sum;
}


//used in the blur operation
//The invoker is in charge of calling the free() function
double kernel_avg(PPM * img, Kernel * kernel, int pxRow, int pxCol, int rgb) {
    int countf = 0; //On the edges of the picture not all of the kernel's fields will count
    double avg = 0;

    int globRow = pxRow - kernel->size /2;
    int globCol = pxCol - kernel->size /2;

    int startRow, endRow;
    int startCol, endCol;

    startRow = globRow < 0 ? kernel->size - pxRow -1 : 0;
    startCol = globCol < 0 ? kernel->size - pxCol -1 : 0;

    //A kernel azon részéig megy, ami még bent van a kép keretein bleül -> a képen kieső kernel részét ignorálja
    endRow = pxRow + kernel->size / 2 >= img->height ? kernel->size - ((pxRow + kernel->size / 2 + 1) - img->height): kernel->size;
    endCol = pxCol + kernel->size / 2 >= img->width ? kernel->size - ((pxCol + kernel->size / 2 + 1) - img->width): kernel->size;

    for(int row = startRow; row < endRow; row++) {
        for (int col = startCol; col < endCol; col++) {
                countf += kernel->grid[row][col];
                avg += img->data[globRow + row][globCol + col][rgb] * kernel->grid[row][col];
        }
    }

    avg = capval (avg /= countf, 0 , img->maxval);

    return avg;
}
