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
        // Handle built-in commands
        if (strcmp(args[0], "echo") == 0) {
            for (int i = 1; args[i] != NULL; i++) {
                printf("%s ", args[i]);
            }
            printf("\n");
        } else if (strcmp(args[0], "cd") == 0) {
            if (args[1] != NULL) {
                if (chdir(args[1]) != 0) {
                    perror("shell");
                }
            } else {
                fprintf(stderr, "shell: cd: missing argument\n");
            }
        } else if (strcmp(args[0], "pwd") == 0) {
            char cwd[MAX_PATH_SIZE];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("%s\n", cwd);
            } else {
                perror("shell");
            }
        } else if (strcmp(args[0], "export") == 0) {
            // Implement export
        } else if (strcmp(args[0], "unset") == 0) {
            // Implement unset
        } else if (strcmp(args[0], "env") == 0) {
            // Implement env
        } else if (strcmp(args[0], "exit") == 0) {
            exit(EXIT_SUCCESS);
        } else {
            // Execute external command
            execute_command(args);
        }

        int input_fd = STDIN_FILENO;
        int output_fd = STDOUT_FILENO;

        for (int i = 0; args[i] != NULL; i++) {
            if (strcmp(args[i], "<") == 0) {
                // Input redirection
                if (args[i + 1] != NULL) {
                    input_fd = open(args[i + 1], O_RDONLY);
                    if (input_fd == -1) {
                        perror("shell");
                        return;
                    }
                    // Skip the filename
                    i++;
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
                    // Skip the filename
                    i++;
                } else {
                    fprintf(stderr, "shell: syntax error near unexpected token `>'\n");
                    return;
                }
            } else if (strcmp(args[i], ">>") == 0) {
                // Output redirection (append)
                if (args[i + 1] != NULL) {
                    output_fd = open(args[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
                    if (output_fd == -1) {
                        perror("shell");
                        return;
                    }
                    // Skip the filename
                    i++;
                } else {
                    fprintf(stderr, "shell: syntax error near unexpected token `>>'\n");
                    return;
                }
            } else if (strcmp(args[i], "<<") == 0) {
                // Input redirection until delimiter
                if (args[i + 1] != NULL) {
                    char *delimiter = args[i + 1];

                    // Set input_fd to a temporary file descriptor
                    int temp_fd = mkstemp("/tmp/minishell_tempfile.XXXXXX");
                    if (temp_fd == -1) {
                        perror("shell");
                        return;
                    }

                    // Write input until the delimiter is encountered
                    for (i += 2; args[i] != NULL; i++) {
                        if (strcmp(args[i], delimiter) == 0) {
                            break;
                        }
                        write(temp_fd, args[i], strlen(args[i]));
                        write(temp_fd, " ", 1);  // Add a space between words
                    }

                    // Close the temporary file descriptor
                    close(temp_fd);

                    // Open the temporary file for reading
                    input_fd = open("/tmp/minishell_tempfile.XXXXXX", O_RDONLY);
                    if (input_fd == -1) {
                        perror("shell");
                        return;
                    }
                } else {
                    fprintf(stderr, "shell: syntax error near unexpected token `<<'\n");
                    return;
                }
            }
        }

        // Redirect stdin and/or stdout
        if (input_fd != STDIN_FILENO) {
            if (dup2(input_fd, STDIN_FILENO) == -1) {
                perror("shell");
                exit(EXIT_FAILURE);  // Exit on error
            }
            close(input_fd);
        }

        if (output_fd != STDOUT_FILENO) {
            if (dup2(output_fd, STDOUT_FILENO) == -1) {
                perror("shell");
                exit(EXIT_FAILURE);  // Exit on error
            }
            close(output_fd);
        }

        // Execute the command
        execute_command(args);

        // Reset stdin and stdout for the parent process
        if (dup2(STDIN_FILENO, 0) == -1 || dup2(STDOUT_FILENO, 1) == -1) {
            perror("shell");
            exit(EXIT_FAILURE);  // Exit on error
        }

        exit(EXIT_SUCCESS);  // Exit child process

        // Execute the command
        parse_and_execute(input);

        // Reset stdin and stdout for the parent process
        if (dup2(STDIN_FILENO, 0) == -1 || dup2(STDOUT_FILENO, 1) == -1) {
            perror("shell");
            return;
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

        // Handle quotes
        // ...

        // Handle redirections, pipes, environment variables
        // ...

        // Execute the command
        pid_t child_pid = fork();

        if (child_pid == 0) {
            // Inside the child process
            handle_signals();
            parse_and_execute(input);
            exit(EXIT_SUCCESS);  // Exit child process
        } else if (child_pid > 0) {
            // Inside the parent process
            int status;
            waitpid(child_pid, &status, 0);
            if (WIFEXITED(status)) {
                child_exit_status = WEXITSTATUS(status);
            }
        } else {
            perror("shell");
        }
    }

    return 0;
}



