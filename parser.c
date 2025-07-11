#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<errno.h>
#include<error.h>
#include<time.h>
#include<stdlib.h>
#include"stack_ini.h"

//TODO add support for file path


int main(int argc,char* argv[])
{
    clock_t start, end;
    double time_spent;
    start = clock();

    char* File_Name;
    char* Dest_File_Name;
    
    //define file extensions of the source and destination files
    char* Src_FExt = "vm";
    char* Dest_FExt = "asm";

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
            char* File_Ext = File_Extension(File_Name);
            printf("%s\n", File_Ext);
            if ( !strcmp(File_Ext,Src_FExt) )
            {
                //validate the destination file if it exists
                //destination file not defined
                if (argc < 3)
                {
                    //TODO also check if a file already exists with the same name
                    Dest_File_Name = File_Preffix(File_Name);
                    printf("this is the file preffix %s\n",Dest_File_Name);
                    //create output file ith the same prefix as the source file
                    (Dest_File_Name,strlen(Dest_File_Name) + strlen(Dest_FExt) - 1);
                    strcat(Dest_File_Name,Dest_FExt);
                    printf("%s",Dest_File_Name);
                    int fd = open(Dest_File_Name,O_CREAT | O_TRUNC | O_RDWR);
                    if (fd < 0)
                    {
                        fprintf(stderr,"An error has occured when creating assembly file: %s",strerror(errno));
                        return fd;
                    }
                    close(fd);
                    // confirm that file has been created successfully
                }
                else
                {
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

    //printf("%s", File_Name);'
    int Source_File = open(File_Name,O_RDONLY);
    int Destination_File = open(Dest_File_Name,O_WRONLY);
    if (Source_File > 0 && Dest_File_Name > 0)
    {
        //do operation since file has been opened successfully
        char* line = readline(Source_File);
        // printf("This is the function return: %s\n",line);
        char** cmds;// = Cmd_Segregate(line);

        while (line)
        {
            cmds = Cmd_Segregate(line);
            Parse_File(Destination_File, cmds);
            
            line = readline(Source_File);
            printf("This is the function return:... %s\n",line);
            // printf("This is the function return: %s\n",line);
            // cmds = Cmd_Segregate(line);
            // Parse_File(Destination_File, cmds);

            // line = readline(Source_File);
            // printf("This is the function return: %s\n",line);
            // cmds = Cmd_Segregate(line);
            // Parse_File(Destination_File, cmds);

            // line = readline(Source_File);
            // printf("This is the function return: %s\n",line);
            // cmds = Cmd_Segregate(line);
            // Parse_File(Destination_File, cmds);
        }
        // printf("hrere");
        //printf("This is the function return: %s\n",cmds[0]);

        // int wr = Parse_File(Destination_File,cmds);
        printf("closing files\n");
        // int RD =access(Dest_File_Name,W_OK);
        // printf("%d %s\n",RD, strerror(errno));
        // chmod(Dest_File_Name,666);
        // write(Destination_File,Line,20);
        //close file after compiling
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
    printf("\nExecution time: %f seconds\n", time_spent);
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
    printf("%d %d\n",filename_len,prefix_len);
     char* prefix = malloc(prefix_len);

     strncpy(prefix,File_name,prefix_len);
    prefix[prefix_len] = '\0';
     return prefix;
}

char* readline(int File_Desc)
{
    
    char* Read_Buf = calloc(25,sizeof(char));
    int rd = read(File_Desc,Read_Buf,25);
    if (!rd)
    {
        printf("error\n");
        return NULL;
    }
    //check for the first occurence of new line character in the read line(s)
    char* Line_Noise = strchr(Read_Buf,'\n');
    int Size_of_line;
    int pos;
    //since the result of Line noise also includes the new line character we minus 1 from the supposed length
    if (Line_Noise ==NULL && strcmp(Read_Buf," "))
    {
        // Line_Noise = ;
        Size_of_line = strlen(Read_Buf);
        // int pos = lseek(File_Desc,-strlen(Line_Noise)+2,SEEK_CUR);
        
    }
    else
    {
        Size_of_line = strlen(Read_Buf) - strlen(Line_Noise);
        int pos = lseek(File_Desc,-strlen(Line_Noise)+2,SEEK_CUR);
        
    }
    
    char* Read_Line = calloc(Size_of_line,sizeof(char));
    
    //copy contents of line to the read_line
    strncpy(Read_Line,Read_Buf,Size_of_line);
    printf("%s;%s:%s:%d\n",Read_Buf,Line_Noise,Read_Line,Size_of_line);

    //set the file cursor to the next line start position
    // printf("This is the position of the cursor: %d\n",pos);
    free(Read_Buf);
    return Read_Line;
}

char** Cmd_Segregate(char* instructions)
{   //TODO raise error if the coomands are more than three
    // iterate through the string separating its contents using " " as separator
    char** cmd_array = calloc(strlen(instructions),sizeof(char));
    printf("INSTRUCT %s\n",instructions);
    int count =0;
    char *commands = strdup(instructions);
    while(commands)
    {
        // printf("starting at %s and count is %d\n",commands,count);
        
        char* commands_ext = strchr(commands,' ');
        
        if (commands_ext == NULL && strcmp(commands," "))
        {
            cmd_array[count] = commands;
            printf("last command is %s",commands);
            // count++;
            
            break;
        }
        
        //get length of commands
        int cmd_len = strlen(commands) - strlen(commands_ext);
        char* command = calloc(cmd_len + 1,sizeof(char));
        char* cpy = strncpy(command,commands,sizeof(char) * cmd_len);
        printf("cycle return is %s\n",cpy);
        printf("%d %s:%s\n",cmd_len,command,commands_ext+1);
        cmd_array[count] = command;
        
        char* temp = strdup(commands_ext + 1);
        // printf("command computed is: %s\n",cmd_array[count]);
        // printf("final comp:%s\n",temp);
        free(commands);
        commands = calloc(strlen(temp),sizeof(char));
        commands = temp;
        count++;
    }
    printf("computed %d commands\n",count);
    // int i;
    // for (i = 0;i < count; i++){
        printf("commands computed are %s\n",cmd_array[0]);
        printf("commands computed are %s\n",cmd_array[1]);
        printf("commands computed are %s\n",cmd_array[2]);
    // }
    return cmd_array;
}

int Parse_File(int Dest_File_Desc, char** Command_Array)
{
    // decontruct first element to get type of command
    printf("%d",strcmp("push","push"));
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
                printf("contant %s\n",mem_segment_number);
                Push_Stack(Dest_File_Desc,STACK_P,mem_segment_number);
            }
            else if (!strcmp(memory_segment,"static"))
            {
                printf("contant %s\n",mem_segment_number);
                Push_Stack(Dest_File_Desc,STATIC_P,mem_segment_number);
            }
            else if (!strcmp(memory_segment,"this"))
            {
                printf("contant %s\n",mem_segment_number);
                Push_Stack(Dest_File_Desc,THIS_P,mem_segment_number);
            }
            else if (!strcmp(memory_segment,"that"))
            {
                printf("contant %s\n",mem_segment_number);
                Push_Stack(Dest_File_Desc,THAT_P,mem_segment_number);
            }
            else if (!strcmp(memory_segment,"pointer"))
            {
                printf("contant %s\n",mem_segment_number);
                Push_Stack(Dest_File_Desc,POINTER_P,mem_segment_number);
            }
            else if (!strcmp(memory_segment,"temp"))
            {
                printf("contant %s\n",mem_segment_number);
                Push_Stack(Dest_File_Desc,TEMP_P,mem_segment_number);
            }
            
            // write(Dest_File_Desc);
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
                printf("contant %s\n",mem_segment_number);
                Pop_Stack(Dest_File_Desc,STACK_P,mem_segment_number);
            }
            else if (!strcmp(memory_segment,"static"))
            {
                printf("contant %s\n",mem_segment_number);
                Pop_Stack(Dest_File_Desc,STATIC_P,mem_segment_number);
            }
            else if (!strcmp(memory_segment,"this"))
            {
                printf("contant %s\n",mem_segment_number);
                Pop_Stack(Dest_File_Desc,THIS_P,mem_segment_number);
            }
            else if (!strcmp(memory_segment,"that"))
            {
                printf("contant %s\n",mem_segment_number);
                Pop_Stack(Dest_File_Desc,THAT_P,mem_segment_number);
            }
            else if (!strcmp(memory_segment,"pointer"))
            {
                printf("contant %s\n",mem_segment_number);
                Pop_Stack(Dest_File_Desc,POINTER_P,mem_segment_number);
            }
            else if (!strcmp(memory_segment,"temp"))
            {
                printf("contant %s\n",mem_segment_number);
                Pop_Stack(Dest_File_Desc,TEMP_P,mem_segment_number);
            }
            
        }
    
    }
    else
    {

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
    Instruct = calloc(3,sizeof(char));
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

