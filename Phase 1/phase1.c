#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void breakupLine(char *input, char *command, char *param1, char *param2 , int *numParams);
void printArr(char *input);
void helpMenu();

int main()
{
	/* variables */
	char input[50];
	char command[10];
	char param1[10];
	char param2[10];
	int len = 0;
	int numParams;
	
	

	/* print the welcome message and print the command line */
	printf("Hello welcome to Jesus Morales Personal Assembler\n \n");

	/*set a loop that repeats the menu until the user chooses to exit*/
	while (1)
	{
		numParams = 0;

		/*take the input of the user*/
		printf("Command ------->  ");
		fgets(input, 50, stdin);


		/*Get read of the nextline character*/
		len = strlen(input) - 1;
		if (input[len] == '\n')
		{
			input[len] = ' ';
		}

	




	
		/*break up the line using a function with the input line and the parameters*/
		breakupLine(input, command, param1, param2, &numParams);

		
		

	
		/*conditions for each option the user whats to use*/

			/*load file_name*/
			if ( strcmp(command, "load") == 0 || strcmp(command,"LOAD") == 0)
			{
				if (numParams == 1)
				{
					printf("This function is not readily available, contact the system administrator. \n \n");
				}
				else
				{
					printf("You are missing one or more parameters for this function, type 'help' for more information \n \n");
				}
			}

			/*execute*/
			else if (strcmp(command, "execute") == 0 || strcmp(command, "EXECUTE") == 0)
			{
				printf("This function is not readily available, contact the system administrator. \n \n");
			}
			/*debug*/
			else if (strcmp(command, "debug") == 0 || strcmp(command, "DEBUG") == 0)
			{
			
				printf("This function is not readily available, contact the system administrator. \n \n");
			}

			/*dump start end*/
			else if (strcmp(command, "dump") == 0 || strcmp(command, "DUMP") == 0)
			{
				if (numParams == 2)
				{
					printf("This function is not readily available, contact the system administrator. \n \n");
				}
				else
				{
					printf("You are missing one or more parameters for this function, type 'help' for more information \n \n");
				}
				
			}

			/*help(working)*/
			else if (strcmp(command, "help") == 0 || strcmp(command, "HELP") == 0)
			{
				helpMenu();
			}

			/*assemble filename*/
			else if (strcmp(command, "assemble") == 0 || strcmp(command, "ASSEMBLE") == 0)
			{
				if (numParams == 1)
				{
					printf("This function is not readily available, contact the system administrator. \n \n");
				}
				else
				{
					printf("You are missing one or more parameters for this function, type 'help' for more information. \n \n");
				}
			}

			/*directory(working)*/
			else if (strcmp(command, "directory") == 0 || strcmp(command, "DIRECTORY") == 0 || strcmp(command, "dir") == 0 || strcmp(command, "DIR") == 0)
			{
				system("ls");
			}

			/*exit(working) */
			else if (strcmp(command, "exit") == 0 || strcmp(command, "EXIT") == 0)
			
			{
				break;
			}
			else
			{
				printf("Invalid Command , for any help type 'help' to display the command list. \n \n");
			}
			numParams = 0;
		

	}

	printf("thanks for using my compiler \n");




	return 0;

}

void breakupLine(char *input, char *command, char *param1, char *param2, int *numParams)
{
	
	int commandFlag = 1;
	int param1Flag = 0;
	int param2Flag = 0;
	int whitespaceFlag = 0;
	int j = 0;
	int count = 0;

	for (int i = 0; i < strlen(input); i++)
	{
		if (commandFlag == 1)
		{ 
			if (input[i] != ' ')
			{
				command[j] = input[i];
				j++;
				
			}
			else
			{
				commandFlag = 0;
				param1Flag = 1;
				command[j] = '\0';
				j = 0;
				
			}

		}
		else if (param1Flag == 1)
		{
			if (input[i] != ' ')
			{
				param1[j] = input[i];
				j++;
			}
			else
			{
				
				param1Flag = 0;
				param2Flag = 1;
				param1[j] = '\0';
				j = 0;
				count++;

				
			}
		}
		else if (param2Flag == 1)
		{
			if (input[i] != ' ')
			{
				param2[j] = input[i];
				j++;
			}
			else
			{
				param2Flag = 0;
				param2[j] = '\0';
				j = 0;
				count++;
				break;
				
			}
		}


	}
	*numParams = count;
}

void printArr(char *input)
{
	for (int i = 0; i < strlen(input); i++)
	{
		printf("%d: %c \n \n", i, input[i]);

	}
		/* printf("%s \n \n", input); */
	



}

void helpMenu()
{
	printf("\n");
	printf("Welcome to the Help menu. \n");
	printf("Commands are the following: \n \n");
	printf("load [file_name]\n");
	printf("execute \n");
	printf("debug \n");
	printf("dump [start] [end] \n");
	printf("help \n");
	printf("assemble [file_name] \n");
	printf("directory \n");
	printf("exit \n \n");

}
