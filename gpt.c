#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

#define MAX_INPUT_SIZE 1024
#define MAX_ARG_SIZE 64
#define MAX_PATH_SIZE 256

volatile sig_atomic_t child_exit_status;

void sigchld_handler(int signo) {
    (void)signo;
    int status;
    waitpid(-1, &status, WNOHANG);
    if (WIFEXITED(status)) {
        child_exit_status = WEXITSTATUS(status);
    }
}

void execute_command(char **args) {
    pid_t pid, wpid;
    int status;

    (void)args;
    if ((pid = fork()) == 0) {
        // Child process
        execvp(args[0], args);
        perror("shell");
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("shell");
    } else {
        // Parent process
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
}

void handle_signals() {
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigaction(SIGCHLD, &sa, NULL);
}

#include <fcntl.h>

void parse_and_execute(char *input) {
    const char *delimiters = " \t\r\n\a";
    char *args[MAX_ARG_SIZE];
    int arg_count = 0;

    char *token = strtok(input, delimiters);
    while (token != NULL && arg_count < MAX_ARG_SIZE - 1) {
        args[arg_count++] = token;
        token = strtok(NULL, delimiters);
    }
    args[arg_count] = NULL;

    if (arg_count > 0) {
        int input_fd = STDIN_FILENO;
        int output_fd = STDOUT_FILENO;
        int append = 0;

        for (int i = 0; args[i] != NULL; i++) {
            if (strcmp(args[i], "<") == 0) {
                // Input redirection
                if (args[i + 1] != NULL) {
                    input_fd = open(args[i + 1], O_RDONLY);
                    if (input_fd == -1) {
                        perror("shell");
                        return;
                    }
                    i++;  // Skip the filename
                } else {
                    fprintf(stderr, "shell: syntax error near unexpected token `<'\n");
                    return;
                }
            } else if (strcmp(args[i], ">") == 0) {
                // Output redirection (truncate)
                if (args[i + 1] != NULL) {
                    output_fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    if (output_fd == -1) {
                        perror("shell");
                        return;
                    }
                    i++;  // Skip the filename
                } else {
                    fprintf(stderr, "shell: syntax error near unexpected token `>'\n");
                    return;
                }
            } else if (strcmp(args[i], ">>") == 0) {
                // Output redirection (append)
                append = 1;
                if (args[i + 1] != NULL) {
                    output_fd = open(args[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
                    if (output_fd == -1) {
                        perror("shell");
                        return;
                    }
                    i++;  // Skip the filename
                } else {
                    fprintf(stderr, "shell: syntax error near unexpected token `>>'\n");
                    return;
                }
            }
        }

        // Fork and execute the command
        pid_t pid = fork();

        if (pid == 0) {
            // Child process
            // Redirect stdin and/or stdout
            if (input_fd != STDIN_FILENO) {
                if (dup2(input_fd, STDIN_FILENO) == -1) {
                    perror("shell");
                    exit(EXIT_FAILURE);
                }
                close(input_fd);
            }

            if (output_fd != STDOUT_FILENO) {
                if (dup2(output_fd, STDOUT_FILENO) == -1) {
                    perror("shell");
                    exit(EXIT_FAILURE);
                }
                close(output_fd);
            }

            // Execute the command
            execvp(args[0], args);

            // If execvp fails
            perror("shell");
            exit(EXIT_FAILURE);
        } else if (pid < 0) {
            // Error forking
            perror("shell");
        } else {
            // Parent process
            int status;
            waitpid(pid, &status, 0);

            if (append) {
                // For append mode, close the file descriptor to prevent further writes
                close(output_fd);
            }
        }
    }
}




int main() {
    char input[MAX_INPUT_SIZE];

    handle_signals();

    while (1) {
        printf("MiniShell> ");
        fgets(input, sizeof(input), stdin);

        // Remove trailing newline
        input[strcspn(input, "\n")] = '\0';

        // Check if the input is empty (just Enter pressed)
        if (strlen(input) == 0) {
            continue;
        }

        pid_t child_pid = fork();

        if (child_pid == 0) {
            // Inside the child process
            handle_signals();
            parse_and_execute(input);
            exit(EXIT_SUCCESS);  // Exit child process
        } else if (child_pid < 0) {
            perror("shell");
        } else {
            // Inside the parent process
            int status;
            waitpid(child_pid, &status, 0);
            if (WIFEXITED(status)) {
                child_exit_status = WEXITSTATUS(status);
            }
        }
    }

    return 0;
}





