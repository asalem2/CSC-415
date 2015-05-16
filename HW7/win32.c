//Assignment #7
//Created by Eric Gonzalez

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <tchar.h>

#define BUFFER_SIZE 2048

int main (){
	//loop until user exits
	while(1){
		//prompt user input
		printf("Myshell> ");

		//initialize
		char buf[BUFFER_SIZE];
		char bufcopy[BUFFER_SIZE];
		char input[BUFFER_SIZE + 7];
		char input2[BUFFER_SIZE + 7];
		int size, i;
		int background = 0, pip = 0;
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		STARTUPINFO si2;
		PROCESS_INFORMATION pi2;
		HANDLE rhandle;
		HANDLE whandle;
		SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};
		ZeroMemory(&si, sizeof(si));
		ZeroMemory(&pi, sizeof(pi));
		si.cb = sizeof(si);
		ZeroMemory(&si2, sizeof(si2));
		ZeroMemory(&pi2, sizeof (pi2));
		si2.cb = sizeof(si2);
		//reset handles
		si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
        si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
        si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        si.dwFlags = STARTF_USESTDHANDLES;

		//get input
		fgets(buf, BUFFER_SIZE, stdin);
		strncpy(bufcopy, buf, BUFFER_SIZE);

		//parse buf and get first token
		char del[2] = " \n";
		char *token;
		token = strtok(buf, del);

		//check for NULL
		if(token != NULL){
			//check for exit
			if(strcmp(token, "exit") == 0){
				exit(0);
			}

			//fix buf into a proper input
			//flag if nessessary and open files
			//add prefix (cmd /c)
			strcpy(input, "cmd /c ");
			while(token != NULL){
				if(strcmp(token, "&") == 0){
					background = 1;
				}else if(strcmp(token, ">") == 0 || strcmp(token, "2>") == 0){
					if(strcmp(token, ">") == 0){
						token = strtok(NULL, del);
						if((si.hStdOutput = CreateFile(token, GENERIC_WRITE, 0, &sa, CREATE_ALWAYS,
							FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE){
							fprintf(stderr, "Error in creating output file\n");
							exit(0);
						}
					}else{
						token = strtok(NULL, del);
						if((si.hStdError = CreateFile(token, GENERIC_WRITE, 0, &sa, CREATE_ALWAYS,
							FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE){
							fprintf(stderr, "Error in creating output file\n");
							exit(0);
						}
					}
				}else if(strcmp(token, ">>") == 0 || strcmp(token, "2>>") == 0){
					if(strcmp(token, ">>") == 0){
						token = strtok(NULL, del);
						//create append file for output
						if((si.hStdOutput = CreateFile(token, FILE_APPEND_DATA, 0, &sa, OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE){
							if((si.hStdOutput = CreateFile(token, GENERIC_WRITE, 0, &sa, CREATE_ALWAYS,
								FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE){
								fprintf(stderr, "Error in creating output file\n");
								exit(0);
							}
						}
					}else{
						token = strtok(NULL, del);
						//create append file for output
						if((si.hStdError = CreateFile(token, FILE_APPEND_DATA, 0, &sa, OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE){
							si.hStdError = CreateFile(token, GENERIC_WRITE, 0, &sa, CREATE_ALWAYS,
								FILE_ATTRIBUTE_NORMAL, NULL);
						}

						if(si.hStdError == INVALID_HANDLE_VALUE){
							fprintf(stderr, "Error in creating output file\n");
							exit(0);
						}
					}
				}else if(strcmp(token, "<") == 0){
					token = strtok(NULL, del);
					//open file for input
					if((si.hStdInput = CreateFile(token, GENERIC_READ, 0, &sa, OPEN_ALWAYS,
						FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE){
						fprintf(stderr, "Error in opening input file\n");
						exit(0);
					}
				}else if(strcmp(token, "|") == 0){
					pip = 1;
					//get second command
					strcpy(input2, "cmd /c ");
					token = strtok(NULL, del);
					while(token != NULL){
						strcat(input2, token);
						strcat(input2, " ");
						token = strtok(NULL, del);
					}
					//fix suffix (replace last " " with \0)
					size = strlen(input2);
					input2[size - 1] = '\0';

				}else{
					strcat(input, token);
					strcat(input, " ");
				}
				token = strtok(NULL, del);
			}
			
			//fix suffix (replace last " " with \0)
			size = strlen(input);
			input[size - 1] = '\0';
			
			
			//create new process
			//Start the 2nd child process for pipe
			if(pip){
				//create pipe
				if (!CreatePipe(&rhandle, &whandle, &sa, 0)){
					fprintf(stderr, "Create Pipe Failed\n");
					exit(0);
				}
				si.hStdInput = rhandle;
				// Start the 1st child
				if(!CreateProcess(NULL, input2, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)){
					fprintf(stderr, "CreateProcess failed.\n");
					return -1;
				}
				//Start the 2nd child
				si2.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
				si2.hStdError = GetStdHandle(STD_ERROR_HANDLE);
				si2.hStdOutput = whandle;
				si2.dwFlags = STARTF_USESTDHANDLES;
				if(!CreateProcess(NULL, input, NULL, NULL, TRUE, 0, NULL, NULL, &si2, &pi2)){
					fprintf(stderr, "CreateProcess2 failed.\n");
					return -1;
				}
			}else{
				// Start the child process. 
				if(!CreateProcess(NULL, input, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)){
					fprintf(stderr, "CreateProcess failed.\n");
					return -1;
				}
			}

			// Wait until child process exits
			
			if(pip){
				WaitForSingleObject(pi2.hProcess, INFINITE);
				WaitForSingleObject(pi.hProcess, INFINITE);
			}else{
				if(!background){
					WaitForSingleObject(pi.hProcess, INFINITE);
				}
			}
			
			

			// Close process, thread handles and file
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
			if(si.hStdInput != GetStdHandle(STD_INPUT_HANDLE))CloseHandle(si.hStdInput);
			if(si.hStdError != GetStdHandle(STD_ERROR_HANDLE))CloseHandle(si.hStdError);
			if(si.hStdOutput != GetStdHandle(STD_OUTPUT_HANDLE))CloseHandle(si.hStdOutput);
			if(pip){
				CloseHandle(pi2.hProcess);
				CloseHandle(pi2.hThread);
				CloseHandle(rhandle);
				CloseHandle(whandle);
			}
		}
	}
	return 0;
}