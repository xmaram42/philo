/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stubs.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:08 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

#ifndef BUILD_BONUS

/* removed unused split_operators stub in mandatory build */

char	**subshell(char *str, char **envp, t_node *node)
{
	return (parser(str, envp, node));
}

/* Wildcard stubs */
char	*expand_wildcard_redir(char *pattern, t_node *node)
{
	(void)node;
	return (ft_strdup(pattern));
}

char	**get_file_list(bool hidden)
{
	char	**files;

	(void)hidden;
	files = malloc(sizeof(char *));
	if (!files)
		exit(EXIT_FAILURE);
	files[0] = NULL;
	return (files);
}

#endif
