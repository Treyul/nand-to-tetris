#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>



int main()
{ 
    char* string = "64555";
    printf("length of string: %d, length of int: %d",strlen(string), no_of_int(5578755));
    // Left strip the string
    // iterate through the chars of the string comparing if it equals to the " "character
    int str_len = strlen(string);
    int i;
    for (i = 0;i < str_len;i++)
    {
        // char s = string +
        if (string[i] == ' ')
        {
            continue;
        }
        else break;
    }

    printf("eof");
    // char* rev = strrev(string);
    // printf("the resultant string is%d:%s\n",i,rev);

    //right strip the string 
   
    char* string2 = calloc(str_len - i,sizeof(char));
    strcpy(string2,string + i);

    str_len = str_len - i;
    int span = str_len - 1;
    i = 0;
    printf("length is %d",str_len);
    for (span; span > 0; span--)
    {
        printf("%c",string2[span-1]);
        if (isspace(string2[span]))
        {
            i++;
        }
        else
        {
            break;
        }
    }

    char* string3 = calloc(str_len-i,sizeof(char));
    strncpy(string3,string2,str_len-i);
    free(string2);
    printf("final string is :%s:i=%d",string3,i);
    
    return 0;
}


/**
 * @brief 
 * take string and the split character as argument
 * 
 * @param string 
 * @param chr 
 * @return char** 
 */
char** strsplit(char** Result_Array,char* string, char* chr)
{
    char** result;

    return result;
}
/*
get string
split the character and store both results into an array
do this repeatedly until result is 0
keep record of count 
*/