/**
 * Simple shell interface program.
 *
 * Operating System Concepts - Ninth Edition
 * Copyright John Wiley & Sons - 2013
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAX_LINE 80 /* 80 chars per line, per command */

int userInput(char *str){
	char ch = '\0';
	int i = 0;

	/* input */
        while((ch = getchar()) != '\n' && ch != EOF && i < MAX_LINE + 1)
		str[i++] = ch;

	if(i >= MAX_LINE){
		printf("Command excceds maximum character(Max character = 80).\n");
		i = -1;
	}
	else
		str[i] = '\0';

	return i;
}
int argumentHandler(char **args, char *str){
	int i = 0;
	char *arr = strtok(str, " ");

	while(arr != '\0'){
		args[i++] = arr;
		arr = strtok('\0', " ");
	}
	if(i >= MAX_LINE / 2){
		printf("Argument excceds maximum (Maximum = 40).\n");
		return -1;
	}

	args[i] = '\0';
	return i;
}
int createChildProcess(char **args, int background){
	pid_t ret;

	ret = fork();
	if(ret == 0)
		execvp(args[0], args);
	else if (ret < 0) {
		printf("Failed to create childprocess.\n");
		return ret;
	}
	else{
		if(!background)
			waitpid(ret, NULL, 0);
		background = 0;
	}

	return 0;
}

int main(void)
{
	char *args[MAX_LINE/2 + 1];					/* command line (of 80) has max of 40 arguments */
	char memory[5][MAX_LINE + 1] = {'\0', };	/* Store command history */
	char str[MAX_LINE + 1];							/* user input */
	int should_run = 1;

	while (should_run){
	    int i = 0;
		int cntInput = 0;
		int cntArgs = 0;
		int background = 0;

		printf("osh>");
		fflush(stdout);

		/* input */
		cntInput = userInput(str);

		/* Command exceeds maximum */
		if(cntInput == -1)
			continue;

		/* Print most recent command in the history */
		if(!strcmp(str, "!!")){
			if(!strcmp(memory[(int)str[0] - '!'], ""))
				printf("Command not exists in history.\n");
			else
				strcpy(str, memory[(int)str[0] - '!']);
		}

		/* Print N'th command in the history */
		if(str[0] == '!'){
			switch(str[1]){
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
					  if(!strcmp(memory[(int)str[1] - '1'], "")) printf("Command not exists in history.\n");
					  else strcpy(str, memory[(int)str[1] - '1']);
					  break;
				default :
					  break;
			}
		}

        /* Store command in Memory*/
        for(int i = 4; i > 0; i--)
			strcpy(memory[i], memory[i - 1]);
		strcpy(memory[0], str);

        /* history command */
         if(!strcmp(str, "history")){
			 for(int i = 5; i > 0; i--){
				 if(!strcmp(memory[i - 1], ""))
					 continue;
				 else
					 printf("%d %s\n", i, memory[i - 1]);
			 }
			 continue;
		 }

		 /* Argument Handling */
		 cntArgs = argumentHandler(args, str);
		 if(cntArgs == -1)
			 continue;

		 /* exit command */
		 if(!strcmp(args[0], "exit")){
			 return 0;
		 }

		 if(!strcmp(args[cntArgs - 1], "&")){
			 background = 1;
			 args[cntArgs - 1] = '\0';
		 }

		 createChildProcess(args, background);

        /**
         * After reading user input, the steps are:
         * (1) fork a child process
         * (2) the child process will invoke execvp()
         * (3) if command included &, parent will invoke wait()
         */
	}
	return 0;
}
