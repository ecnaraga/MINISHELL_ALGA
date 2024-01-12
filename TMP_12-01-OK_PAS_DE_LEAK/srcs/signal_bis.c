/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_bis.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 11:56:21 by galambey          #+#    #+#             */
/*   Updated: 2024/01/10 17:03:17 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_free(int signal)
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
Ctrl-C : print newline and set global to 1 > it tell that CTRL-C was catched
Ctrl-/ : Do nothing
*/
int	ft_signal_handler_msh_hdoc(t_msh *msh)
{
	if (signal (SIGINT, &ft_free) == SIG_ERR)
		return (perror("signal"), msh->status = 255);
	if (signal (SIGQUIT, SIG_IGN) == SIG_ERR)
		return (perror("signal"), msh->status = 255);
	return (0);
}
