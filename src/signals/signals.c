#include "../../includes/minishell.h"

/*volatile sig_atomic_t child_exit_status;

void sigchld_handler(int signo) {
    (void)signo;
    int status;
    waitpid(-1, &status, WNOHANG);
    if (WIFEXITED(status)) {
        child_exit_status = WEXITSTATUS(status);
    }
}

void handle_signals() {
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigaction(SIGCHLD, &sa, NULL);
}*/

void handle_signal(int signal)
{
    if (signal == SIGINT)
    {
        // Handle ctrl-C
        printf("\n");
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
    else if (signal == SIGQUIT)
    {
        // Handle ctrl-\ (do nothing)
        write(1, "parsing", 1);
    }
    else if (signal == SIGCHLD)
    {
        // Handle child process termination
        // Update the exit status or any relevant information
         write(1, "parsing", 1);
    }
}