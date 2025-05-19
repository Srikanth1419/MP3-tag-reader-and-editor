
#include "mp3.h"

status display_mp3(char *str)
{
    FILE *fptr = fopen(str, "r");

    if(fptr == NULL)
    {
        printf("INFO : %s -> This file is not opened successfully\n", str);
        return FAILURE;
    }

    int size;
    printf("-------------------------------------------------------------------------------------\n");
    printf("%*s", 60, "MP3 TAG READER AND EDITOR FOR ID3V2\n");
    printf("-------------------------------------------------------------------------------------\n");
    fseek(fptr,10, SEEK_SET);           // skipping header 10 bytes 
    for(int i=0;i<6;i++)
    {
        char tag[5];

        fread(tag, 4, 1, fptr);         // reading tag from file 
      
        fread(&size, 4, 1, fptr);
      
        big_to_little(4, (char*)&size);            // function call to con

        char *data = malloc(200);
        
        fseek(fptr, 3, SEEK_CUR);       // skip 3bytes -> flags 

        fread(data, size-1, 1, fptr);

        data = realloc(data, strlen(data) + 1);

        if( strcmp(tag, "TIT2") == 0)
        {
            printf("TITLE       :     %s\n", data);
        }
        else if(strcmp (tag,"TPE1") == 0)
        {
            printf("ARTIST      :     %s\n", data);
        }
        else if( strcmp(tag, "TALB") == 0)
        {
            printf("ALBUM       :     %s\n", data);
        }
        else if( strcmp(tag, "TYER") == 0)
        {
            printf("YEAR        :     %s\n", data);
        }
        else if(strcmp(tag, "TCON") == 0)
        {
            printf("CONTENT     :     %s\n", data);
        }
        else if(strcmp(tag, "COMM") == 0)
        {
            printf("COMMENT     :     %s\n", data);
        }
    }
    printf("-------------------------------------------------------------------------------------\n\n");

     printf("--------------------------DETAILS DISPLAYED SUCCESSFULLY------------------------------\n\n");
}

void big_to_little( int size, char *ptr)
{
    char temp;              
    int i=0;
    int j =size-1;
     
    while(i<size/2)
    {
       temp = *(ptr +i);                // swapping operation by accessing byte by byte
       *(ptr +i) = *(ptr+j);
       *(ptr +j) = temp;
       i++;
       j--;
    }
}
