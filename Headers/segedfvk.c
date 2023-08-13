//
// Created by derig on 2021. 11. 05..
//

#include <stdlib.h>
#include <string.h>

#include "segedfvk.h"


//caps a value in a closed interval defined by min and max
double capval(double value, int min, int max) {
    return value < min ? min : (value > max ? max : value);
}
bool isNumber(char * input) {
    for(int i =0 ; input[i] != '\0';i++)
        if(!isdigit(input[i]))
            return false;
    return true;
}


//Creates a height * width grid ouf doubles
double ** grid2d_init(int height, int width) {
    double ** new = (double **) malloc(height * sizeof(double ));

    for(int i = 0; i < height; i++)
        new[i] = (double *) malloc(width * sizeof (double));

    return new;
}

//frees a 2d matrix
void grid2d_free(double *** grid, int height) {
    for(int i = 0; i < height; i++)
        free((*grid)[i]);
    free((*grid));
}


char * strinsert(char * str, int index, char * plus) {

    int newsize = (int) (strlen(str) + strlen(plus) + 1);
    char * newstring = (char *) malloc( newsize * sizeof(char));

    int olen = strlen(str);

    index = (int) capval(index, 0, olen-1);



    //for(int i = 0; i < index; i++)
    //  newstring[newindex++] = str[i];

    strncpy(newstring, str, index);
    newstring[index] = '\0'; //<- strcat miatt

    //for(int i = 0; i < strlen(plus); i++)
    //  newstring[newindex++] = plus[i];

    strcat(newstring, plus);

    //for(int i = index; i < olen; i++)
    //  newstring[newindex++] = str[i];
    strcat(newstring, (str + index));


    return newstring;


}