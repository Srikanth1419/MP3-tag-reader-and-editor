/*  NAME : G. SRIKANTH
    DATE : 07-02-2025
    BATCH : 24024_C
    Description: MP3 TAG READER AND EDITOR

    Sample Input : ./a.out -v sample.mp3 
    
    Sample Output:
                    --------------------------------SELECTED VIEW DETAILS--------------------------------

                    -------------------------------------------------------------------------------------
                                        MP3 TAG READER AND EDITOR FOR ID3V2
                    -------------------------------------------------------------------------------------
                    TITLE       :     hello
                    ARTIST      :     Aniruth
                    ALBUM       :     Vijay_Thalapathy
                    YEAR        :     2025
                    CONTENT     :     kollywood
                    COMMENT     :     ----sample-download
                    -------------------------------------------------------------------------------------

                    --------------------------DETAILS DISPLAYED SUCCESSFULLY------------------------------

*/
#include "mp3.h"

int main(int argc, char *argv[])
{

    if(argc == 1)
    {
        prompt();
        return 0;
    }
    status ret = validate_CLA(argc, argv);

    if(ret == V_SUCCESS)
    {
       if(display_mp3(argv[2]) ==  FAILURE) 
       {
            printf("INFO : Displaying not done Successfully\n");
       }
    }
    else if(ret == E_SUCCESS)
    {
       if( edit_mp3(argc, argv) == FAILURE)
       {
            printf("INFO : Edit not done Successfully\n");
            
       }
    }

     return 0;
}

status validate_CLA(int argc, char *argv[])
{
    if(strlen(argv[1]) == 2)        // check argc count is 2 or not 
    {
        if( strcmp(argv[1], "-v") == 0)     // check which operation user wants to perform 
        {
            if(argc == 3)                       // if user wants to perform view check for mp3 file present or not 
            {
                if(validate_mp3_file(argv[2]) == VALID)     // fun call to validate mp3 file 
                {
                    printf("--------------------------------SELECTED VIEW DETAILS--------------------------------\n\n");
                     return V_SUCCESS;
                }   
                else
                {
                    return FAILURE;
                }
            }
            else
            {
                prompt();
                return FAILURE;
            }
        }
        else if((strcmp(argv[1], "-e") == 0))       // check 2nd argument is -e or not 
        {
            if(argc >= 5)
            {
                if( (strcmp(argv[2],"-t") ==0) || (strcmp(argv[2],"-a")==0) || (strcmp(argv[2],"-A") ==0) || (strcmp(argv[2],"-m")==0) || (strcmp(argv[2],"-y")==0) || (strcmp(argv[2],"-c")==0))
                {
                    if(validate_mp3_file(argv[4]) == VALID)
                    {
                        printf("-----------------------------SELECTED EDIT DETAILS------------------------------\n\n");
                        return E_SUCCESS;
                    }
                    else   
                        return FAILURE;
                }
                else
                {
                    prompt();
                    return FAILURE;
                }
            }
            else
            {
                prompt();
                return FAILURE;
            }  
        }
        else
        {
            prompt();
            return FAILURE;
        }
    }
    else if(strcmp(argv[1], "--help") == 0)
    {
        printf("--------------------HELP MENU-------------------\n\n");
        printf("1. -v -> to view mp3 file contents\n");
        printf("2. -e -> to edit mp3 file contents\n");
        printf("%-10s 2.1 -t -> to edit Song Title\n"," ");
        printf("%-10s 2.2 -a -> to edit Artist Name\n"," ");
        printf("%-10s 2.3 -A -> to edit Album Name\n"," ");
        printf("%-10s 2.4 -y -> to edit Year\n"," ");
        printf("%-10s 2.5 -m -> to edit Content\n"," ");
        printf("%-10s 2.6 -c -> to edit Comment\n\n"," ");

        printf("------------------------------------------------\n\n");

    }
    else
    {
        prompt;
        return FAILURE;
    }
}

status validate_mp3_file(char *str)
{
    char *ptr = strstr(str, ".mp3");

    if(ptr)                             // check str has .mp3 extn has or not 
    {
        if(strcmp(ptr, ".mp3") == 0)       // checking .mp3 in substring of str 
        {
            FILE *fptr = fopen(str, "r");       // open file 

            if(fptr == NULL)                // checking file is open or not 
            {
                printf("\nINFO : %s -> This file is not present\n", str);
                return NOT_VALID;
            }
            fseek(fptr, 0,SEEK_END);            //moving off-set to end position to know the data 
            if(ftell(fptr) == 0)
            {
                printf("\nINFO : %s -> This file is Empty\n", str);
                return NOT_VALID;
            }
            rewind(fptr);
            char valid[4];
            fread(valid, 3, 1, fptr);

            
           if( strcmp(valid, "ID3") == 0)
            {
                 short int size;
                fread(&size, 4, 1, fptr);
                
                rewind(fptr);
                fseek(fptr, 10, SEEK_SET);
                
                fread(valid, 4, 1, fptr);

                if(strcmp(valid, "TIT2") != 0)
                {
                    printf("\nINFO : %s -> This file is not a ID3 version\n", str);
                    fclose(fptr);
                    return NOT_VALID;
                }
                else
                {
                    fclose(fptr);
                    return VALID;
                }          
            }    
           else
           {
                printf("\nINFO : %s -> This file is not a ID3 version\n", str);
                return NOT_VALID;
           }   
        }
        else
        {
            printf("\nINFO : %s -> This file is not a mp3 file\n",str);
            return NOT_VALID;
        }
    }
    else
    {
         printf("\nINFO : %s -> This file is not a mp3 file\n",str);
         return NOT_VALID;
    }
}


void prompt()
{
    printf("-------------------------------------------------------------------------------------------------------\n\n");
    printf("ERROR : ./a.out : INVALID ARGUEMENTS\nUSAGE :\n");
    printf("To view please pass like  : ./a.out -v mp3file_name\n");
    printf("To Edit please pass like  : ./a.out -e -t/-a/-A/-m/-y/-c  change_text mp3file_name\n");
    printf("To get help pass like     : ./a.out --help\n");
    printf("-------------------------------------------------------------------------------------------------------\n\n");
}