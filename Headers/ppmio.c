//
// Created by derig on 2021. 11. 05..
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ppmio.h"
//#include "debugmalloc.h"

//ppm I/O operations
void ppm_open(PPM * img, char * filename) {
    char sorBuffer[100] = {'0'};
    FILE * fptr = NULL;

    if(filename == NULL) {
        printf("File name is NULL");
        exit(1);
    }

    fptr = fopen(filename,"rb");
    if(fptr == NULL) {
        printf("Error while opening file!");
        exit(1);
    }

    img->path = (char *) malloc((strlen(filename) + 1) * sizeof(char));
    strcpy(img->path, filename);

    //First 3 non comment rows contain the header
    int headerLines = 0;

    const int BUFFER_SIZE = sizeof sorBuffer;

    while(headerLines < 3) {
        fgets(sorBuffer, BUFFER_SIZE, fptr);
        ftell(fptr);
        if(sorBuffer[0] != '#') {
            switch (headerLines) {
                case 0:
                    if (sorBuffer[0] != 'P' || sorBuffer[1] != '6') {
                        printf("Image is not in the right format(p6)");
                        exit(1);
                    }

                    img->mnumber = "P6\0";
                    break;
                case 1:
                    sscanf(sorBuffer, "%d %d", &(img->width), &(img->height));
                    break;
                case 2:
                    sscanf(sorBuffer, "%d", &(img->maxval));
                    break;

                default:
                    break;

            }
            headerLines++;
        }
    }

    img->data = (unsigned char ***) malloc(img->height * sizeof(unsigned char **));

    //Ha a maxval < 256 akkor 1 byte a pixelek értéke, különben 2 byte-ot kell beolvasni
    const size_t READ_SIZE = img -> maxval < 256 ? 1 : 2;
    img->dataSize = READ_SIZE;


    //After the header lines, the entire image comes in as continuous binary data
    //Then it gets separated into rows and columns
    for(int i = 0; i < img->height; i++) {
        int pxCount = 0;
        img->data[i] = (unsigned char **) malloc(img->width * sizeof(unsigned char *));

        unsigned char * c = (unsigned char *) malloc(READ_SIZE * sizeof(unsigned char));

        while(pxCount < img->width)
        {
            img -> data[i][pxCount] = (unsigned char *) malloc(3 * READ_SIZE * sizeof(unsigned char));
            for(int p = 0; p < 3; p++) {
                //Reading the image by pixel rgb values
                fread(c, READ_SIZE, 1,fptr);
                img -> data[i][pxCount][p] = *c;

            }
            //incrementing the read pixel counter
            pxCount++;

        }

        free(c);
    }

    fclose(fptr);
}

void ppm_fprintf(const PPM * img, char * path) {
    FILE * fptr = NULL;

    fptr = fopen(path, "wb");

    if(fptr == NULL) {
        printf("Error occurred while writing to file!");
        exit(1);
    }

    //Header kiírása
    fprintf(fptr, "%s\n%d %d\n%d\n",img->mnumber, img->width, img->height, img->maxval);

    for(int row = 0; row < img->height; row++)
        for(int col = 0; col < img->width; col++)
            for(int p = 0; p < 3; p++)
                fwrite(&(img->data[row][col][p]), img->dataSize, 1, fptr);


    printf("File written to: %s\n", path);
    fclose(fptr);
}

void ppm_close(PPM * img) {
    for(int row = 0; row < img->height; row++) {
        for(int col = 0; col < img->width; col++)
            free(img->data[row][col]);
        free(img->data[row]);
    }
    free(img->data);

    free(img->path);
}