//
// Created by derig on 2021. 11. 28..
//

#include "ppm.h"

#ifndef DGEDIT_CMDHELPER_H
#define DGEDIT_CMDHELPER_H

void execCommand (PPM * myImage, char * option, char * optVal);
void writeWCheck (PPM myImage, char * ogImgName, char * option, char * optVal, char * imgPath );
#endif //DGEDIT_CMDHELPER_H
