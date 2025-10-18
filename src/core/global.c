/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:07 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

#include <signal.h>

/* Single global slot for both signal number and last exit status */

static struct s_global_slots	*slots(void)
{
	static struct s_global_slots	state = {0, 0};

	return (&state);
}

int	get_signal_number(void)
{
	return ((int)slots()->signal_number);
}

void	clear_signal_number(void)
{
	slots()->signal_number = 0;
}

void	set_signal_number(int sig)
{
	slots()->signal_number = (sig_atomic_t)sig;
}

/* internal helper to get/set exit status without adding globals elsewhere */
int	_ms_exit_status(int op, int value)
{
	if (op)
		slots()->exit_status = value;
	return (slots()->exit_status);
}
