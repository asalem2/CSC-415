/*Author: Eric Gonzalez
  Created: 3/6/15
  Shell Program
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define buffer 1024

int main(){
    
    int pid, status = 0;
    char path[buffer], *cmd, *arg[4];
    
    
    while(1)
    {
        
        int myarg = 0;
        int argc = 0;
        
        //print MyShell
        printf("Myshell>>");
        
        //receive the input string
        if(fgets(path, buffer, stdin) == NULL)
        {
            printf("fgets fail");
        }
        
        cmd = strtok(path, " \t\n");
        
        
        //hard code exit
        if((strcmp(cmd, "exit")) == 0)
            return 0;
        
        
        
        //turn it into seperate argument
        while(cmd != NULL)
        {
            arg[myarg] = cmd;
            cmd = strtok(NULL, " \t\n");
            myarg++;
        }
        
        
        //create pid fork for seperation
        pid = fork();
        
        if(pid == -1)
            printf("fork error");
        
        else if (pid > 0) {
            wait(&status);
        }
        
        else if (pid == 0)
        {
            switch(myarg)
            {
                    
                case 1:
                    execlp(arg[0], arg[0], NULL);
                case 2:
                    execlp(arg[0], arg[0], arg[1], NULL);
                case 3:
                    execlp(arg[0], arg[0], arg[1], arg[2], arg[3], NULL);
                case 4:
                    execlp(arg[0], arg[0], arg[1], arg[2], arg[3], arg[4], NULL);
                default:
                    printf("Error in switch\n");
            }
            
        }
    }
    
    return 0;
}
