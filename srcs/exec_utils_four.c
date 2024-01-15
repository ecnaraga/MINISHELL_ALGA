/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_four.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 14:20:52 by galambey          #+#    #+#             */
/*   Updated: 2024/01/15 11:01:39 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_skip_par(t_msh *msh)
{
	int	par;

	par = 1;
	msh->av = msh->av->next;
	while (msh->av && par > 0)
	{
		if (msh->av->token == PAR_OPEN)
			(par)++;
		else if (msh->av->token == PAR_CLOSE)
			(par)--;
		msh->av = msh->av->next;
	}
}

int	ft_search_pipe(t_msh *msh)
{
	t_split	*head;

	head = msh->av;
	while (msh->av && msh->av->token != OP)
	{
		if (msh->av->token == PAR_OPEN)
		{
			ft_skip_par(msh);
			if (!msh->av)
				break ;
		}
		if (msh->av->token == PIPE)
			return (msh->av = head, 1);
		if (msh->av->token == OP)
			return (msh->av = head, 0);
		msh->av = msh->av->next;
	}
	return (msh->av = head, 0);
}
