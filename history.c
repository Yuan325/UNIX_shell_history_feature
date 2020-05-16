#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_LINE 80 /* The maximum length command */

struct command *head = NULL;
char runCommand[MAX_LINE];
int count = 0;

/* linked list struct */
struct command{
	struct command *next;
	int sequence;
	char *command;
};

/* add another command into linked list */
void add_history(char *paramCommand){
	struct command *link = (struct command*) malloc(sizeof(struct command));

	count++;
	link->command = paramCommand;
	link->sequence = count;

	link->next= head;
	head = link;
	
}

/* print the recent 10 commands */
void print_history(){
	int countPrint = 0;
	struct command *ptr = head;
	while (ptr != NULL && countPrint<10){
		printf("%d, %s\n", ptr->sequence, ptr->command);
		ptr = ptr -> next;
		countPrint++;
	}
	printf("\n");
}

/* get a specific command */
struct command* get_history(int param){
	struct command *current = head;
	if(count-10>=param){
		printf("No commands in history\n");
		runCommand[0]='\0';
		return NULL;
	}
	if(head == NULL){
		printf("No commands in history\n");
		runCommand[0]='\0';
		return NULL;
	}
	while(current->sequence != param){
		if (current->next == NULL){
			printf("No commands in history\n");
			runCommand[0]='\0';
			return NULL;
		}
		else{
			current = current->next;
			strcpy(runCommand, current -> command);
		}
	}

	return current;
}

int main(void)
{
        char *args[MAX_LINE/2 + 1]; /* command line arguments */
        int should_run = 1; /* flag to determine when to exit program */
        int i = 0;
        bool x;
        int j;
	char ss[10][MAX_LINE];
	int countss = 0;
	char tempss[MAX_LINE];
	char *number;
	int temp;	
	
        while (should_run){

                x = true;
                printf("osh>");
                fflush(stdout);
		
		if(countss == 10){
			countss = 0;
		}	
		
		strcpy(tempss, ss[countss]);
		scanf("%[^\n]%*c", ss[countss]);
		
		/* if user enter history */
		if (strcmp(ss[countss],"history")==0){
			strcpy(ss[countss],tempss);
			if(count == 0){
				printf("No commands in history\n");
				runCommand[0]='\0';
			}
			else{
				print_history();
			}
		}
		else {
			/* if user enters something that starts with ! */
			if (ss[countss][0] == '!'){
				number = strtok(ss[countss],"!");
				if (ss[countss][1] == '!'){
					strcpy(ss[countss], tempss);
					if(count == 0){
						printf("No commands in history\n");
						runCommand[0]='\0';
					}
					else{
						get_history(count);
					}
				}
				else if (number <= 0){
					strcpy(ss[countss], tempss);
					printf("No commands in history\n");
					runCommand[0]='\0';
				}
				else{
					temp = atoi(number);
					strcpy(ss[countss], tempss);
					get_history(temp);
				}
			}
			else{
				/* add the command into history linked list */
				strcpy(runCommand, ss[countss]);
				add_history(ss[countss]);
				countss++;
			}
			
			/* break command to token */
			args[i] = strtok(runCommand, " ");
	                for (i = 1; args[i] != NULL ; i++){
	                        args[i] = strtok(NULL, " ");
	                }

	                for (i = 0; args[i] != NULL; i++){
	                }
		
			/* fork a child process */
	                pid_t pid;
       		        pid = fork();
			
			/* if & is included in command, x becomes false and parent don't wait for child process */
 	               if (strcmp(args[i-1],"&") == 0){
	                        x = false;
	                }

	                if(pid<0){
	                        fprintf(stderr, "Fork Failed\n");
	                        return 1;
	                }
	                else if (pid == 0){ /* child */
	                        if (x == false){
	                                args[i-1] = NULL;
	                        }
	                        execvp(args[0], args);
	                        for (j = 0; j<i; j++){
	                                args[j] = NULL;
	                        }
	                }
	                else{	/* parent */
	     	        	/* if command does not include &, parent will invoke wait() */
	                        if (x == true){
                                	wait(NULL);
        	                }
			}

		}
		
        }
        return 0;
}
