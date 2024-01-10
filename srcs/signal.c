/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 11:56:21 by galambey          #+#    #+#             */
/*   Updated: 2024/01/10 16:43:46 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
If SIG_INT is catched: 
- Redisplay the prompt
*/
static void	ft_free(int sig)
{
	if (sig == SIGINT)
	{
		if (g_sign == 0 || g_sign == 2)
		{
			write(2, "\n", 1);
			g_sign = 2;
		}
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

/*
Set the instructions in case of catch Ctrl-C or Ctrl-/
Ctrl-C : free all except environment + redisplay prompt
Ctrl-/ : Do nothing
*/
int	ft_signal_handler_msh(t_msh *msh)
{
	if (signal (SIGINT, &ft_free) == SIG_ERR)
		return (perror("signal"), msh->status = 255);
	if (signal (SIGQUIT, SIG_IGN) == SIG_ERR)
		return (perror("signal"), msh->status = 255);
	if (signal (SIGPIPE, SIG_DFL) == SIG_ERR)
		return (perror("signal"), msh->status = 255);
	return (0);
}

static void	ft_free_bis(int signal)
{
	mcgic(NULL, QUIT, 0, NULL);
	exit(signal + 128);
}

/*
Set the instructions in case of catch Ctrl-C or Ctrl-/
Ctrl-C : in child default behaviour
Ctrl-/ : in child default behaviour
*/
int	ft_signal_handler_msh_child(t_msh *msh)
{
	if (signal(SIGINT, SIG_DFL) == SIG_ERR)
		return (perror("signal"), msh->status = 255);
	if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
		return (perror("signal"), msh->status = 255);
	if (signal(SIGQUIT, &ft_free) == SIG_ERR)
		return (perror("signal"), msh->status = 255);
	if (signal(SIGPIPE, &ft_free_bis) == SIG_ERR)
		return (perror("signal"), msh->status = 255);
	return (0);
}
