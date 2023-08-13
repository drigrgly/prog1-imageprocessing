//
// Created by derig on 2021. 11. 05..
//
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "ppmmod.h"
#include "segedfvk.h"

#include "kernel.h"
#include "debugmalloc.h"

//------------------------
//ppm modifier operations
//------------------------

void ppm_changeBr(PPM * img, double pr) {
    printf("PROGRESS: ###");
    int brVal = (int) ( ((double) (pr - 100) / 100)*img->maxval);

    for(int i = 0; i < img->height;i++)
        for(int j = 0; j< img->width; j++)
            for(int p = 0; p < 3; p++)
                img->data[i][j][p] = (unsigned char) capval(img->data[i][j][p] + brVal, 0 ,img->maxval);

    printf("###");
}

void ppm_changeCr(PPM * img, double pr) {
    printf("PROGRESS: ###");
    double contrast = ((double) (pr - 100) / 100)*img->maxval;

    double contrCorFactor = (259*(255+ contrast )) / (255*(259- contrast));

    for(int i = 0; i < img->height;i++)
        for(int j = 0; j< img->width; j++)
            for(int p = 0; p < 3; p++)
                img->data[i][j][p] = (unsigned char) capval(contrCorFactor*(img->data[i][j][p]-128)+128, 0 ,img->maxval);
    printf("###");

}


//Converting to grayscale by mean, so r g and b are the same
void ppm_toGrayscale(PPM * img) {
    printf("PROGRESS: ###");
    for(int i = 0; i < img->height;i++)
        for(int j = 0; j< img->width; j++) {
            unsigned char * pixel = img->data[i][j];
            //unsigned char gray =  (unsigned char) round(0.21 * (double) pixel[0] + (double) 0.72*pixel[1] + (double) 0.07 * pixel[2]);
            unsigned char gray =  (unsigned char) round((double) (pixel[0] + pixel[1] + pixel[2]) / 3);

            for(int p = 0; p < 3; p++)
                img->data[i][j][p] = gray;

        }
    printf("###");
}

//Applies gaussian blur to the image
void ppm_blur(PPM *img, double pr) {

    printf("PROGRESS: ");

    Kernel * bkern = kernel_set(5, blur);


    double ** blurredImgR = grid2d_init(img->height, img->width);
    double ** blurredImgG = grid2d_init(img->height, img->width);
    double ** blurredImgB = grid2d_init(img->height, img->width);

    int maxrep = (int) ceil(capval(pr, 0, 100 )/10);
    printf("###");

    for(int repeat = 0; repeat < maxrep; repeat++) {
        for(int row = 0; row < img->height;row++)
            for(int col = 0; col < img->width; col++) {
                blurredImgR[row][col] = kernel_avg(img, bkern, row, col, 0);
                blurredImgG[row][col] = kernel_avg(img, bkern, row, col, 1);
                blurredImgB[row][col] = kernel_avg(img, bkern, row, col, 2);
            }

        for(int i = 0; i < img->height;i++)
            for(int j = 0; j< img->width; j++) {
                    img->data[i][j][0] = (unsigned char) round( blurredImgR[i][j]);
                    img->data[i][j][1] = (unsigned char) round( blurredImgG[i][j]);
                    img->data[i][j][2] = (unsigned char) round( blurredImgB[i][j]);
            }

        printf("###");
    }
    //Freeing the helper grid&
    grid2d_free(&blurredImgR, img->height);
    grid2d_free(&blurredImgG, img->height);
    grid2d_free(&blurredImgB, img->height);

    kernel_free(bkern);
    printf("###");

}

void ppm_gray_blur(PPM * img, double pr) {

    Kernel * bkern = kernel_set(5, blur);


    double ** blurredImg = grid2d_init(img->height, img->width);

    int maxrep = (int) ceil(capval(pr, 0, 100 )/10);

    for(int repeat = 0; repeat < maxrep; repeat++) {

        for(int row = 0; row < img->height;row++)
            for(int col = 0; col < img->width; col++) {
                double grayavg = kernel_avg(img, bkern, row, col, 0);
                blurredImg[row][col] = grayavg;
            }

        for(int i = 0; i < img->height;i++)
            for(int j = 0; j< img->width; j++)
                for(int p = 0; p < 3; p++)
                    img->data[i][j][p] = (unsigned char) round( blurredImg[i][j]);

        printf("###"); //<--- progress indicator
    }
    //Freeing the helper grid&
    grid2d_free(&blurredImg, img->height);

    kernel_free(bkern);
}

//Edge detection with vertical and horizontal sobel filters
void ppm_edge(PPM * img) {
    //in order to find the edges you first have to convert it to grayscale
    ppm_toGrayscale(img);
    printf("###");

    ppm_gray_blur(img, 30);
    printf("###");


    //Vertical and Horizontal kernels combined will result in the final edge detection image
    Kernel * vekern = kernel_set(3, veredge);
    Kernel * hekern = kernel_set(3, horedge);

    double ** vertEdges = grid2d_init(img->height, img->width);
    double ** horiEdges = grid2d_init(img->height, img->width);


    for(int row = 0; row < img->height; row++)
        for(int col = 0; col < img->width; col++) {
            vertEdges[row][col] = kernel_add(img, vekern, row, col, 0);
            horiEdges[row][col] = kernel_add(img, hekern, row, col, 0);
        }
    printf("###");

    for(int row = 0; row < img->height; row++)
        for(int col = 0; col < img->width; col++) {
            double vertSq = vertEdges[row][col]*vertEdges[row][col];
            double horiSq = horiEdges[row][col]*horiEdges[row][col];
            unsigned char combinedEdge = (unsigned  char) round( sqrt( vertSq+ horiSq ));
            for(int p = 0; p < 3; p++)
                img->data[row][col][p] = combinedEdge;
        }
    printf("###");



    //Freeing the mallocced variables
    kernel_free(vekern);
    kernel_free(hekern);
    grid2d_free(&vertEdges, img->height);
    grid2d_free(&horiEdges, img->height);
    printf("###");

}
