/*
The given code is a basic implementation of a shell program.
It reads user input from the command line, splits it into individual commands 
(separated by pipes), and executes each command in a separate child process.

The program starts by initializing various variables and setting up file descriptors 
for input and output. 
It then enters a loop to prompt the user for input until a certain heredoc 
delimiter is entered.

Inside the loop, it reads user input using the getline() function and writes the input 
to both stdout and a file descriptor. 
Once the delimiter is entered or the user presses CTRL + C, the loop breaks and 
the program moves on to execute the commands.

The program then initializes a pipe, forks a new child process, 
and assigns input and output file descriptors to the child process based on the current 
command being executed. It then passes the command to the execute() function, 
which is responsible for executing the actual command.

If the command is executed successfully, the program moves on to the next command, 
and the loop continues until all commands have been executed. 
If any command fails, the program exits with an error message.

Overall, the program provides a basic shell interface for executing commands, 
with support for pipes and basic input/output redirection. 
However, it lacks support for more advanced features such as background processes, 
shell variables, and more complex input/output handling.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

// Function declarations
void execute(char** command, int input_fd, int output_fd);

// Main function
int main() 
{
    char* command_line = NULL;
    size_t buffer_size = 0;
    ssize_t line_size = 0;

    // Loop through each command line input
    while (1) 
	{
        printf("minishell> ");
        line_size = getline(&command_line, &buffer_size, stdin);

        // Check for errors or EOF
        if (line_size == -1) {
            break;
        }

        // Remove newline character
        command_line[strcspn(command_line, "\n")] = 0;

        // Tokenize command line input by pipe
        char* command_token = strtok(command_line, "|");
        char* previous_token = NULL;
        int input_fd = 0;

        while (command_token != NULL) {
            // Trim leading whitespace
            while (isspace((unsigned char)*command_token)) {
                command_token++;
            }

            // Trim trailing whitespace
            char* end_token = command_token + strlen(command_token) - 1;
            while (end_token > command_token && isspace((unsigned char)*end_token)) {
                end_token--;
            }
            *(end_token + 1) = '\0';

            // Check if command has input or output redirection
            char* input_file = NULL;
            char* output_file = NULL;
            char* output_mode = NULL;
            char* heredoc_text = NULL;

            char* redirection_token = strtok(command_token, "><");
            int is_input_redirection = 0;
            int is_output_redirection = 0;
            while (redirection_token != NULL) {
                redirection_token = strtok(NULL, "><");
                if (redirection_token != NULL) {
                    // Strip leading/trailing whitespace
                    while (isspace((unsigned char)*redirection_token)) {
                        redirection_token++;
                    }

                    char* end_redirection_token = redirection_token + strlen(redirection_token) - 1;
                    while (end_redirection_token > redirection_token && isspace((unsigned char)*end_redirection_token)) {
                        end_redirection_token--;
                    }
                    *(end_redirection_token + 1) = '\0';

                    // Set redirection flags & filenames
                    if (*(redirection_token - 1) == '<') {
                        is_input_redirection = 1;
                        input_file = redirection_token;
                    } else {
                        is_output_redirection = 1;
                        if (*redirection_token == '>') {
                            output_mode = "w";
                            if (*(redirection_token + 1) == '>') {
                                output_mode = "a";
                                redirection_token++;
                            }
                            output_file = redirection_token + 1;
                        } else if (*redirection_token == '<') {
                            heredoc_text = redirection_token + 1;
                        }
                    }
                }
            }

            // Open input and output files if necessary
            int output_fd = STDOUT_FILENO;
            if (is_output_redirection) {
                output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            } else if (heredoc_text != NULL) {
                char* end_heredoc_text = strchr(command_line, '<') + 1;
                *end_heredoc_text = '\0';

                char* prompt = strtok(end_heredoc_text, "\n");
                char* user_input = NULL;
                size_t input_size = 0;
			}

			// Prompt user for input until heredoc delimiter is reached 
			
			while (1)
			{
				printf("> ");
				ssize_t input_size = getline(&user_input, &buffer_size, stdin);
				if (input_size == -1 || strcmp(user_input, heredoc_text) == 0)
				{
					break; 
				}
				write(STDOUT_FILENO, user_input, input_size);
				write(output_fd, user_input, input_size);
			} 
		}

        int pipe_fd[2];
        if (pipe(pipe_fd) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        pid_t pid = fork();
        if (pid == -1)
		{
            perror("fork");
            exit(EXIT_FAILURE);
        }
		else if (pid == 0)
		{
            // Child process
            close(pipe_fd[0]);
            if (previous_token != NULL)
			{
                dup2(input_fd, STDIN_FILENO);
            }
            if (command_token != NULL)
			{
                dup2(pipe_fd[1], STDOUT_FILENO);
            }
            execute(command, input_fd, pipe_fd[1]);
            exit(EXIT_FAILURE);
        }
		else 
		{
            // Parent process
            wait(NULL);
            close(pipe_fd[1]);
            // Save input file descriptor for next command
            input_fd = pipe_fd[0];
            previous_token = command_token;
            // Get next command token
            command_token = strtok(NULL, "|");
        }

        // Close input file if necessary
        if (is_input_redirection)
		{
            close(input_fd);
        }

        // Close output file if necessary
        if (is_output_redirection)
		{
            close(output_fd);
        }
    }
	free(command_line);
	return 0;
}

// Function to execute command 
void execute(char** command, int input_fd, int output_fd)
{
	dup2(input_fd, STDIN_FILENO);
	dup2(output_fd, STDOUT_FILENO);

	char* args[100];
	int i = 0;
	while (command[i] != NULL) {
		args[i] = command[i];
		i++;
	}
	args[i] = NULL;

	execvp(args[0], args);

	perror("execvp");
	exit(EXIT_FAILURE);
}
