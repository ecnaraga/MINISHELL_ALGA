/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 11:56:21 by galambey          #+#    #+#             */
/*   Updated: 2023/12/21 15:09:41 by galambey         ###   ########.fr       */
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
		// printf("NORMAL\n");
		// ft_magic_malloc(FLUSH, 0, NULL, 0); // a mettre avec update status
		// status = 130;
		if (sign == 0 || sign == 2)
		{
			write(2, "\n", 1);
			sign = 2;
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
		// ft_magic_malloc(FLUSH, 0, NULL, 0); // a mettre avec update status
		sign = 3;
		// status = 130;
		write(2, "\n", 1);
		// exit(130);
		rl_on_new_line();
		rl_replace_line("", 0);
		// rl_redisplay();
	}
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
		// printf("WTF\n");
		// status = 130;
		// ft_magic_malloc(FLUSH, 0, NULL, 0); // a mettre avec update status
		close(STDIN_FILENO);
		printf("\n");
		// signal(SIGINT, SIG_DFL);
		sign = 1;
		// rl_on_new_line();
		// rl_replace_line("", 0);
		// rl_redisplay();
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