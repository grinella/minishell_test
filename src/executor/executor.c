#include "../../includes/minishell.h"

/*void execute_commands(t_mini *mini) {
    t_cmds *cmd = mini->cmds;
    int saved_stdin = dup(STDIN_FILENO);
    int saved_stdout = dup(STDOUT_FILENO);

    while (cmd) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            if (cmd->redirect) {
                if (cmd->redirect->infile) {
                    dup2(mini->fdin, STDIN_FILENO);
                    close(mini->fdin);
                }
                if (cmd->redirect->outfile) {
                    dup2(mini->fdout, STDOUT_FILENO);
                    close(mini->fdout);
                }
            }

            execvp(cmd->cmd, cmd->args);
            perror("Execvp failed");
            fprintf(stderr, "Command: %s\n", cmd->cmd);
            fprintf(stderr, "Arguments:\n");
            char **args = cmd->args;
            while (*args) {
                fprintf(stderr, "  %s\n", *args++);
            }
            exit(EXIT_FAILURE);
        } else {
            int status;
            waitpid(pid, &status, 0);

            if (WIFEXITED(status)) {
                printf("Exit Status: %d\n", WEXITSTATUS(status));
            } else if (WIFSIGNALED(status)) {
                printf("Terminated by Signal: %d\n", WTERMSIG(status));
            } else {
                printf("Unknown termination\n");
            }
            //exit(EXIT_SUCCESS);
            
            }

        cmd = cmd->next;
    }
 
    dup2(saved_stdin, STDIN_FILENO);
    dup2(saved_stdout, STDOUT_FILENO);

    close(saved_stdin);
    close(saved_stdout);
}*/

