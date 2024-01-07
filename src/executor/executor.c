#include "../../includes/minishell.h"

void execute_commands(t_mini *mini) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        execvp(mini->cmds->cmd, mini->cmds->args);
        perror("Execvp failed");
        exit(EXIT_FAILURE);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            printf("PORCODIO %d\n", WEXITSTATUS(status));
        } else {
            printf("PORCODIO\n");
        }
    }
}
