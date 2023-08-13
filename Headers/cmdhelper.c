//
// Created by derig on 2021. 11. 28..
//
#include <string.h>

#include "cmdhelper.h"
#include "ppm.h"
#include "ppmio.h"
#include "ppmmod.h"
#include "segedfvk.h"


void validateOption(PPM * img, char * option_value) {
    if(option_value == NULL) {
        printf("Option value entered is NULL");
        exit(2);
    }

    if(!isNumber(option_value)) {
        printf("Option value entered is invalid");
        exit(2);
        ppm_close(img);
    }
}


void execCommand (PPM * myImage, char * option, char * optVal) {

    if(option == NULL) {
        printf("No option entered");
        exit(3);
    }

    char * ptr; //needed for strol

    if(strcmp(option, "-br") == 0) {
        validateOption(myImage, optVal);
        ppm_changeBr(myImage, capval(strtol(optVal, &ptr, 10), 0, 200));
    }
    else if (strcmp(option, "-c") == 0) {
        validateOption(myImage, optVal);
        ppm_changeCr(myImage, capval(strtol(optVal, &ptr, 10), 0, 200));
    }
    else if (strcmp(option, "-bl") == 0) {
        validateOption(myImage, optVal);
        ppm_blur(myImage, capval(strtol(optVal, &ptr, 10), 0, 100));
    }
    else if (strcmp(option, "-gr") == 0) {
        ppm_toGrayscale(myImage);
    }
    else if (strcmp(option, "-e") == 0) {
        ppm_edge(myImage);
    }
    else {
        printf("Invalid option entered");
        exit(3);
    }
}

void writeWCheck (PPM myImage, char * ogImgName, char * option, char * optVal, char * imgPath ) {
        if (strcmp(option, "-gr") == 0 || strcmp(option, "-e") == 0) {
            if(optVal == NULL) {
                char * newFileName = strinsert(ogImgName, (int) strlen(ogImgName) - 4, "_edited");
                ppm_fprintf(&myImage, newFileName);
                free(newFileName);
            }
            else
                ppm_fprintf(&myImage, optVal);
        }
        else if(imgPath == NULL) {
            char * newFileName = strinsert(ogImgName, (int) strlen(ogImgName) - 4, "_edited");
            ppm_fprintf(&myImage, newFileName);
            free(newFileName);
        }
        else
        ppm_fprintf(&myImage, imgPath);
};