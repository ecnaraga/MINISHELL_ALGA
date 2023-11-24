/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 11:56:21 by galambey          #+#    #+#             */
/*   Updated: 2023/11/24 14:11:51 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_free(int signal)
{
	if (signal == SIGINT)
	{
		ft_magic_malloc(FLUSH, 0, NULL);
		status = 130;
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

int	ft_signal_handler_msh(void)
{
	if (signal (SIGINT, &ft_free) == SIG_ERR)
		printf("ctrl c marche pas\n"); // Revoir message erreur (utiliser perror?)
	if (signal (SIGQUIT, &ft_free) == SIG_ERR)
		printf("ctrl / marche pas\n"); // Revoir message erreur (utiliser perror?)
	return (0); //revoir le code renvoye en cas d erreur
}

