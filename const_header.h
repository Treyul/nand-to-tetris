/**
 * @file stack_ini.h
 * @author Emmanuel Kamau (emmanuelryley55@gmail.com)
 * @brief  
 * This file is used to initailize constant and declaration of functions
 * 
 * @version 0.1
 * @date 2025-07-11
 * 
 * @copyright Copyright (c) 2025
 */

 
//define memory segment addresses
#define STACK_P 0
#define LOCAL_P 1
#define ARG_P 2
#define THIS_P 3
#define THAT_P 4
#define TEMP_P 5
#define STATIC_P 17
#define POINTER_P 16

// declare all function
/**
 * @brief Takes the file name and return the file extension of the file
 * 
 * @param File_Name 
 * @return char* 
 */
char* File_Extension(char* File_Name);

/**
 * @brief Return the file name without extension 
 * 
 * @param File_Name 
 * @return char* 
 */
char* File_Preffix(char* File_Name);

/**
 * @brief read a new line on the file that has been opened
 * takes a file descriptor as an argument
 * @param File_Desc 
 * @return char* 
 */
char* readline(int File_Desc);

/**
 * @brief Takes a line of stack machine instructions and splits them
 * using the space character
 * @param instructions 
 * @return char** 
 */
char** Cmd_Segregate(char* instructions);

/**
 * @brief Takes an array of segmented instructions and parses
 * them into assembly language whiel also writing the
 * out puts into an assembly file
 * @param Dest_File_Desc 
 * @param Command_Array 
 * @return int 
 */
int Parse_File(int Dest_File_Desc, char** Command_Array);

int Push_Stack(int Asm_File_Desc,int Memory_Segment, char* Segement_Index);

int Pop_Stack(int Asm_File_Desc,int Memory_Segment, char* Segement_Index);


// removed code pending approval for deletion
               /* code */
                // char* cmd = calloc(4,sizeof(char));
                // sprintf(cmd,"@%s\n",mem_segment_number);
                // printf("%s\n",cmd);

                // write(Dest_File_Desc,cmd,strlen(cmd));
                // write(Dest_File_Desc,"D=A\n",4);
                // cmd = calloc(4,sizeof(char));
                // sprintf(cmd,"@%d\n",ARG_P);
                // write(Dest_File_Desc,cmd,strlen(cmd));
                // free(cmd);
                // write(Dest_File_Desc,"A=A+D\n",6);
                // write(Dest_File_Desc,"D=M\n",4);

                // cmd = calloc(3,sizeof(char));
                // sprintf(cmd,"@%d\n",STACK_P);
                // write(Dest_File_Desc,cmd,strlen(cmd));
                // write(Dest_File_Desc,"AM=M+1\n",7);
                // write(Dest_File_Desc,"A=A-1\n",6);
                // write(Dest_File_Desc,"M=D\n",4);

                // sprintf();