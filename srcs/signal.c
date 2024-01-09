/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 11:56:21 by galambey          #+#    #+#             */
/*   Updated: 2024/01/09 15:08:28 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
If SIG_INT is catched: 
- Free all the mallocs except the environment
- Set the exit status at 130
- Redisplay the prompt
*/
void	ft_free(int sig)
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
int	ft_signal_handler_msh(void)
{
	if (signal (SIGINT, &ft_free) == SIG_ERR)
		return (perror("signal"), 1);
	if (signal (SIGQUIT, SIG_IGN) == SIG_ERR)
		return (perror("signal"), 1);
	if (signal (SIGPIPE, SIG_DFL) == SIG_ERR)
		return (perror("signal"), 1);
	return (0); //revoir le code renvoye en cas d erreur
}

/*
If SIG_INT is catched: 
- Free all the mallocs except the environment
- Set the exit status at 130
- Redisplay the prompt
*/
void	ft_free_bis(int signal)
{
	if (signal == SIGINT)
	{
		g_sign = 3;
		write(2, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
	}
}

void	ft_free_four(int signal)
{
	mcgic(NULL, QUIT, 0, NULL);
	exit(signal + 128);
}

/*
Set the instructions in case of catch Ctrl-C or Ctrl-/
Ctrl-C : free all except environment + redisplay prompt
Ctrl-/ : Do nothing
*/
int	ft_signal_handler_msh_bis(void)
{
	if (signal(SIGINT, SIG_DFL) == SIG_ERR)
		return (perror("signal"), 1);
	if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
		return (perror("signal"), 1);
	if (signal(SIGQUIT, &ft_free) == SIG_ERR)
		return (perror("signal"), 1);
	if (signal(SIGPIPE, &ft_free_four) == SIG_ERR)
		return (perror("signal"), 1);
	return (0);
}

/*
If SIG_INT is catched: 
- Free all the mallocs except the environment
- Set the exit status at 130
- Redisplay the prompt
*/
void	ft_free_ter(int signal)
{
	if (signal == SIGINT)
	{
		close(STDIN_FILENO);
		printf("\n");
		g_sign = 1;
	}
}

/*
Set the instructions in case of catch Ctrl-C or Ctrl-/
Ctrl-C : free all except environment + redisplay prompt
Ctrl-/ : Do nothing
*/
int	ft_signal_handler_msh_ter(void)
{
	if (signal (SIGINT, &ft_free_ter) == SIG_ERR)
		return (perror("signal"), 1);
	if (signal (SIGQUIT, SIG_IGN) == SIG_ERR)
		return (perror("signal"), 1);
	return (0); //revoir le code renvoye en cas d erreur
}