
#include "mp3.h"

status edit_mp3(int argc, char *argv[])
{
    int flag_done =0;
    char temp_tag[5];
    char check[10];
    if (assign_tag(argv[2], temp_tag, check) == FAILURE)
    {
        printf("INFO : Assigning of  Tags  failed\n");
        return FAILURE;
    }
    temp_tag[4] = '\0';

    char file[] = "temp.mp3";
    FILE *dest_fptr = fopen(file, "w");

    if (dest_fptr == NULL)
    {
        printf("INFO : %s -> File open failed\n", file);
        return FAILURE;
    }

    FILE *src_fptr = fopen(argv[4], "r");

    if (src_fptr == NULL)
    {
        printf("INFO : %s -> File open failed\n", argv[4]);
        return FAILURE;
    }

    printf("----------------CHANGE THE %s-------------------\n\n", check );

    char buff[100];

    fread(buff, 10, 1, src_fptr);   // reading header from src file
    fwrite(buff, 10, 1, dest_fptr); // writing header into dest file

    char tag[5];
    int src_size;
    for (int i = 0; i < 6; i++)
    {
        fread(tag, 4, 1, src_fptr);           // read tag from original file
        tag[4] = '\0';
        fwrite(tag, 4, 1, dest_fptr);         // copy tag into dest file

        fread(&src_size, 4, 1, src_fptr); // Read frame size

        int b_size = src_size;
       
        big_to_little(4, (char *)&src_size);                // Convert size to little-endian
                                            
      
        if (strcmp(tag, temp_tag) == 0)                         // chek tags are same or not
        {
            int new_size = strlen(argv[3]) ;                 // finding size of changing data

            int L_size = new_size;                          // take temp size of new_size
            new_size += 1;
           
            little_to_big(4, (char *)&new_size);            // convert new_size into little endian to big endian

            fwrite(&new_size, 4, 1, dest_fptr);            // writing new_size into dest file

            char flag[3];

            fread(flag, 3, 1, src_fptr);                // reading flags from src file

            fwrite(flag, 3, 1, dest_fptr);              // storing flags in dest file

            fwrite(argv[3], L_size, 1, dest_fptr);      // writing changing data into dest file

            fseek(src_fptr, src_size-1, SEEK_CUR);      // skipping src_size -1 times from current position

            unsigned char ch;
            while (fread(&ch, 1, 1, src_fptr) > 0) // copying remaining data into dest file
            {
                fwrite(&ch, 1, 1, dest_fptr);
            }

            flag_done = 1;
            fclose(dest_fptr);         // close dest file
            fclose(src_fptr);         // close src file
            break; 
                   
        }
        else
        {
            fwrite(&b_size, 4, 1, dest_fptr); // writing size into dest file

            char flag[3];

            fread(flag, 3, 1, src_fptr);            // reading 3 bytes from src file
            fwrite(flag, 3, 1, dest_fptr);          // writing 3 bytes into dest file

            char data[100];

            fread(data, src_size - 1, 1, src_fptr); // reading data from src file

            fwrite(data, src_size - 1, 1, dest_fptr); // writing data into dest file
        }
    }


    src_fptr = fopen(argv[4], "w");
    if(src_fptr == NULL)
    {
        printf("File not Opened\n");
        return FAILURE;
    }

     dest_fptr = fopen(file, "r");

     if(dest_fptr == NULL)
    {
        printf("INFO : %s -> File open failed\n",file);
        return FAILURE;
    }

    unsigned char ch;
    while( fread(&ch, 1, 1, dest_fptr) > 0)                
    {
        fwrite( &ch, 1, 1, src_fptr);
    }
    
     remove(file);                // removing temporary created file
            

    if(flag_done)
    {
        
        printf("%s    : %s \n\n", check, argv[3]);
        printf("--------------%s CHANGED SUCCESSFULLY------------\n\n", check);
        return E_SUCCESS;
    }
    
}

void little_to_big(int size, char *ptr)
{
    char temp;
    int i = 0;
    int j = size - 1;

    while (i < size / 2)
    {
        temp = *(ptr + i);          // swapping operation by accessing byte by byte
        *(ptr + i) = *(ptr + j);
        *(ptr + j) = temp;
        i++;
        j--;
    }
}

status assign_tag(char *argv, char *tag, char *check)
{

    if (strcmp(argv, "-t") == 0)        // if -t means assigning TIT2 tag for title 
    {
        strcpy(tag, "TIT2");
        strcpy(check, "TITLE");
        return E_SUCCESS;
    }
    else if (strcmp(argv, "-a") == 0)      // if -a means assigning TPE1 tag for Artist name
    {
        strcpy(tag, "TPE1");
        strcpy(check, "ARTIST");
        return E_SUCCESS;
    }
    else if (strcmp(argv, "-A") == 0)       // if -A means assigning TALB tag for Album name
    {
        strcpy(tag, "TALB");
        strcpy(check, "ALBUM");
        return E_SUCCESS;
    }
    else if (strcmp(argv, "-y") == 0)       // if -y means assigning TYER tag for Year 
    {
        strcpy(tag, "TYER");
        strcpy(check, "YEAR");
        return E_SUCCESS;
    }
    else if (strcmp(argv, "-m") == 0)          // if -m means assigning TCON tag for Content
    {
        strcpy(tag, "TCON");
        strcpy(check, "CONTENT");
        return E_SUCCESS;
    }
    else if (strcmp(argv, "-c") == 0)          // if -c means assigning COMM tag for comments
    {
        strcpy(tag, "COMM");
        strcpy(check, "COMMENT");
        return E_SUCCESS;
    }
    return FAILURE;
}