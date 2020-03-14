#include "revert_string.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void RevertString(char *str)
{
	char *temp = malloc(sizeof(char) * (strlen(str) + 1));
    int a = sizeof(char) * (strlen(str));

    strcpy(temp, str);


    int i;
    for(i=0; i<a; i++)
    { 
        str[i] = temp[a-i-1];
    }
    str[a+1] = '\0';
    free(temp);

}

