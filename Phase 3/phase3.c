#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
System's Programming Phase 3 Algorithm

Author: Jesus M. Morales
Due Date: 3/25/2018


*/

typedef struct
{
	char label[10];
	int memoryAddress;
}LABELS;

typedef struct
{	
	int memoryAddress;
	char *label;
	char *mnemonic;
	char *opcode;
	int errorCode;
} TOKEN;

typedef struct
{
	char mnemonic[5];
	char opcode[5];
} OPCODE;

void breakupLine(char *input, char *command, char *param1, char *param2, int *numParams);
void loadFile(char *fileName);
void passOne(char * fileName);
void passTwo();
int searchLabelLocation(char *);
char *searchOpcodeTable(char *);
void executeFile();
void debugFile();
void dumpFile();
void helpFile();
void assembleFile();
void errorFile();
int programLenght;
int errorFound;
int numLabels;
int numMnemonics = 25;
LABELS labelStructure[500];
OPCODE opcodeStructure[] = { { "ADD", "18" },{ "AND","58" },{ "COMP", "28" },{ "DIV", "24" },
{ "J", "3C" },{ "JEQ", "30" },{ "JGT", "34" },{ "JLT", "38" },
{ "JSUB", "48" },{ "LDA", "00" },{ "LDCH", "50" },{ "LDL", "08" },
{ "LDX", "04" },{ "MUL", "20" },{ "OR", "44" },{ "RD", "D8" },
{ "RSUB", "4C" },{ "STA", "0C" },{ "STCH", "54" },{ "STL", "14" },
{ "STX", "10" },{ "SUB", "1C" },{ "TD", "E0" },{ "TIX", "2C" },{ "WD", "DC" } };


int main(void)
{
	char input[50];
	char command[50];
	char param1[50];
	char param2[50];

	printf("Hello welcome to Jesus Morales Personal Assembler\n \n");
	while (1)
	{
		int numParams = 0;
		int len = 0;
		printf("Command ----> ");
		fgets(input, 50, stdin);

		len = strlen(input) - 1;
		if (input[len] == '\n')
		{
			input[len] = '\0';
		}

		breakupLine(input, command, param1, param2, &numParams);
		numParams--;

		if (strcmp(command, "load") == 0)
		{
			if (numParams == 1)
			{
				loadFile(param1);
			}
			else
				errorFile();
		}
		else if (strcmp(command, "execute") == 0)
		{
			executeFile();
		}
		else if (strcmp(command, "debug") == 0)
		{
			debugFile();
		}
		else if (strcmp(command, "dump") == 0)
		{
			if (numParams == 2)
			{
				dumpFile();
			}
			else
				errorFile();
		}
		else if (strcmp(command, "help") == 0)
		{
			helpFile();
		}
		else if (strcmp(command, "assemble") == 0)
		{
			if (numParams == 1)
			{
				assembleFile();
			}
			else
				errorFile();
		}
		else if (strcmp(command, "dir") == 0)
		{
			system("dir");
		}
		else if (strcmp(command, "exit") == 0)
		{
			break;
		}
		else
		{
			printf("Invalid Command , for any help type 'help' to display the command list. \n \n");
		}

		numParams = 0;
	}
	return 0;
}

void breakupLine(char *input, char *command, char *param1, char *param2, int *numParams)
{
	command[0] = param1[0] = param2[0] = '\0';
	*numParams = sscanf(input, "%s %s %s %*s", command, param1, param2);
}
void loadFile(char *param1)
{
	printf("Loading file:  %s\n", param1);
	passOne(param1);
	passTwo();
	printf("The Programg lenght of this file is:   %d Bytes\n\n", programLenght);
	programLenght = 0;

}
void executeFile()
{
	printf(" is not yet avaibalbe.\n");
}
void debugFile()
{
	printf("debug is not avaialabe.\n");
}
void dumpFile()
{
	printf("dump is not avaiblable.\n");
}
void helpFile()
{
	printf("\n");
	printf("\tWelcome to the Help menu. \n");
	printf("\tCommands are the following: \n \n");
	printf("\tload [file_name]\n");
	printf("\texecute \n");
	printf("\tdebug \n");
	printf("\tdump [start] [end] \n");
	printf("\thelp \n");
	printf("\tassemble [file_name] \n");
	printf("\tdirectory \n");
	printf("\texit \n\n");
	printf("\t**ALL COMMANDS ARE CASE SENSITIVE.**\n\n");
}
void assembleFile()
{
	printf("assemble not avaibalbe. \n");
}
void errorFile()
{
	printf("You typed the wrong number of parameters try again. \n");
}
void passOne(char *param1)
{
	char input[500];
	char *tokenizer = input;

	char *startingLoct;
	int start = 0;
	int locctr = 0;
	int memLenght = 0;
	
	int index = 0;

	int labelPresentFlag = 0;
	int duplicateLabelFlag = 0;
	int illegalLabelFlag = 0;
	int illegalOperationFlag = 0;
	int missingDataDirectiveFlag = 0;
	int missingStartFlag = 0;
	int missingEndFlag = 0;
	int tooManyLabelsFlag = 0;
	int programTooLongFlag = 0;
	int errorCode = 0;

	FILE *source_file, *symbol_file, *intermediate_file, *opcode_file;
	TOKEN sourceFileTokenizer;
	

	source_file = fopen(param1, "r");
	intermediate_file = fopen("intermediate.txt", "w");
	symbol_file = fopen("symbolTable.txt", "w");

	if (source_file == NULL)
	{
		printf("Error openning file does not exist:	%s\n", param1);
		return;
	}

	sourceFileTokenizer.label = (char *)malloc(6);
	sourceFileTokenizer.mnemonic = (char *)malloc(6);
	sourceFileTokenizer.opcode = (char *)malloc(6);

	errorFound = 0;
	numLabels = 0;
	while (fgets(input, 500, source_file))
	{
		labelPresentFlag = 0;
		duplicateLabelFlag = 0;
		illegalLabelFlag = 0;
		illegalOperationFlag = 0;
		missingDataDirectiveFlag = 0;
		missingStartFlag = 0;
		missingEndFlag = 0;
		tooManyLabelsFlag = 0;
		programTooLongFlag = 0;
		errorCode = 0;
		memLenght = 0;

		/*	Check if label is present in the string line	*/
		if (input[0] == ' ' || input[0] == '\t')
		{
			labelPresentFlag = 0;
		}
		else
		{
			labelPresentFlag = 1;
		}

		/*	Check if comment is present in the string line	*/
		if (input[0] == '.')
		{
			continue;
		}

		/*	Tokenize the input string	*/
		tokenizer = strtok(input, " \t\r\n\v\f");

		/*	Remove of the trailing newLine at the end of the string	*/
		int counter = 0;
		while (input[counter - 1] != '\n')
		{
			counter++;
		}
		input[counter] = '\0';

		/*	If there is a label */
		if (labelPresentFlag == 1)
		{
			/*	Tokenize the label into the structure	*/
			strcpy(sourceFileTokenizer.label, tokenizer);

			/*	Tokenize the mnemonic into the structure	*/
			tokenizer = strtok(NULL, " \t\r\n\v\f");
			strcpy(sourceFileTokenizer.mnemonic, tokenizer);

			/*	Tokenize the opcode of the mnemonic into the structure	*/
			tokenizer = strtok(NULL, " \t\r\n\v\f");
			strcpy(sourceFileTokenizer.opcode, tokenizer);

			/*	Add the labels to the structure to create a list of existing labels/symbols */
			strcpy(labelStructure[numLabels].label, sourceFileTokenizer.label);
			labelStructure[numLabels].memoryAddress = locctr;

			/*	Check if there are labels in the list	*/
			if (numLabels > 0)
			{
				/*	Check if limit of labels has been reached	*/
				if (numLabels > 500)
				{
					tooManyLabelsFlag = 1;
				}

				/*	Inefficiently scan the label/symbol list to check for duplicate labels/symbols	 */
				for (int i = 0; i < numLabels; i++)
				{
					if (strcmp(labelStructure[i].label, sourceFileTokenizer.label) == 0)
					{
						duplicateLabelFlag = 1;
					}
				}
			}

			/*	Check if the label is legal	*/
			if (!isalpha(sourceFileTokenizer.label[0]))
			{
				illegalLabelFlag = 1;
			}

			/*	Check if we have a START directive in the beginning of the program	*/
			if (index == 0 && strcmp(sourceFileTokenizer.mnemonic, "START") != 0)
			{
				missingStartFlag = 1;
				locctr = 0;
			}

			/*	Check if we have a END directive in the end of the program	*/
			if (missingEndFlag == 1 && errorCode == 0)
			{
				if (strcmp(sourceFileTokenizer.mnemonic, "END") != 0)
				{
					missingEndFlag = 1;
				}
			}

			/*	If directive START initialize LOCCTR to the starting address	*/
			if (strcmp(sourceFileTokenizer.mnemonic, "START") == 0) // if start directive initialize locct to the start(convert the string to integer)
			{

				startingLoct = sourceFileTokenizer.opcode;
				start = (int)strtol(startingLoct, NULL, 16);
				locctr = start;
			}

			/*	Check if program is too long	*/
			if (locctr > 32000)
			{
				programTooLongFlag = 1;
			}
			/* Lenght size in memory from the directives to increment the LOCCTR	*/
			if (strcmp(sourceFileTokenizer.mnemonic, "WORD") == 0)
			{
				memLenght += 3;
			}
			if (strcmp(sourceFileTokenizer.mnemonic, "RESB") == 0)
			{
				memLenght += (int)strtol(sourceFileTokenizer.opcode, NULL, 10);

			}
			if (strcmp(sourceFileTokenizer.mnemonic, "RESW") == 0)
			{
				memLenght += 3 * (int)strtol(sourceFileTokenizer.opcode, NULL, 10);
			}

			if (strcmp(sourceFileTokenizer.mnemonic, "BYTE") == 0)
			{
				/*	Check if operand is set to read a string (C) or a hexadecimal (X)	*/
				if (sourceFileTokenizer.opcode[0] == 'C')
				{

					int bufferSpace = 0;
					int counter = 2;
					while (sourceFileTokenizer.opcode[counter] != '\'' && bufferSpace < 30)
					{
						bufferSpace++;
						counter++;
					}

					memLenght += bufferSpace;

				}
				else if (sourceFileTokenizer.opcode[0] == 'X')
				{
					char hexInput[10];
					int bufferSpace = 0;
					int counter = 3;
					while (sourceFileTokenizer.opcode[counter] != '\'' && bufferSpace < 10)
					{
						hexInput[bufferSpace] = sourceFileTokenizer.opcode[counter];
						bufferSpace++;
						counter++;
					}

					memLenght = (int)strtol(hexInput, NULL, 10);
				}

				/* Check for errors in the input for the BYTE directive	*/
				else
				{
					illegalOperationFlag = 1;
				}

				if (sourceFileTokenizer.opcode[1] != '\'' || sourceFileTokenizer.opcode[strlen(sourceFileTokenizer.opcode) - 1] != '\'')
				{
					missingDataDirectiveFlag = 1;
				}
			}

			/* Error Flag conditions	*/
			if (duplicateLabelFlag == 1 && errorCode == 0)
			{
				errorCode = 1;
				errorFound = 1;
			}
			else if (illegalLabelFlag == 1 && errorCode == 0)
			{
				errorCode = 2;
				errorFound = 1;
			}
			else if (illegalOperationFlag == 1 && errorCode == 0)
			{
				errorCode = 3;
				errorFound = 1;
			}
			else if (missingDataDirectiveFlag == 1 && errorCode == 0)
			{
				errorCode = 4;
				errorFound = 1;
			}
			else if (missingStartFlag == 1 && errorCode == 0)
			{
				errorCode = 5;
				errorFound = 1;
			}
			else if (missingEndFlag == 1 && errorCode == 0)
			{
				errorCode = 6;
				errorFound = 1;
			}
			else if (tooManyLabelsFlag == 1 && errorCode == 0)
			{
				errorCode = 7;
				errorFound = 1;
			}
			else if (programTooLongFlag == 1 && errorCode == 0)
			{
				errorCode = 8;
				errorFound = 1;
			}

			/*Print to the intermediate file and symbol file */
			fprintf(intermediate_file, "%X\t%s\t%s\t%s\t%d\n", locctr, sourceFileTokenizer.label, sourceFileTokenizer.mnemonic, sourceFileTokenizer.opcode, errorCode);
			fprintf(symbol_file, "%X\t%s\n", locctr, sourceFileTokenizer.label);

			/* Search for the mnemonic in the opcode table and add 3 to it	*/
			for (int i = 0; i < numMnemonics; i++)
			{
				if (strcmp(opcodeStructure[i].mnemonic, sourceFileTokenizer.mnemonic) == 0)
				{
					locctr += 3;
				}
			}

			/*Update the memory locations after LOCCTR is printed in the file	*/
			if (strcmp(sourceFileTokenizer.mnemonic, "BYTE") == 0 || strcmp(sourceFileTokenizer.mnemonic, "RESB") == 0 || strcmp(sourceFileTokenizer.mnemonic, "RESW") == 0 || strcmp(sourceFileTokenizer.mnemonic, "WORD") == 0)
			{
				locctr += memLenght;
			}

			/*	Increment the number of labels in the system	*/
			numLabels++;
		}

		/*If there is no label in the input line do the same as above but without labels */
		else
		{
			/*	Tokenize the mnemonic into the structure	*/
			strcpy(sourceFileTokenizer.mnemonic, tokenizer);

			if (strcmp(sourceFileTokenizer.mnemonic, "RSUB") != 0)
			{
				tokenizer = strtok(NULL, " \t\r\n\v\f");
				strcpy(sourceFileTokenizer.opcode, tokenizer);

			}
			else
			{
				strcpy(sourceFileTokenizer.label, " ");
				strcpy(sourceFileTokenizer.opcode, " ");
			}
				

			/*	Tokenize the opcode into the structure	*/
			

			/*	Check if we have a START directive in the beginning of the program	*/
			if (index == 0 && strcmp(sourceFileTokenizer.mnemonic, "START") != 0)
			{
				missingStartFlag = 1;
				locctr = 0;
			}

			/*	Check if we have a END directive in the end of the program	*/
			if (missingEndFlag == 1 && errorCode == 0)
			{
				if (strcmp(sourceFileTokenizer.mnemonic, "END") != 0)
				{
					missingEndFlag = 1;
				}
			}

			/*	If directive START initialize LOCCTR to the starting address	*/
			if (strcmp(sourceFileTokenizer.mnemonic, "START") == 0) // if start directive initialize locct to the start(convert the string to integer)
			{
				start = atoi(sourceFileTokenizer.opcode);
				locctr = start;
			}

			/*	Check if program is too long	*/
			if (locctr > 32000)
			{
				programTooLongFlag = 1;
			}

			/* Lenght size in memory from the directives to increment the LOCCTR	*/
			if (strcmp(sourceFileTokenizer.mnemonic, "WORD") == 0)
			{
				memLenght += 3;
			}
			if (strcmp(sourceFileTokenizer.mnemonic, "RESB") == 0)
			{
				memLenght += (int)strtol(sourceFileTokenizer.opcode, NULL, 10);

			}
			if (strcmp(sourceFileTokenizer.mnemonic, "RESW") == 0)
			{
				memLenght += 3 * (int)strtol(sourceFileTokenizer.opcode, NULL, 10);
			}

			if (strcmp(sourceFileTokenizer.mnemonic, "BYTE") == 0)
			{
				/*	Check if operand is set to read a string (C) or a hexadecimal (X)	*/
				if (sourceFileTokenizer.opcode[0] == 'C')
				{
					int bufferSpace = 0;
					int counter = 2;
					while (sourceFileTokenizer.opcode[counter] != '\'' && bufferSpace < 30)
					{
						bufferSpace++;
						counter++;
					}
					memLenght += bufferSpace;

				}
				else if (sourceFileTokenizer.opcode[0] == 'X')
				{
					char hexInput[16];
					int bufferSpace = 0;
					int counter = 3;
					while (sourceFileTokenizer.opcode[counter] != '\'' && bufferSpace < 16)
					{
						hexInput[bufferSpace] = sourceFileTokenizer.opcode[counter];
						bufferSpace++;
						counter++;
					}
					
					memLenght = (int)strtol(hexInput, NULL, 10);
				}

				/* Check for errors in the input for the BYTE directive	*/
				else
				{
					illegalOperationFlag = 1;
				}

				if (sourceFileTokenizer.opcode[1] != '\'' || sourceFileTokenizer.opcode[strlen(sourceFileTokenizer.opcode) - 1] != '\'')
				{
					missingDataDirectiveFlag = 1;
				}
			}

			/* Error Flag conditions	*/
			if (duplicateLabelFlag == 1 && errorCode == 0)
			{
				errorCode = 1;
				errorFound = 1;
			}
			else if (illegalLabelFlag == 1 && errorCode == 0)
			{
				errorCode = 2;
				errorFound = 1;
			}
			else if (illegalOperationFlag == 1 && errorCode == 0)
			{
				errorCode = 3;
				errorFound = 1;
			}
			else if (missingDataDirectiveFlag == 1 && errorCode == 0)
			{
				errorCode = 4;
				errorFound = 1;
			}
			else if (missingStartFlag == 1 && errorCode == 0)
			{
				errorCode = 5;
				errorFound = 1;
			}
			else if (missingEndFlag == 1 && errorCode == 0)
			{
				errorCode = 6;
				errorFound = 1;
			}
			else if (tooManyLabelsFlag == 1 && errorCode == 0)
			{
				errorCode = 7;
				errorFound = 1;
			}
			else if (programTooLongFlag == 1 && errorCode == 0)
			{
				errorCode = 8;
				errorFound = 1;
			}

			/*Print to the intermediate file and symbol file */
			fprintf(intermediate_file, "%X\t\t\t%s\t%s\t%d\n", locctr, sourceFileTokenizer.mnemonic, sourceFileTokenizer.opcode, errorCode);

			/* Search for the mnemonic in the opcode table and add 3 to it	*/
			for (int i = 0; i < numMnemonics; i++)
			{
				if (strcmp(opcodeStructure[i].mnemonic, sourceFileTokenizer.mnemonic) == 0)
				{
					locctr += 3;
				}
			}

			/*Update the memory locations after LOCCTR is printed in the file	*/
			if (strcmp(sourceFileTokenizer.mnemonic, "BYTE") == 0 || strcmp(sourceFileTokenizer.mnemonic, "RESB") == 0 || strcmp(sourceFileTokenizer.mnemonic, "RESW") == 0 || strcmp(sourceFileTokenizer.mnemonic, "WORD") == 0)
			{
				locctr += memLenght;
			}
		}
		index++;
	}

	programLenght = locctr - start;
	programLenght = programLenght - 4;
	
printf("Pass One complete successfully. \n");

fprintf(intermediate_file, "\n\n\t Printing Error Code List: \n\n");
fprintf(intermediate_file, "*=================================================*\n");
fprintf(intermediate_file, "\tNo Error = 0\n");
fprintf(intermediate_file, "\tDuplicate Label = 1\n");
fprintf(intermediate_file, "\tIllegal Label = 2\n");
fprintf(intermediate_file, "\tIllegal Operation = 3\n");
fprintf(intermediate_file, "\tIllegal Data Storage Directive = 4\n");
fprintf(intermediate_file, "\tMissing START Directive = 5\n");
fprintf(intermediate_file, "\tMissing END Directive = 6\n");
fprintf(intermediate_file, "\tToo Many Symbols = 7\n");
fprintf(intermediate_file, "\tProgram Too Long = 8\n");
fprintf(intermediate_file, "*=================================================*\n");

fclose(intermediate_file);
fclose(source_file);
fclose(symbol_file);
}
void passTwo()
{
	char input[100];//
	char *tokenizer;//

	char *objectCode;
	int addressConverter;


	int needNewLine = 1;
	int lineSize = 0;





	char *machineCode;
	char *incommingMachineCode;
	char *tMnemonic;
	char *tOperand;


	int startingAddress = 0;

	int addre;
	int objAddress;

	int labelFound = 0;

	FILE *intermediate, *symbolTable, *objectFile, *listingFile;
	TOKEN intermediateFileTokenizer;

	objectFile = fopen("objectFile.txt", "w");
	listingFile = fopen("listingFile.txt", "w");
	intermediate = fopen("intermediate.txt", "r");
	symbolTable = fopen("symbolTable.txt", "r");

	if (intermediate == NULL)
	{
		printf("Intermediate file did not opened correctly \n");
		return;
	}

	machineCode = (char*)malloc(100);
	incommingMachineCode = (char *)malloc(100);

	memset(machineCode, '\0', 100);
	memset(incommingMachineCode, '\0', 100);
	

	intermediateFileTokenizer.label = (char *)malloc(6);
	intermediateFileTokenizer.mnemonic = (char *)malloc(6);
	intermediateFileTokenizer.opcode = (char *)malloc(6);



	while (fgets(input, 100, intermediate))
	{
		labelFound = 0;
		tokenizer = strtok(input, "\t");
		intermediateFileTokenizer.memoryAddress = (int)strtol(tokenizer, NULL, 16);   ////save address

		tokenizer = strtok(NULL, "\t");

		for (int i = 0; i < numLabels; i++)
		{
			if (strcmp(labelStructure[i].label, tokenizer) == 0)
			{
				labelFound = 1;
				break;
			}
		}

		if (labelFound == 1)
		{
			strcpy(intermediateFileTokenizer.label, tokenizer); ////save label
			tokenizer = strtok(NULL, "\t");
		}
		
		strcpy(intermediateFileTokenizer.mnemonic, tokenizer); ////save mnemonic
		
		if (strcmp(intermediateFileTokenizer.mnemonic, "RSUB") != 0)
		{
			tokenizer = strtok(NULL, "\t");
			strcpy(intermediateFileTokenizer.opcode, tokenizer); /////save opcode
			tokenizer = strtok(NULL, " \t");
			intermediateFileTokenizer.errorCode =(int)strtol(tokenizer,NULL,10); ////save errorcode

		}
		else
		{
			strcpy(intermediateFileTokenizer.label, " ");
			strcpy(intermediateFileTokenizer.opcode, " ");

			/*printf("Here is a RSUB condition \n");

			char foundOpcode;
			int newChar;
			for (int i = 0; i < numMnemonics; i++)
			{
				if (strcmp(opcodeStructure[i].mnemonic, intermediateFileTokenizer.mnemonic) == 0)
				{
					foundOpcode = opcodeStructure[i].opcode;
					break;
				}
			}

			newChar = foundOpcode << 6 ;

			if (labelFound == 1)
			{
				printf("%X\t%s\t%s\t%s\t%06X\n", intermediateFileTokenizer.memoryAddress, intermediateFileTokenizer.label, intermediateFileTokenizer.mnemonic, intermediateFileTokenizer.opcode, newChar);
				continue;
			}
			else
			{
				printf("%X\t%s\t%s\t%06X\n", intermediateFileTokenizer.memoryAddress, intermediateFileTokenizer.mnemonic, intermediateFileTokenizer.opcode, newChar);
				continue;

			}*/

		}


		

		
		
		if (lineSize == 10)
		{
			needNewLine = 1;
			//fprintf(objectFile, "\n");
			lineSize = 0;
		}
		
		
		if (strcmp(intermediateFileTokenizer.mnemonic, "RESW") == 0 || strcmp(intermediateFileTokenizer.mnemonic, "RESB") == 0)
		{
			
			if (labelFound == 1)
			{
				fprintf(listingFile, "%X\t%s\t%s\t%s\n", intermediateFileTokenizer.memoryAddress, intermediateFileTokenizer.label, intermediateFileTokenizer.mnemonic, intermediateFileTokenizer.opcode);
				printf("%X\t%s\t%s\t%s\n", intermediateFileTokenizer.memoryAddress, intermediateFileTokenizer.label, intermediateFileTokenizer.mnemonic, intermediateFileTokenizer.opcode);
				continue;
			}
			else
			{
				fprintf(listingFile, "%X\t%s\t%s\t%06X\n", intermediateFileTokenizer.memoryAddress, intermediateFileTokenizer.mnemonic, intermediateFileTokenizer.opcode);
				printf("%X\t%s\t%s\n", intermediateFileTokenizer.memoryAddress, intermediateFileTokenizer.mnemonic, intermediateFileTokenizer.opcode);
				continue;

			}

		}
		

		if (strcmp(intermediateFileTokenizer.mnemonic, "START") == 0)
		{
			printf("STARTING LOCATION\n");
			printf("H%_%s %06d%06X\n", intermediateFileTokenizer.label, intermediateFileTokenizer.memoryAddress, programLenght); //need to add the print to the object file
			fprintf(objectFile,"H%_%s %06d%06X", intermediateFileTokenizer.label, intermediateFileTokenizer.memoryAddress, programLenght);

			if (labelFound == 1)
			{
				fprintf(listingFile,"%X\t%s\t%s\t%s\n", intermediateFileTokenizer.memoryAddress, intermediateFileTokenizer.label, intermediateFileTokenizer.mnemonic, intermediateFileTokenizer.opcode);
			}
			else
			{
				fprintf(listingFile, "%X\t\t%s\t%s\n", intermediateFileTokenizer.memoryAddress, intermediateFileTokenizer.mnemonic, intermediateFileTokenizer.opcode);

			}
			startingAddress = intermediateFileTokenizer.memoryAddress;
		}

		else if (strcmp(intermediateFileTokenizer.mnemonic, "END") == 0)
		{
			printf("ENDING LOCATION\n");
			addre = searchLabelLocation(intermediateFileTokenizer.opcode);
			//fprintf(objectFile, "E%06d", addre);
			printf("E%06X\n", addre);
			fprintf(objectFile,"\n");
			fprintf(objectFile,"E%06X", addre);

			if (labelFound == 1)
			{
				fprintf(listingFile, "\t%s\t%s\t%s\n", intermediateFileTokenizer.label, intermediateFileTokenizer.mnemonic, intermediateFileTokenizer.opcode);
			}
			else
			{
				fprintf(listingFile, "\t\t\t\t%s\t\t%s\n", intermediateFileTokenizer.mnemonic, intermediateFileTokenizer.opcode);

			}
			break;


		}
		else if (strcmp(intermediateFileTokenizer.mnemonic, "WORD") == 0)
		{

			printf("HERE IS A WORD LOCATION\n");
			objectCode = intermediateFileTokenizer.opcode;
			addressConverter = (int)strtol(objectCode,NULL,10);
			//printf( "%06X\n",	addressConverter);
			if (strcmp(intermediateFileTokenizer.opcode, "0")== 0)
			{
				fprintf(objectFile, "%06X", addressConverter);
				needNewLine = 1;
				lineSize = 0;
				continue;

			}

			if (needNewLine == 1)
			{
				fprintf(objectFile, "\n");
				fprintf(objectFile, "T%06X%06X",intermediateFileTokenizer.memoryAddress, addressConverter);
				needNewLine = 0;
				lineSize++;

			}
			else
			{
				fprintf(objectFile, "%06X", addressConverter);
				lineSize++;
			}

			if (labelFound == 1)
			{
				fprintf(listingFile,"%X\t%s\t%s\t%s\t%06X\n", intermediateFileTokenizer.memoryAddress, intermediateFileTokenizer.label, intermediateFileTokenizer.mnemonic, intermediateFileTokenizer.opcode, addressConverter);
				printf("%X\t%s\t%s\t%s\t%06X\n", intermediateFileTokenizer.memoryAddress, intermediateFileTokenizer.label, intermediateFileTokenizer.mnemonic, intermediateFileTokenizer.opcode, addressConverter);
			}
			else
			{
				fprintf(listingFile,"%X\t%s\t%s\t%06X\n", intermediateFileTokenizer.memoryAddress, intermediateFileTokenizer.mnemonic, intermediateFileTokenizer.opcode, addressConverter);
				printf("%X\t%s\t%s\t%06X\n", intermediateFileTokenizer.memoryAddress, intermediateFileTokenizer.mnemonic, intermediateFileTokenizer.opcode, addressConverter);

			}
		}
		
		else if(strcmp(intermediateFileTokenizer.mnemonic, "BYTE") == 0)
		{
			

			if(intermediateFileTokenizer.opcode[0] == 'C')
			{	
				
				char copyHex[10];

				char string1[10];
				

				int counter = 2;
				int buffer = 0;

				while (intermediateFileTokenizer.opcode[counter] != '\'')
				{
					copyHex[buffer] = intermediateFileTokenizer.opcode[counter];
					counter++;
					buffer++;
				}

				sprintf(string1, "%X%X%X", copyHex[0],copyHex[1],copyHex[2]);
				

				printf("here is a byte'C' LOCATION\n");

				if (labelFound == 1)
				{
					printf("%X\t%s\t%s\t%s\t%s\n", intermediateFileTokenizer.memoryAddress, intermediateFileTokenizer.label, intermediateFileTokenizer.mnemonic, intermediateFileTokenizer.opcode, string1);
					fprintf(listingFile,"%X\t%s\t%s\t%s\t%s\n", intermediateFileTokenizer.memoryAddress, intermediateFileTokenizer.label, intermediateFileTokenizer.mnemonic, intermediateFileTokenizer.opcode, string1);
				}
				else
				{
					printf("%X\t%s\t%s\t%s\n", intermediateFileTokenizer.memoryAddress, intermediateFileTokenizer.mnemonic, intermediateFileTokenizer.opcode, string1);
					fprintf(listingFile,"%X\t%s\t%s\t%s\n", intermediateFileTokenizer.memoryAddress, intermediateFileTokenizer.mnemonic, intermediateFileTokenizer.opcode, string1);
				}
				

				if (needNewLine == 1)
				{
					fprintf(objectFile, "\n");
					fprintf(objectFile, "T%06X%s", intermediateFileTokenizer.memoryAddress, string1);
					needNewLine = 0;
					lineSize++;

				}
				else
				{
					fprintf(objectFile, "%s", string1);
					lineSize++;
				}
				//printf("%s\n", string1);

				

			}
			else if (intermediateFileTokenizer.opcode[0] == 'X')
			{
				printf("HERE IS A BYTE'X' LOCATION\n");
				char string1[10];
				int counter = 2;
				int buffer = 0;

				while (intermediateFileTokenizer.opcode[counter] != '\'')
				{
					string1[buffer] = intermediateFileTokenizer.opcode[counter];
					counter++;
					buffer++;
				}
				string1[buffer] = '\0';

				//lprintf("%s\n", string1);

				if (labelFound == 1)
				{
					printf("%X\t%s\t%s\t%s\t%s\n", intermediateFileTokenizer.memoryAddress, intermediateFileTokenizer.label, intermediateFileTokenizer.mnemonic, intermediateFileTokenizer.opcode, string1);
					fprintf(listingFile,"%X\t%s\t%s\t%s\t%s\n", intermediateFileTokenizer.memoryAddress, intermediateFileTokenizer.label, intermediateFileTokenizer.mnemonic, intermediateFileTokenizer.opcode, string1);
				}
				else
				{
					printf("%X\t%s\t%s\t%s\n", intermediateFileTokenizer.memoryAddress, intermediateFileTokenizer.mnemonic, intermediateFileTokenizer.opcode, string1);
					fprintf(listingFile,"%X\t%s\t%s\t%s\n", intermediateFileTokenizer.memoryAddress, intermediateFileTokenizer.mnemonic, intermediateFileTokenizer.opcode, string1);
				}

				if (needNewLine == 1)
				{
					fprintf(objectFile, "\n");
					fprintf(objectFile, "T%06X%s", intermediateFileTokenizer.memoryAddress, string1);
					needNewLine = 0;
					lineSize++;

				}
				else
				{
					fprintf(objectFile, "%s", string1);
					lineSize++;
				}


			}


		}
		else
		{
			if (labelFound == 1)
			{
				char *foundOpcode;
				int convertedOpcode;
				char objectCode[10];
				int opcodeLocation;
				
				
				
				opcodeLocation = searchLabelLocation(intermediateFileTokenizer.opcode);
				
				
				for (int i = 0; i < numMnemonics; i++)
				{
					if (strcmp(opcodeStructure[i].mnemonic, intermediateFileTokenizer.mnemonic) == 0)
					{
						foundOpcode = opcodeStructure[i].opcode;
						break;
					}
				}

				convertedOpcode = (int)strtol(foundOpcode, NULL, 16);
				
				printf("HERE are the symboltable with labels \n");
				sprintf(objectCode, "%X%X", convertedOpcode, opcodeLocation);
				printf("%X\t%s\t%s\t%s\t%06s\n", intermediateFileTokenizer.memoryAddress, intermediateFileTokenizer.label, intermediateFileTokenizer.mnemonic, intermediateFileTokenizer.opcode, objectCode);
				fprintf(listingFile,"%X\t%s\t%s\t\t%s\t%06s\n", intermediateFileTokenizer.memoryAddress, intermediateFileTokenizer.label, intermediateFileTokenizer.mnemonic, intermediateFileTokenizer.opcode, objectCode);
				


				if (needNewLine == 1)
				{
					fprintf(objectFile, "\n");
					fprintf(objectFile, "T%06X%06s", intermediateFileTokenizer.memoryAddress, objectCode);
					needNewLine = 0;
					lineSize++;

				}
				else
				{
					fprintf(objectFile, "%06s", objectCode);
					lineSize++;
				}
			}
			else
			{
				char *foundOpcode;
				int convertedOpcode;
				char objectCode[10];
				int opcodeLocation;



				opcodeLocation = searchLabelLocation(intermediateFileTokenizer.opcode);
				

				for (int i = 0; i < numMnemonics; i++)
				{
					if (strcmp(opcodeStructure[i].mnemonic, intermediateFileTokenizer.mnemonic) == 0)
					{
						foundOpcode = opcodeStructure[i].opcode;
						break;
					}
				}

				convertedOpcode = (int)strtol(foundOpcode, NULL, 16);

				printf("HERE are the symboltable with NO LABELS \n");
				sprintf(objectCode, "%X%X", convertedOpcode, opcodeLocation);
				printf("%X\t%s\t%s\t%06s\n", intermediateFileTokenizer.memoryAddress, intermediateFileTokenizer.mnemonic, intermediateFileTokenizer.opcode, objectCode);
				fprintf(listingFile,"%X\t\t\t%s\t\t%s\t%06s\n", intermediateFileTokenizer.memoryAddress, intermediateFileTokenizer.mnemonic, intermediateFileTokenizer.opcode, objectCode);



				if (needNewLine == 1)
				{
					fprintf(objectFile, "\n");
					fprintf(objectFile, "T%06X%06s", intermediateFileTokenizer.memoryAddress, objectCode);
					needNewLine = 0;
					lineSize++;

				}
				else
				{
					fprintf(objectFile, "%06s", objectCode);
					lineSize++;
				}
			}



		}




	}








	
	fclose(objectFile);
	fclose(listingFile);
	fclose(intermediate);
	fclose(symbolTable);

	//printf("Files closed");

	//if (errorFound == 1)
	//{
	//	if (remove("objectFile.txt") == 0)
	//		printf("Program Has Errors.\n");
	//}


}
int searchLabelLocation(char *inputLabel)
{
	char input[100];
	char *tokenizer;

	int memoryLocation;

	FILE *symbol_table = fopen("symbolTable.txt", "r");
	
	while (fgets(input, 100, symbol_table))
	{
		input[strcspn(input, "\n")] = '\0';
		tokenizer = strtok(input, "\t");
		memoryLocation = (int)strtol(tokenizer, NULL, 16);
		tokenizer = strtok(NULL, "\t");
		if (strcmp(tokenizer, inputLabel) == 0)
		{
			fclose(symbol_table);
			return memoryLocation;
		}
	}
	fclose(symbol_table);
		return 00000;

}

char *searchOpcodeTable(char*input)
{
	char *foundOpcode;
	char *noFoundOpcode = "0000";

	for (int i = 0; i < numMnemonics; i++)
	{
		if (strcmp(opcodeStructure[i].mnemonic, input) == 0)
		{
			foundOpcode = opcodeStructure[i].opcode;
			return foundOpcode;
		}
	}
	return noFoundOpcode;
	
}
