#include "../../includes/minishell.h"

int is_redirection(const char *str) {
    return (strcmp(str, ">") == 0 || strcmp(str, "<") == 0);
}

// Function to count arguments following a redirection operator
void count_args_after_redirection(t_mini *mini, int *index) {
    if (mini->toks[*index] != NULL) {
        (*index)++;
    }
}

// Function to count the total number of arguments in the command
int count_total_arguments(t_mini *mini) {
    int count = 0;
    int index = 0;

    while (mini->toks[index] != NULL) {
        if (strcmp(mini->toks[index], "|") == 0 || is_redirection(mini->toks[index])) {
            break;
        } else {
            count++;
        }
        index++;
    }

    return count;
}

// Function to count the total number of redirections in the command
int count_total_redirections(t_mini *mini) {
    int count = 0;
    int index = 0;

    while (mini->toks[index] != NULL) {
        if (is_redirection(mini->toks[index])) {
            count++;
        }
        index++;
    }

    return count;
}

// Function to initialize the cmds structure
void initialize_cmds(t_cmds *cmds, int args_count, int redirect_count) {
    cmds->cmd = NULL;

    cmds->args = malloc((args_count + 1) * sizeof(char *));
    cmds->args[args_count] = NULL;

    cmds->redirect = malloc(sizeof(t_redirect) * redirect_count);

    cmds->fdi = 0;
    cmds->fdo = 1;
    cmds->next = NULL;
}

// Function to handle adding output and input redirection to the cmds structure
void handle_redirection(t_mini *mini, t_cmds *cmds, int *index) {
    if (strcmp(mini->toks[*index], "<") == 0) {
        cmds->fdi = open_next_file(mini->toks[++(*index)]);
    } else if (strcmp(mini->toks[*index], ">") == 0) {
        cmds->fdo = open_next_file(mini->toks[++(*index)]);
    } else if (strcmp(mini->toks[*index], ">>") == 0) {
        cmds->fdo = open_next_file(mini->toks[++(*index)]);
    }
}

// Function to handle adding the last command in the command sequence to the cmds structure
void handle_last_command(t_mini *mini, t_cmds *cmds, int *index) {
    if (cmds->cmd == NULL) {
        cmds->cmd = strdup(mini->toks[*index]);
    } else {
        int args_count = 0;
        while (cmds->args[args_count] != NULL) {
            args_count++;
        }

        cmds->args = realloc(cmds->args, (args_count + 2) * sizeof(char *));
        cmds->args[args_count] = strdup(mini->toks[*index]);
        cmds->args[args_count + 1] = NULL;
    }

    (*index)++;
}

// Function to create a new command in the linked list of commands
t_cmds *create_new_command() {
    t_cmds *new_cmd = malloc(sizeof(t_cmds));
    if (!new_cmd) {
        exit(EXIT_FAILURE);
    }

    new_cmd->redirect = NULL;
    new_cmd->next = NULL;

    return new_cmd;
}

// Function to initialize the lex struct with appropriate values
void initialize_lex(t_lexer *lex) {
    lex->toks_count = 0;
    lex->args = 0;
    lex->redirect = 0;
}

// Main parser function
int parse_input(t_mini *mini) {
    t_cmds *current_cmd = NULL;
    t_lexer lex;
    int index = 0;

    initialize_lex(&lex);

    while (mini->toks[index] != NULL) {
        if (strcmp(mini->toks[index], "|") == 0) {
            current_cmd = create_new_command();
            initialize_cmds(current_cmd, lex.args, lex.redirect);

            if (mini->cmds == NULL) {
                mini->cmds = current_cmd;
            } else {
                t_cmds *temp = mini->cmds;
                while (temp->next != NULL) {
                    temp = temp->next;
                }
                temp->next = current_cmd;
            }

            initialize_lex(&lex);
            index++;
        } else if (is_redirection(mini->toks[index])) {
            handle_redirection(mini, current_cmd, &index);
            lex.redirect++;
        } else {
            handle_last_command(mini, current_cmd, &index);
            lex.args++;
        }
    }

    // Add the last command if it exists
    if (current_cmd != NULL && current_cmd->cmd != NULL) {
        return 1;
    }

    return -1; // Syntax error
}