// This SECTION CONSISTS OF HEADERS THAT NEED TO BE DEFINED ON HOST OS
//
//
// OMG STDIO! I LOVE YOU!

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

// THIS SECTION HAS #define-s THAT I THOUGHT WERE USEFUL
//
//
//

/*NOTHING TO SEE HERE.*/

// PROGRAM SPECIFIC FUNCTIONS
//
//
//

void findAndPrint (char *l)
{
	char *line = l;
	int len=0;

	while(*line != ':' && *line)
	{
		line++;
		len++;
	}
	line++;

	if(l[0] == 'N' && l[1] == 'a' && len == 4) // Laziness ki koi hadd nahi :P
	{
		printf("Process Name      : \t\t");
		printf("%s" , line);
	}

	if(l[0] == 'S' && len == 5)
	{
		printf("Process Status    : \t\t");
		printf("%s" , line);
	}

	if(l[0] == 'P' && len == 4)
	{
		printf("Parent PID        : \t\t");
		printf("%s" , line);
	}

	if(l[0] == 'T' && len == 7)
	{
		printf("Threads           : \t\t");
		printf("%s" , line);
	}
}


void printStatus(char *path)
{
	FILE *fp = fopen(path , "r");
	char *lineBuffer = NULL;
	size_t len;

	while (getline(&lineBuffer, &len, fp) != -1)
		findAndPrint(lineBuffer);

	fclose(fp);
}

const char * getNum( const int argc , const char ** argv)
{
	int i;
	for (i = 1 ; i < argc ; i++)
	{
		char *ch = (char *)argv[i];
		while (*ch)
		{
			if ( ((*ch - '0') >= 0) && ((*ch - '0') <= 9) )
				ch++;
			else
				break;
		}
		if (!(*ch))
			return argv[i];
	}
	return NULL;
}

int main(const int argc, char const ** argv)
{
	const char *PID = getNum(argc,argv);

	if (PID)
	{
		//printf(PID);
		char path[100];
		strcpy(path , "/proc/");
		strcat(path , PID);
		strcat(path , "/status");

		if (access (path , F_OK) != 0)
		{
			printf("No Process found having this PID\n");
			return -1;
		}
		else
			printStatus(path);
	}
	else
		printf("Enter Number\n");
	return 0;
}
