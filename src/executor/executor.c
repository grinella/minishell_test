#include "../../includes/minishell.h"

/*void execute_commands(t_mini *mini)
{
    t_cmds *current_cmd = mini->cmds;

    while (current_cmd != NULL)
    {
        pid_t pid = fork();
        if (pid == -1)
        {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            if (current_cmd->fdi != 0)
            {
                dup2(current_cmd->fdi, 0);
                close(current_cmd->fdi);
            }
            if (current_cmd->fdo != 1)
            {
                dup2(current_cmd->fdo, 1);
                close(current_cmd->fdo);
            }
            execvp(current_cmd->cmd, current_cmd->args);
            perror("Execution failed");
            exit(EXIT_FAILURE);
        }
        else
        {
            int status;
            waitpid(pid, &status, 0);
            mini->exit_status = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
        }

        current_cmd = current_cmd->next;
    }
}*/
