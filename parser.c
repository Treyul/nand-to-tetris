#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<errno.h>
#include<error.h>
#include<time.h>
#include<stdlib.h>
#include<ctype.h>
#include"stack_ini.h"

int no_of_int(int x)
{
    int num = 0;
    while(x>0)
    {
        x /= 10;
        num++;
    }
    return num;
}
//TODO add support for file path
// TODO create a variadic function to write a formatted string to a file descriptor

int main(int argc,char* argv[])
{
    clock_t start, end;
    double time_spent;
    start = clock();

    //get the vm file name and the name to be of the compiled file
    if (argc < 2)
    {
        fprintf(stderr,"Please provide a vm file to be compiled");
        return -1;
    }
    else if ( argc > 1)
    {
        //TODO verify extension and existence of file of input file
        File_Name = argv[1];
        
        int fexists = access(File_Name, F_OK);
        
        //ensure file exists
        if (fexists < 0)
        {
            fprintf(stderr,strerror(errno));
            return fexists;
        }
        else
        {
            int name = strlen(File_Name);
            char* File_Extn = File_Extension(File_Name);
         
            if ( !strcmp(File_Extn,Src_FExt) )
            {
                //validate the destination file if it exists
                //destination file not defined
                if (argc < 3)
                {
                    //TODO also check if a file already exists with the same name
                    Dest_File_Name = File_Preffix(File_Name);
                    
                    //create output file ith the same prefix as the source file
                    (Dest_File_Name,strlen(Dest_File_Name) + strlen(Dest_FExt) - 1);
                    strcat(Dest_File_Name,Dest_FExt);
                    
                    int fd = open(Dest_File_Name,O_CREAT | O_TRUNC | O_RDWR);
                    if (fd < 0)
                    {
                        fprintf(stderr,"An error has occured when creating assembly file: %s",strerror(errno));
                        return fd;
                    }
                    close(fd);
                }
                else
                {
                    //ensure destination file exists
                    Dest_File_Name = argv[2];
                    int fexists = access(Dest_File_Name,F_OK);
                    // cjeck if file exists
                    if(fexists < 0)
                    {
                        fprintf(stderr,strerror(errno));
                        return fexists;
                    }
                    //check if the file is valid
                    char* dest_ext = File_Extension(Dest_File_Name);
                    if(strcmp(dest_ext,Dest_FExt))
                    {
                        fprintf(stderr,"The destination file is not a valid file\n");
                        return -1;
                    } 
                }
            }
            else
            {
                fprintf(stderr,"The Source file is not valid. Choose another File\n");
                return -1;
            }
        }
    }

    
    // open file without translation the character \n to \r\n
    int Source_File = open(File_Name,O_RDONLY | O_BINARY);
    // clear contents of destination file while opening it
    int Destination_File = open(Dest_File_Name,O_WRONLY | O_TRUNC);
    if (Source_File > 0 && Dest_File_Name > 0)
    {
        //do operation since file has been opened successfully
        char* line = readline(Source_File);
        
        char** cmds;

        // check if destination file has write permissions and change if not
        int Read_permission = access(Dest_File_Name,W_OK);

        if (Read_permission < 0 )
        {
            chmod(Dest_File_Name,666);
        }

        while (line)
        {
            cmds = Cmd_Segregate(line);
            Parse_File(Destination_File, cmds);
            
            line = readline(Source_File);
            printf("This is the function return:... %s\n",line);
       
        }

        //Loop the assembly program to the end
        char* end_loop = "(END)\n@END\n0;JMP";
        write(Destination_File,end_loop,strlen(end_loop));
        printf("closing files\n");
        close(Source_File);
        close(Destination_File);
    }
    else
    {
        fprintf(stderr,"An error has occured when opening the files: %s", strerror(errno));
        return -1;
    }
    
    end = clock();
    time_spent =  (double)(end - start) / CLOCKS_PER_SEC;
    printf("\nExecution time: %f seconds, translating %d lines.\n", time_spent,line_number-1);
    return 0;
}

char* File_Extension(char* File_Name)
{
    char *dot = strchr(File_Name, '.');
    if (!dot) {
    /* no extension */
    fprintf(stderr,"The file has no valid extension\n");
    return "";
    }

   return dot + 1;
}

char* File_Preffix(char* File_name)
{
    char* dot = strchr(File_name,'.');
    
    int filename_len = strlen(File_name);
    int prefix_len = filename_len - strlen(dot) + 1;
    
    char* prefix = malloc(prefix_len);

    strncpy(prefix,File_name,prefix_len);
    prefix[prefix_len] = '\0';
    return prefix;
}

char* readline(int File_Desc)
{
    // read 25 bytes from file
    char* Read_Buf = calloc(25,sizeof(char));
    int rd = read(File_Desc,Read_Buf,25);
    
    // if error occurs during reading, close operation
    if (!rd)
    {
        printf("error\n");
        return NULL;
    }
    //check for the first occurence of new line character in the read line(s)
    char* Line_Noise = strchr(Read_Buf,'\n');
    int Size_of_line;
    int pos;
    printf("line numbers are: %d : %d",rd>=25,Line_Noise==NULL);

    //last line in file
    if (Line_Noise == NULL && strcmp(Read_Buf," "))
    {
        Size_of_line = strlen(Read_Buf);
        line_number++;
    }
    //TODO catch error on lines with gt 25 characters
    else if (Line_Noise == NULL && rd >= 25)
    {
        printf("Error in line read");
    }
    
    else
    {
        Size_of_line = strlen(Read_Buf) - strlen(Line_Noise);
        line_number++;
        if (!Size_of_line)
        {
            // Advance upto the next line with data

        }
        //since the result of Line noise also includes the new line character we minus 1 from the supposed length
        int pos = lseek(File_Desc,-strlen(Line_Noise)+1,SEEK_CUR);
        
    }
    
    char* Read_Line = calloc(Size_of_line,sizeof(char));
    
    //copy contents of line to the read_line
    strncpy(Read_Line,Read_Buf,Size_of_line);
    free(Read_Buf);

    //strip leading whitespaces on the readline
    int i;
    for (i = 0;i < Size_of_line;i++)
    {
        if (Read_Line[i] == ' ') continue;
        else break;
    }
    
    //copy the stripped string to a new memory
    char* string2 = calloc(Size_of_line - i,sizeof(char));
    strncpy(string2,Read_Line + i,Size_of_line - i);
    free(Read_Line);

    //right strip the string
    Size_of_line = Size_of_line - i;
    int span = Size_of_line - 1;
    i = 0;
    
    printf("length is %d : %d\n",Size_of_line,span);
    for (span; span > 0; span--)
    {
        if (isspace(string2[span]))  i++;
        else   break;
    }

    // copy final string to a new memory
    char* string3 = calloc(Size_of_line-i,sizeof(char));
    strncpy(string3,string2,Size_of_line-i);
    free(string2);

    printf("computed string is : %s :\n",string3);
    return string3;
}

char** Cmd_Segregate(char* instructions)
{   //TODO raise error if the commands are more than three

    char** cmd_array = calloc(strlen(instructions),sizeof(char));
    int count =0;
    char *commands = strdup(instructions);
    
    // iterate through the string separating its contents using " " as separator
    while(commands)
    {
        char* commands_ext = strchr(commands,' ');
        
        //compute the last command and break out of loop
        if (commands_ext == NULL && strcmp(commands," "))
        {
            cmd_array[count] = commands;
            break;
        }
        
        //get length of commands
        int cmd_len = strlen(commands) - strlen(commands_ext);
        char* command = calloc(cmd_len + 1, sizeof(char));
        char* cpy = strncpy(command, commands, sizeof(char) * cmd_len);
        
        printf("cycle return is::%d :%s:%s\n",cmd_len,command,commands_ext+1);
        
        //break out of operation for blank lines 
        if (!cmd_len) break;

        cmd_array[count] = command;
        
        char* temp = strdup(commands_ext + 1);
        
        free(commands);
        commands = calloc(strlen(temp),sizeof(char));
        commands = temp;
        count++;
    }
    printf("computed %d commands\n",count);

    return cmd_array;
}

int Parse_File(int Dest_File_Desc, char** Command_Array)
{
    // decontruct first element to get type of command
    char* command = Command_Array[0];

    // check if command is arithmetic or stack operands
    if (!strcmp(command,"push") || !strcmp(command,"pop"))
    {
        // get memory segments to be manipulated
        char* memory_segment = Command_Array[1];
        char* mem_segment_number = Command_Array[2];
        
         if( !strcmp(command,"push"))
        {
    
            if (!strcmp(memory_segment,"argument"))
            {
                Push_Stack(Dest_File_Desc, ARG_P, mem_segment_number);
            }
            else if (!strcmp(memory_segment,"local"))
            {
                Push_Stack(Dest_File_Desc,LOCAL_P,mem_segment_number);
            }
            else if (!strcmp(memory_segment,"constant"))
            {
                
                Push_Stack(Dest_File_Desc,STACK_P,mem_segment_number);
            }
            else if (!strcmp(memory_segment,"static"))
            {
                
                Push_Stack(Dest_File_Desc,STATIC_P,mem_segment_number);
            }
            else if (!strcmp(memory_segment,"this"))
            {
                Push_Stack(Dest_File_Desc,THIS_P,mem_segment_number);
            }
            else if (!strcmp(memory_segment,"that"))
            {
                Push_Stack(Dest_File_Desc,THAT_P,mem_segment_number);
            }
            else if (!strcmp(memory_segment,"pointer"))
            {
                Push_Stack(Dest_File_Desc,POINTER_P,mem_segment_number);
            }
            else if (!strcmp(memory_segment,"temp"))
            {
                Push_Stack(Dest_File_Desc,TEMP_P,mem_segment_number);
            }
            
        }
        else if (!strcmp(command,"pop"))
        {
            if (!strcmp(memory_segment,"argument"))
            {
                Pop_Stack(Dest_File_Desc, ARG_P, mem_segment_number);
            }
            else if (!strcmp(memory_segment,"local"))
            {
                Pop_Stack(Dest_File_Desc,LOCAL_P,mem_segment_number);
            }
            else if (!strcmp(memory_segment,"constant"))
            { 
                Pop_Stack(Dest_File_Desc,STACK_P,mem_segment_number);
            }
            else if (!strcmp(memory_segment,"static"))
            {
                Pop_Stack(Dest_File_Desc,STATIC_P,mem_segment_number);
            }
            else if (!strcmp(memory_segment,"this"))
            {
                Pop_Stack(Dest_File_Desc,THIS_P,mem_segment_number);
            }
            else if (!strcmp(memory_segment,"that"))
            {
                Pop_Stack(Dest_File_Desc,THAT_P,mem_segment_number);
            }
            else if (!strcmp(memory_segment,"pointer"))
            {
                Pop_Stack(Dest_File_Desc,POINTER_P,mem_segment_number);
            }
            else if (!strcmp(memory_segment,"temp"))
            {
                Pop_Stack(Dest_File_Desc,TEMP_P,mem_segment_number);
            }   
        }
    }
    
    else
    {
        // perform arithmetic and logical operations
        if (!strcmp(command,"add"))
        {
            char* Ar_Ll_Cmd = "@0\nAM=M-1\nD=M\n@0\nAM=M-1\nM=M+D\n@0\nAM=M+1\n";
            int length = strlen(Ar_Ll_Cmd);
            write(Dest_File_Desc,Ar_Ll_Cmd,length);
        }
        else if (!strcmp(command,"sub"))
        {
            char* Ar_Ll_Cmd = "@0\nAM=M-1\nD=M\n@0\nAM=M-1\nM=M-D\n@0\nAM=M+1\n";
            int length = strlen(Ar_Ll_Cmd);
            write(Dest_File_Desc,Ar_Ll_Cmd,length);
        }
        else if (!strcmp(command,"or"))
        {
            char* Ar_Ll_Cmd = "@0\nAM=M-1\nD=M\n@0\nAM=M-1\nM=M|D\n@0\nAM=M+1\n";
            int length = strlen(Ar_Ll_Cmd);
            write(Dest_File_Desc,Ar_Ll_Cmd,length);
        }
        else if (!strcmp(command,"gt"))
        {
            int length = strlen("@0\nAM=M-1\nD=M\n@0\nAM=M-1\nD=M-D\n@GT\nD;JGT\n@0\nA=M\nM=0\n@LT\nD;JLT\n(GT)\n@0\nA=M\nM=-1\n(LT)\n@0\nAM=M+1\n") + no_of_int(logicjmp) * 4;
            char* Ar_Ll_Cmd = calloc(length,sizeof(char)); 
            sprintf(Ar_Ll_Cmd,"@0\nAM=M-1\nD=M\n@0\nAM=M-1\nD=M-D\n@GT%d\nD;JGT\n@0\nA=M\nM=0\n@LT%d\nD;JLT\n(GT%d)\n@0\nA=M\nM=-1\n(LT%d)\n@0\nAM=M+1\n",logicjmp,logicjmp,logicjmp,logicjmp);
            write(Dest_File_Desc,Ar_Ll_Cmd,length);
            free(Ar_Ll_Cmd);
            logicjmp++;
        }
        else if (!strcmp(command,"lt"))
        {
             int length = strlen("@0\nAM=M-1\nD=M\n@0\nAM=M-1\nD=D-M\n@LT\nD;JLT\n@0\nA=M\nM=0\n@GT\nD;JGT\n(LT)\n@0\nA=M\nM=-1\n(GT)\n@0\nAM=M+1\n") + no_of_int(logicjmp) * 4;
            char* Ar_Ll_Cmd = calloc(length,sizeof(char)); 
            sprintf(Ar_Ll_Cmd,"@0\nAM=M-1\nD=M\n@0\nAM=M-1\nD=M-D\n@LT%d\nD;JLT\n@0\nA=M\nM=0\n@GT%d\nD;JGT\n(LT%d)\n@0\nA=M\nM=-1\n(GT%d)\n@0\nAM=M+1\n",logicjmp,logicjmp,logicjmp,logicjmp);
            write(Dest_File_Desc,Ar_Ll_Cmd,length);
            free(Ar_Ll_Cmd);
            logicjmp++;
        }
        else if (!strcmp(command,"and"))
        {
            char* Ar_Ll_Cmd = "@0\nAM=M-1\nD=M\n@0\nAM=M-1\nM=M&D\n@0\nAM=M+1\n";
            int length = strlen(Ar_Ll_Cmd);
            write(Dest_File_Desc,Ar_Ll_Cmd,length);
        }
        else if (!strcmp(command,"neg"))
        {
            char* Ar_Ll_Cmd = "@0\nAM=M-1\nM=-M\n@0\nAM=M+1\n";
            int length = strlen(Ar_Ll_Cmd);
            write(Dest_File_Desc,Ar_Ll_Cmd,length);
        }
        else if (!strcmp(command,"eq"))
        {
            /* code */
        }
        else if (!strcmp(command,"not"))
        {
            char* Ar_Ll_Cmd = "@0\nAM=M-1\nM=!M\n@0\nAM=M+1\n";
            int length = strlen(Ar_Ll_Cmd);
            write(Dest_File_Desc,Ar_Ll_Cmd,length);
        }
        else
        {
            fprintf(stderr,"A unknown command was used please ensure no errors in file");
            return -1;
        }
    }
    printf("parsed\n");
    return 0;
}

int Push_Stack(int Asm_File_Desc,int Memory_Segment, char* Segement_Index)
{
    char* Instruct;
    int length;

    if (Memory_Segment > 0)
    {
    // get length of index plus 2(new line and @)chars
    length = strlen(Segement_Index) + 2;
    Instruct = calloc(length,sizeof(char));
    sprintf(Instruct,"@%s\n",Segement_Index);

    // write instruction into assembly file
    write(Asm_File_Desc,Instruct,length);

    // select the ram memory to be used
    free(Instruct);
    length = strlen("D=A\n@\nA=M+D\nD=M\n") + 2;
    Instruct = calloc(length, sizeof(char));
    sprintf(Instruct,"D=A\n@%d\nA=M+D\nD=M\n", Memory_Segment,length);
    write(Asm_File_Desc,Instruct,strlen(Instruct));
    free(Instruct);
    }
    else
    {
        // @index,D=A
        length = strlen("@\nD=A\n") + strlen(Segement_Index);
        Instruct = calloc(length, sizeof(char));
        sprintf(Instruct,"@%s\nD=A\n",Segement_Index);
        write(Asm_File_Desc,Instruct,strlen(Instruct));
        free(Instruct);
        
    }
    // manipulate the stack pointer and stack
    length = strlen("@\nAM=M+1\nA=A-1\nM=D\n") + 2;
    Instruct = calloc(length, sizeof(char));
    sprintf(Instruct,"@%d\nAM=M+1\nA=A-1\nM=D\n",STACK_P);
    write(Asm_File_Desc,Instruct,strlen(Instruct));
    free(Instruct);
}

int Pop_Stack(int Asm_File_Desc,int Memory_Segment, char* Segement_Index)
{
    char* Instruct;
    int length;

    // get the top most variable in the stack
    // manipulate the stack pointer and stack
    length = strlen("@\nAM=M-1\n") + 2;
    Instruct = calloc(length, sizeof(char));
    sprintf(Instruct,"@%d\nAM=M-1\n",STACK_P);
    write(Asm_File_Desc,Instruct,strlen(Instruct));
    free(Instruct);

    if (Memory_Segment > 0)
    {
    // get length of index plus 2(new line and @)chars
    length = strlen(Segement_Index) + 2;
    Instruct = calloc(length,sizeof(char));
    sprintf(Instruct,"@%s\n",Segement_Index);

    // write instruction into assembly file
    write(Asm_File_Desc,Instruct,length);

    // select the ram memory to be used
    free(Instruct);
    length = strlen("D=A\n@\nD=M+D\n@5\nM=D\n@0\nA=M\nD=M\n@5\nA=M\nM=D\n") + 2;
    Instruct = calloc(length, sizeof(char));
    sprintf(Instruct,"D=A\n@%d\nD=M+D\n@5\nM=D\n@0\nA=M\nD=M\n@5\nA=M\nM=D\n", Memory_Segment,length);
    write(Asm_File_Desc,Instruct,strlen(Instruct));
    free(Instruct);
    }
    else
    {
        // @index,D=A
        fprintf(stderr,"Cannot pop a constant from stack");
        
    }

}

char* strip(char* _string, int len)
{
    char* string = strdup(_string);
    // Left strip the string
    // iterate through the chars of the string comparing if it equals to the " "character
    printf("strippign string");
    int str_len = strlen(string);
    int i;
    for (i = 0;i < str_len;i++)
    {
        // char s = string +
        if (_string[i] == ' ')
        {
            continue;
        }
        else break;
    }
    printf("the resultant string is%d:%s\n",i,_string+i);

    //right strip the string 
    char* string2 = calloc(str_len - i,sizeof(char));
    strcpy(string2,_string + i);

    str_len = str_len - i;
    int span = str_len - 1;
    i = 0;
    printf("length is %d",str_len);
    for (span; span > 0; span--)
    {
        printf("%c",string2[span-1]);
        if (string2[span] == ' ')
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
    
    return string3;
}