/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_loop.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/18 21:27:15 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell.h"

char    *get_heredoc_line(void);
int             check_heredoc_signal(void);

static bool     command_has_non_redir_token(char **args)
{
        int             i;

        i = 0;
        while (args && args[i])
        {
                if (isdlr(args[i]) || isdrr(args[i]) || islr(args[i])
                        || islrr(args[i]) || isrr(args[i]) || istlr(args[i]))
                {
                        if (args[i + 1])
                                i += 2;
                        else
                                i += 1;
                        continue ;
                }
                if (args[i][0])
                        return (true);
                i++;
        }
        return (false);
}

static void     write_heredoc_line(bool expand_vars, char *line,
                char **envp, t_node *node)
{
        char    *expanded_line;

        if (node->redir_fd < 0)
                return ;
        if (expand_vars)
        {
                expanded_line = expand_envvar(line, envp, node);
                ft_putendl_fd(expanded_line, node->redir_fd);
                free(expanded_line);
        }
        else
        {
                ft_putendl_fd(line, node->redir_fd);
                free(line);
        }
}

int     process_heredoc_iteration(char **args, bool has_command,
                char *delimiter, bool expand_vars, char **envp, t_node *node)
{
        char    *line;

        if (check_heredoc_signal())
                return (1);
        line = get_heredoc_line();
        if (!line)
        {
                if (check_heredoc_signal())
                        return (1);
                return (2);
        }
        if (!has_command && ft_strlen(line) == 4
                && ft_strncmp(line, "exit", 5) == 0)
        {
                free(line);
                unlink(".temp");
                cleanup_and_exit(args, envp, node);
        }
        if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
                && ft_strlen(line) == ft_strlen(delimiter))
        {
                free(line);
                return (0);
        }
        write_heredoc_line(expand_vars, line, envp, node);
        return (1);
}

int     process_heredoc_loop(char **args, bool has_command, char *delimiter,
                bool expand_vars, char **envp, t_node *node)
{
        int             lines_read;
        int             result;

        lines_read = 0;
        while (1)
        {
                result = process_heredoc_iteration(args, has_command,
                                delimiter, expand_vars, envp, node);
                if (result == 0)
                        return (result);
                if (result == 1)
                {
                        lines_read++;
                        continue ;
                }
                if (result == 2)
                        break ;
        }
        if (lines_read > 0)
                node->heredoc_swallowed_input = true;
        return (2);
}

int     process_heredoc_input(char **args, bool has_command, char *delimiter,
                bool expand_vars, char **envp, t_node *node)
{
        return (process_heredoc_loop(args, has_command, delimiter,
                        expand_vars, envp, node));
}

int     heredoc_loop(char **args, char **envp, int *i, t_node *node)
{
        char    *delimiter;
        char    *clean_delimiter;
        bool    expand_vars;
        int             unterminated;
        bool    has_command;

        delimiter = args[*i + 1];
        if (delimiter && delimiter[0] == (char)31)
                clean_delimiter = delimiter + 1;
        else
                clean_delimiter = delimiter;
        expand_vars = (!ft_strchr(clean_delimiter, '"')
                        && !ft_strchr(clean_delimiter, '\''));
        has_command = command_has_non_redir_token(args);
        unterminated = process_heredoc_input(args, has_command, clean_delimiter,
                        expand_vars, envp, node);
        if (unterminated)
                node->heredoc_unterminated = true;
        return (unterminated);
}
