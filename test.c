#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

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
    while (token != NULL) {
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

        // Handle redirections
        // ...

        // Handle pipes
        // ...

        // Handle environment variables
        // ...

        // Execute the command
        parse_and_execute(input);
    }

    return 0;
}
