//
// Created by abodi on 01/06/2023.
//
#include "codec.h"
#include <stdio.h>
#include <string.h>

void main ()
{
    char str[2000]={0};
    //printf("%d",strlen(str));
    for (int i = 0; i <=1998; ++i) {
        str[i]='a';
    }
    str[1999]=0;
    encrypt(str,12);
    printf("%s",str);
    int index=-1;
    for (int i = 0; i < 2000; ++i) {
        if(str[i]=='a') {
            index = i;
            break;
        }
    }
    /*
     * TODO we can understand encrypt/decrypt are limited to 1024 chars
     */
    printf("\nmax size/index the algorithm reached is %d",index);
}