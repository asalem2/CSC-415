//Assignment #7
//Created by Eric Gonzalez

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>

int main (){
	while(1){
		printf("Myshell> ");
		int buf_size = 2048;
		char buf[buf_size], bufcopy[buf_size], ibuf[buf_size];
		char *token, *tempv, del[2] = " \n";
		int background = 0, file = 0, err = 0, infile = 0, pip = 0;
		int tempc, myargc2 = 0, myargc1 = 0, pid1, pid2, status;
		int	pipeID[2];
		fgets(buf, buf_size, stdin);
		strncpy(bufcopy, buf, buf_size);
		token = strtok(buf, del);
		while(token != NULL){
			if(strcmp(token, "&") == 0){
				background = 1;
			}else if(strcmp(token, ">") == 0 || strcmp(token, "2>") == 0){
				if(strcmp(token, "2>") == 0){
					err = 1;
				}
				token = strtok(NULL, del);
				file = open(token, O_WRONLY|O_CREAT|O_TRUNC, 
					S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
				if(file == -1){
					fprintf(stderr, "Error in opening file\n");
					exit(0);
				}
			}else if(strcmp(token, ">>") == 0 || strcmp(token, "2>>") == 0){
				if(strcmp(token, "2>>") == 0){
					err = 1;
				}
				token = strtok(NULL, del);
				file = open(token, O_WRONLY|O_APPEND|O_CREAT, 
					S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
				if(file == -1){
					fprintf(stderr, "Error in opening file\n");
					exit(0);
				}
			}else if(strcmp(token, "<") == 0){
				token = strtok(NULL, del);
				infile = open(token, O_RDONLY, NULL);
				if(file == -1){
					fprintf(stderr, "Error in opening file\n");
					exit(0);
				}
			}else if(strcmp(token, "|") == 0){
				pip = 1;
				tempc = myargc1;
				myargc1 = 0;
			}else{
				myargc1++;
			}
			token = strtok(NULL, del);
		}

		if(myargc1 != 0){
			if(pip){
				myargc2 = myargc1;
				myargc1 = tempc;
			}
			char *myargv1[myargc1 + 1];
			char *myargv2[myargc2 + 1];
			int i = 0, j = 0;
			myargv1[i] = strtok(bufcopy, del);
			do{
				if(background){
					if(strcmp(myargv1[i], "&") == 0){
						myargv1[i] = strtok(NULL, del);
						break;
					}
				}
				if(file){
					if(strcmp(myargv1[i], ">") == 0 || strcmp(myargv1[i], "2>") == 0){
						strtok(NULL, del);
						myargv1[i] = strtok(NULL, del);
						break;
					}else if(strcmp(myargv1[i], ">>") == 0 ||strcmp(myargv1[i], "2>>") == 0){
						strtok(NULL, del);
						myargv1[i] = strtok(NULL, del);
						break;
					}
				}
				if(infile){
					if(strcmp(myargv1[i], "<") == 0){
						strtok(NULL, del);
						myargv1[i] = strtok(NULL, del);
						break;
					}
				}
				if(pip){
					if(strcmp(myargv1[i], "|") == 0){
						do{
							myargv2[j] = strtok(NULL, del);
						}while(myargv2[j++] != NULL);
						myargv1[i] = myargv2[--j];
						break;
					}
				}
				myargv1[++i] = strtok(NULL, del);
			}while(myargv1[i] != NULL);



			if(pip){
				if ((status = pipe(pipeID)) == -1) {
					perror("Bad pipe");
					exit(-1);
				}
				if ((pid1 = fork()) == -1) {
					perror("Bad fork");
					exit(-1);
				}
				if (pid1 == 0) {
					close(pipeID[1]);
					close(0);
					dup(pipeID[0]);
					if(execvp(myargv2[0], myargv2) < 0){
						if(strcmp(myargv2[0], "exit") != 0){
							fprintf(stderr, "Unknown command\n");
						}
						exit(1);
					}
					close(pipeID[0]);
				}
				if ((pid2 = fork()) == -1) {
					perror("Bad fork");
					exit(-1);
				}
				if (pid2 == 0) {
					close(pipeID[0]);
					close(1);
					dup(pipeID[1]);
					if(execvp(myargv1[0], myargv1) < 0){
						if(strcmp(myargv1[0], "exit") != 0){
							fprintf(stderr, "Unknown command\n");
						}
						exit(1);
					}
					close(pipeID[1]);
				}
				close(pipeID[0]);
				close(pipeID[1]);
				while(wait(&status) != pid2);
				while(wait(&status) != pid1);
			}
			else{
				if ((pid1 = fork()) == -1) {
					perror("Bad fork");
					exit(-1);
				}
				if(pid1 == 0) {
					//child proccess
					if(file){
						close(1);
						dup(file);
					}
					if(infile){
						close(0);
						dup(infile);
					}
					if(err){
						dup2(file,2);
					}
					if(execvp(myargv1[0], myargv1) < 0){
						if(strcmp(myargv1[0], "exit") != 0){
							fprintf(stderr, "Unknown command\n");
						}
						exit(1);
					}
					if(file || err){
						close(file);
					}
					if(infile){
						close(infile);
					}
				}else{
					if(!background){
						while(wait(&status) != pid1);
					}
				}
			}
			
			if(strcmp(myargv1[0], "exit") == 0){
				exit(0);
			}

		}
	}
	return 0;
}
