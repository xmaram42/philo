/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:07 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	mark_nontext(const char *buf, size_t nread)
{
	size_t			i;
	unsigned char	c;

	i = 0;
	while (i < nread)
	{
		c = (unsigned char)buf[i];
		if (c != '\n' && ((c < 32 && c != '\t') || c >= 128))
		{
			set_nontext_input(true);
			break ;
		}
		i++;
	}
}

char	*read_line_non_tty(void)
{
	char		*buf;
	size_t		cap;
	ssize_t		nread;

	buf = NULL;
	cap = 0;
	nread = getline(&buf, &cap, stdin);
	if (nread < 0)
	{
		if (buf)
			free(buf);
		return (NULL);
	}
	mark_nontext(buf, (size_t)nread);
	if (nread > 0 && buf[nread - 1] == '\n')
		buf[nread - 1] = '\0';
	return (buf);
}

void	handle_eof_exit(char **envp, t_node *node)
{
	if (node)
	{
		if (node->pwd)
			free(node->pwd);
		if (node->path_fallback)
			free(node->path_fallback);
	}
	if (envp)
		strarrfree(envp);
	clear_history();
	restore_termios();
	(void)node;
	exit(get_exit_status());
}

int	process_read_line(char **result, char **cur_prompt, char *orig)
{
	char	*line;

	line = readline(*cur_prompt);
	if (!line)
	{
		if (*result)
			free(*result);
		return (-1);
	}
	if (!append_line(result, line))
		return (-1);
	if (quote_check(*result, (int)ft_strlen(*result), NULL) == 0)
		return (0);
	if (*cur_prompt != orig)
		free(*cur_prompt);
	*cur_prompt = ft_strdup("> ");
	return (1);
}

char	*get_line(char *str)
{
	char	*line;
	char	*prompt;

	if (!isatty(STDIN_FILENO))
		return (read_line_non_tty());
	prompt = ft_strdup(str);
	line = get_continuation_line(prompt);
	free(prompt);
	return (line);
}
