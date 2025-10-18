/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_quotes_helpers.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:09 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_quote_state(char c, bool *in_single, bool *in_double)
{
	if (c == '\'' && !*in_double)
		*in_single = !*in_single;
	else if (c == '"' && !*in_single)
		*in_double = !*in_double;
}

static int	count_valid_chars(char *str, t_node *node)
{
	int		i;
	int		l;
	bool	in_single;
	bool	in_double;

	(void)node;
	i = -1;
	l = 0;
	in_single = false;
	in_double = false;
	while (str[++i])
	{
		if ((str[i] == '\'' && !in_double) || (str[i] == '"' && !in_single))
			handle_quote_state(str[i], &in_single, &in_double);
		else if (str[i] == '\\' && !in_single && !in_double)
			continue ;
		else
			l++;
	}
	return (l);
}

static char	*alloc_new_string(char *str, t_node *node)
{
	char	*newstr;
	int		l;

	if (str && str[0] == (char)WILDMARK)
		return (return_marked_unchanged(str));
	l = count_valid_chars(str, node);
	newstr = malloc(((size_t)l + 1) * sizeof(char));
	if (!newstr)
		return (free(str), exit(EXIT_FAILURE), NULL);
	return (newstr);
}

static void	process_char(char c, char *newstr, t_quote_state *state)
{
	handle_quote_state(c, &state->in_single, &state->in_double);
	if ((c == '\'' && !state->in_double) || (c == '"' && !state->in_single))
		return ;
	else if (c == '\\' && !state->in_single && !state->in_double)
		return ;
	else
		newstr[state->j++] = c;
}

char	*process_quotes(char *str, t_node *node)
{
	char			*newstr;
	int				i;
	t_quote_state	state;

	newstr = alloc_new_string(str, node);
	if (str && str[0] == (char)WILDMARK)
		return (newstr);
	i = -1;
	state.j = 0;
	state.in_single = false;
	state.in_double = false;
	while (str[++i])
		process_char(str[i], newstr, &state);
	return (newstr[state.j] = '\0', free(str), newstr);
}
