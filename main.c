#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "Headers/ppmio.h"
#include "Headers/ppmmod.h"
#include "Headers/segedfvk.h"

#include "Headers/cmdhelper.h"

int main(int argc, char *argv[]) {
    PPM myImage;
    //dgedit {input_img} {option} [<option_value>] [output_image]
    ppm_open(&myImage, argv[1]);

    printf("\n");

    //Validating the option chosen, and modifying the image
    execCommand(&myImage, argv[2], argv[3]);

    printf("\n");

    //If the picture was successfully openened and modified it'll write to file
    //Checking if there was a specified filename, while keeping in mind the number of options
    //(In some operations an option_value isn't needed)
    writeWCheck(myImage, argv[1], argv[2], argv[3], argv[4]);


    ppm_close( &myImage);

    return 0;
}




