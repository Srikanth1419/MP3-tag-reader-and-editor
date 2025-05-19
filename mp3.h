
#ifndef MP3_H
#define MP3_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef enum mp3
{
    V_SUCCESS,         
    E_SUCCESS,
    FAILURE,
    VALID,
    NOT_VALID
}status;


status validate_CLA(int argc, char *argv[]);

status validate_mp3_file(char *str);

status display_mp3(char *str); 

void prompt();

void big_to_little( int size, char *ptr);

status edit_mp3(int argc, char *argv[]);

status assign_tag(char *argv, char *tag, char *check);

void little_to_big(int size, char *ptr);



#endif