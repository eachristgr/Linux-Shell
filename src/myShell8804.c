/*
 * ====================================================================
 *
 *       Filename:  myShell8804.c
 *
 *         Author:  Emmanouil Christos 8804
 *
 * ====================================================================
 */
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<unistd.h>
#include	<sys/types.h>
#include	<sys/wait.h>
#include	<signal.h>
#include	<fcntl.h>

//Constants
#define BUFFERSIZE 1024
#define MAXSIZE 512
#define DLT 260

//Function Decleraions
FILE* desideIn(int argc, char **argv);
void printPrompt();
void readLine(FILE **input, char **line);
void parseLine(char *line, char ***tokens, int *lastTokenNum);
void execLine(char **tokens, int lastTokenNum);
void quitShell();
int execSimple(char **tokens, int lastTokenNum);
void execQues(char ***commands, int delPos, int restlastTokenNum);
void execAmber(char ***commands, int delPos, int restlastTokenNum);

//main Func.
int main(int argc, char **argv)
{
  printf("Welcome to myShell8804 created by : Emmanouil Christos 8804\n\n");
  FILE *input = desideIn(argc, argv);
  char *line;
  char **tokens;
  int lastTokenNum; //tokens[lastTokenNum] == NULL

  while (1) 
  {
    if (input == stdin)
  	{
  		printPrompt();
  	}
    readLine(&input, &line);
  	parseLine(line, &tokens, &lastTokenNum);
  	execLine(tokens, lastTokenNum);

    free(line);
    free(tokens);
  }

  return 0;
}

//desideIn : select shell function
FILE* desideIn(int argc, char **argv)
{
	FILE* input;

	switch (argc) 
	{
  	case 1: //Keyboard
  		input = stdin;
  		break;
  	case 2: //File
  		if ((input = fopen(argv[1],"r")) == NULL)
  		{
    		printf("Error : fopen() Failure\n");
    		exit(0);
  		}
  		break;
  	default :
  		printf("ERROR : Invalid Number Of Arguments\n");
  		exit(0);
  }

  return input;
}

//printPromt : name_aem
void printPrompt()
{
	printf("emmanouil_8804>");
}

//readLine : read input
void readLine(FILE **input, char **line)
{
	*line = (char *)malloc(BUFFERSIZE * sizeof(char));
	if (!(*line))
	{
		printf("ERROR : malloc() Failure 1\n");
    	exit(0);
	}

	if (fgets(*line, BUFFERSIZE, *input) != NULL)
	{
		if (strlen(*line) > MAXSIZE + 1) //Size Limit Check
    	{
        	printf("ERROR : Size Limit Reached\n");
        	**line = '\0';
    	}
  }
	else //EOF Check
  {
  	printf("EOF or ERROR : fgets() Failure\n");
    exit(0);
  }
}

//parseLine : parsing input
void parseLine(char *line, char ***tokens, int *lastTokenNum)
{
	*tokens = (char **)malloc((DLT + 1) * sizeof(char *)); //DLT lastTokenNum + NULL
	if (!(*tokens))
	{
  		printf("ERROR : malloc() Failure 2\n");
    	exit(0);
	}

	char *token = (char *)malloc((MAXSIZE + 1) * sizeof(char));
	if (!token) 
	{
		printf("ERROR : malloc() Failure 3\n");;
  	exit(0);
	}

	*lastTokenNum = 0;

	token = strtok(line, " \t\n");
	while (token != NULL)
	{
  	(*tokens)[*lastTokenNum] = token;
  	(*lastTokenNum)++;
  	token = strtok(NULL, " \t\n");
	}
	(*tokens)[*lastTokenNum] = NULL;

	free(token);
}

//execLine : check for ; and &&
void execLine(char **tokens, int lastTokenNum)
{
  int type = 0;
  int delPos;

  for (int i=0; i<lastTokenNum; i++) //Search For Delimiters And Find Position
  {
  	if (strcmp(tokens[i], ";") == 0)
    {
      type = 1;
      delPos = i;
      if (delPos == 0) //Delimiters Can Not Be In First Place
  		{
  			printf("ERROR : syntax Failure\n");
  			return;
  		}
  		break;
    }
  	if (strcmp(tokens[i], "&&") == 0)
  	{
  		type = 2;
    	delPos = i;
    	if (delPos == 0) //Delimiters Can Not Be In First Place
			{
				printf("ERROR : syntax Failure\n");
				return;
			}
			break;
  	}
	}

	if (type == 0)//Select Action
	{
		execSimple(tokens, lastTokenNum);
	}
	else
	{
		char ***commands = (char ***)malloc(2 * sizeof(char **));
		if (!commands) 
		{
  		printf("ERROR : malloc() Failure 4\n");;
  		exit(0);
		}

  	commands[0] = &tokens[0];// &tokens[0] == tockens
		commands[0][delPos] = NULL;
		commands[1] = &tokens[delPos + 1];
    int restlastTokenNum = lastTokenNum - delPos - 1;

    switch (type) 
		{
  		case 1:
  			execQues(commands, delPos, restlastTokenNum);
  			break;
  		case 2:
  			execAmber(commands, delPos, restlastTokenNum);
  			break;
  		default:
  			printf("ERROR : switch() Failure\n");
  			break;
		}

  	free(commands);

  }
}

//quitShell : exit shell
void quitShell()
{
	printf("GoodBye!!!\n");
  exit(0);
}

//execSimple : execute one command
int execSimple(char **tokens, int lastTokenNum)
{
	if (tokens[0] == NULL) //Empty Line Check
	{
		return 1;
	}

	if (strcmp(tokens[0], "quit") == 0) //Exit C. Check
	{
  	quitShell();
	}

	int status;
	pid_t pid = fork();
	if (pid == -1) //ERROR
	{
    printf("ERROR : fork() Failure\n");
    exit(1);
  }
  else if (pid == 0) //Child
  {
  	for (int i=0; i<lastTokenNum; i++) //Search For Redirections
	  {	
    	if (strcmp(tokens[i], "<") == 0)
    	{
        if (tokens[i+1] == NULL)
        {
          printf("Directory Missing\n");
          exit(3);
        }
        char *filename = tokens[i + 1];

        int j=0;
        for (int k = 0; k < lastTokenNum; k++)
      	{
          if ((strcmp(tokens[k], "<") != 0) && (strcmp(tokens[k], filename) != 0))
    			{
    				tokens[j] = tokens[k];
    				j++;
    			}
      	}
      	tokens[j] = NULL; //j == lastTokenNum - 2

      	int fd;
      	if ((fd = open(filename, O_RDONLY)) == -1)
        {
          printf("Error : open() Failure 1\n");
          exit(3);
        }
        dup2(fd, STDIN_FILENO);
        break;
      }

    	if (strcmp(tokens[i], ">") == 0)
    	{
        if (tokens[i+1] == NULL)
        {
          printf("Directory Missing\n");
          exit(3);
        }
        char *filename = tokens[i + 1];

      	int j=0;
        for (int k = 0; k < lastTokenNum; k++)
        {
          if ((strcmp(tokens[k], ">") != 0) && (strcmp(tokens[k], filename) != 0))
          {
            tokens[j] = tokens[k];
            j++;
          }
        }
        tokens[j] = NULL; //j == lastTokenNum - 2

      	int fd;
      	if ((fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU)) == -1)
    	 	{
          printf("Error : open() Failure 2\n");
    	 		exit(3);
    	 	}
        dup2(fd, STDOUT_FILENO);
        break;
      }
  	}

  	execvp(tokens[0], tokens);
  	printf("ERROR : execvp() Failure\n");
  	exit(3);
  }
  else //Father
  {
  	wait(&status);
  	if (WIFEXITED(status))// Normal Exit (no signal)
  	{
      if (WEXITSTATUS(status) == 0)
      {
        return 1;
      }
    }
  	return 0;
  }
}

//execQues : if ; exist
void execQues(char ***commands, int delPos, int restlastTokenNum)
{
	execSimple(commands[0], delPos);
  execLine(commands[1], restlastTokenNum);
}

//execAmber : if && exist
void execAmber(char ***commands, int delPos, int restlastTokenNum)
{
	if (commands[1][0] == NULL) 
	{
    printf("Expect Something After &&\n");
    return;
	}

	if (execSimple(commands[0], delPos) == 1) {
    	execLine(commands[1], restlastTokenNum);
	}
}