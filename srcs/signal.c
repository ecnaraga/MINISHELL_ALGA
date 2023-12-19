/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 11:56:21 by galambey          #+#    #+#             */
/*   Updated: 2023/12/19 18:42:39 by galambey         ###   ########.fr       */
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
		ft_magic_malloc(FLUSH, 0, NULL, 0);
		status = 130;
		// printf("\n");
		write(2, "\n", 1);
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
		printf("ctrl c marche pas\n"); // Revoir message erreur (utiliser perror?)
	if (signal (SIGQUIT, &ft_free) == SIG_ERR)
		printf("ctrl / marche pas\n"); // Revoir message erreur (utiliser perror?)
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
		ft_magic_malloc(FLUSH, 0, NULL, 0);
		status = 130;
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
	if (signal (SIGINT, &ft_free_bis) == SIG_ERR)
		printf("ctrl c marche pas\n"); // Revoir message erreur (utiliser perror?)
	if (signal (SIGQUIT, &ft_free_bis) == SIG_ERR)
		printf("ctrl / marche pas\n"); // Revoir message erreur (utiliser perror?)
	return (0); //revoir le code renvoye en cas d erreur
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
		status = 130;
		ft_magic_malloc(FLUSH, 0, NULL, 0);
		close(STDIN_FILENO);
		printf("\n");
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
		printf("ctrl c marche pas\n"); // Revoir message erreur (utiliser perror?)
	if (signal (SIGQUIT, &ft_free_ter) == SIG_ERR)
		printf("ctrl / marche pas\n"); // Revoir message erreur (utiliser perror?)
	return (0); //revoir le code renvoye en cas d erreur
}