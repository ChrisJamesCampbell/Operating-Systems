#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <stdio.h>

//need to compile with gcc -O(capital oh) -Wall -lreadline -lm myshell.c -o myshell

int main(int argc, char **argv)
{


	signal (...) //something needs to go in here

	printf("ShellDeLaCJC > ");

	while(1) //infinite loop
	{
	
	char *user_input;

	user_input = readline(argv[0]);

		if(strncmp("cd", line, 2) == 0)
		{
			//chdir something something 
		}
		else if(strncmp("exit", line 4) == 0)
		{
			//invoke exit or some shit
		}
		else {
				if ( fork () == 0) 
				{
					...
					execvp (...) ;
					...
				} 
				else 
				{
					...
					wait (...) ;
					...
				}
			}
	free(user_input);

	}

	return 0;
}


