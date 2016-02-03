//CHRIS JAMES CAMPBELL
//s1334028
//OPERATING SYSTEMS: COURSEWORK 1
//LAST UPDATED: 02/02/2016

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <setjmp.h>

#define SHELL_TOK_BUFSIZE 64
#define SHELL_TOK_DELIM " \t\r\n\a" //tab, carriage return, new line, bell 

//Signal long jumo
sigjmp_buf buf;

 // Function Declarations for built in shell commands:
int shell_cd(char **args);
int shell_exit(char **args);


//List of built in recognisable commands, followed by their corresponding functions.
char *built_in_str[] = 
{
  "cd",
  "exit"
};

int (*built_in_func[]) (char **) = 
{
  &shell_cd,
  &shell_exit
};

struct built_in
{
  char *acceptable_string;
  int (*call_back)(void);
};

void print_shell_name(void)
{
  printf("\nCJC's Shell > ");
}

char *shell_read_line(void)
{
  char *line = NULL;
  ssize_t bufsize = 0; // have getline allocate a buffer for us
  getline(&line, &bufsize, stdin);
  return line;
}

char **shell_split_line(char *line)
{
  int bufsize = SHELL_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens) {
    fprintf(stderr, "shell: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, SHELL_TOK_DELIM);

  while (token != NULL) 
  {
    tokens[position] = token;
    position++;

    if (position >= bufsize) 
    {
      bufsize += SHELL_TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      
      if (!tokens) 
      {
        fprintf(stderr, "shell: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, SHELL_TOK_DELIM);
  }

  tokens[position] = NULL;
  return tokens;
}

int shell_launch(char **args)
{
  pid_t pid, wpid; //parent ID
  int status;

  pid = fork();

  if (pid == 0) 
  {
    // Child process
    if (execvp(args[0], args) == -1) 
    {
      perror("shell");
    }
    exit(EXIT_FAILURE);
  }

  else if (pid < 0) 
  {
    // Error forking
    perror("shell");
  } 

  else 
  {
    // Parent process
    do 
    
    {
      wpid = waitpid(pid, &status, WUNTRACED);
    } 

    while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}


int shell_num_built_ins() {
  return sizeof(built_in_str) / sizeof(char *);
}

int shell_cd(char **args)
{
  if (args[1] == NULL) //special case for when no arguments are give with 'cd' and we just want to go to HOME directory
  {
    chdir(getenv("HOME"));
  } 
  
  else 
  {
    if (chdir(args[1]) != 0) 
    {
      perror("shell");
    }
  }
  return 1;
}


int shell_exit(char **args)
{
  return 0;
}

int shell_execute(char **args)
{
  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  for (i = 0; i < shell_num_built_ins(); i++) 
  {
    if (strcmp(args[0], built_in_str[i]) == 0) 
    {
      return (*built_in_func[i])(args);
    }
  }

  return shell_launch(args);
}

void shell_loop(void)
{
  char *line;
  char **args;
  int status;

  sigsetjmp(buf, 1);

  do 

  {
    print_shell_name();
    line = shell_read_line();
    args = shell_split_line(line);
    status = shell_execute(args);

    free(line);
    free(args);
  } 

  while (status);
}

void signal_handler(int signal)
{
  switch(signal)
  {
    //print_shell_name();
    case SIGINT: //need to handle
    //printf("Oooooosh\n");
    //call shell print
    //fflush(stdout);
    write(0, "\n", 1);
    siglongjmp(buf, 1);
    //print_shell_name();
    break;
    case SIGTERM: //do nothing, exit
    default:
    //write(0, "\n", 1);
    exit(signal);
  }
}


int main(int argc, char **argv)
{
  // Load config files, if any.
  signal(SIGINT, signal_handler);
  // Run command loop.
  shell_loop();

  // Perform any shutdown/cleanup.

  return EXIT_SUCCESS;
}

