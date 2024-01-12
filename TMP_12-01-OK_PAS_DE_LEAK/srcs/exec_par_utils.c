/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_par_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 16:32:15 by galambey          #+#    #+#             */
/*   Updated: 2024/01/09 16:34:56 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
Return 1 : We go into the loop
Return 0 : We don't
The condition wrote in the condition's loop would be : 
	while (msh->p.hdoc && (ft_strcmp(msh->p.hdoc->name, msh->av->data) != 0
			|| (ft_strcmp(msh->p.hdoc->name, msh->av->data) == 0
				&& msh->p.hdoc->read == 1)))
*/
static int	ft_test_hdoc(t_msh *msh)
{
	if (!msh->p.hdoc)
		return (0);
	if (ft_strcmp(msh->p.hdoc->name, msh->av->data) != 0)
		return (1);
	if (msh->p.hdoc->read != 1)
		return (0);
	if (ft_strcmp(msh->p.hdoc->name, msh->av->data) == 0)
		return (1);
	return (0);
}

void	ft_handle_hdoc(t_msh *msh, t_env *head_hd)
{
	head_hd = msh->p.hdoc;
	while (ft_test_hdoc(msh))
		msh->p.hdoc = msh->p.hdoc->next;
	if (msh->p.hdoc)
		msh->p.hdoc->read = 1;
	msh->p.hdoc = head_hd;
}

void	ft_skip_subelem(t_msh *msh, t_env *head_hd, t_split **head)
{
	int	par;

	par = 0;
	while (msh->av && (msh->av->token != PAR_CLOSE || par != 0))
	{
		if (msh->av->token == PAR_OPEN)
			par++;
		else if (msh->av->token == PAR_CLOSE)
		{
			par--;
			if (par == 0)
				break ;
		}
		else if (msh->av->token == HDOC)
			ft_handle_hdoc(msh, head_hd);
		msh->av = lstdel_relink_split(msh, msh->av, NULL, head);
	}
}
