/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 14:20:52 by galambey          #+#    #+#             */
/*   Updated: 2024/01/15 11:01:46 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_choice_exec(t_msh *msh, t_split **head, int sub)
{
	if (ft_search_pipe(msh) == 1)
		pipex_multi(msh, sub);
	else if (msh->av->token == PAR_OPEN)
		ft_exec_par(msh, head, sub, NULL);
	else
		ft_cmd_alone(msh, sub);
}

static void	ft_exec_and(t_msh *msh, t_split **head, int sub)
{
	if (msh->status == 0)
	{
		msh->av = msh->av->next;
		ft_choice_exec(msh, head, sub);
		*head = msh->av;
	}
	else
	{
		while (msh->av)
			msh->av = lstdel_relink_split(msh, msh->av, NULL, head);
	}
}

static void	ft_exec_or(t_msh *msh, t_split **head, int sub)
{
	if (msh->status != 0)
	{
		msh->previous_status = msh->status;
		msh->status = 0;
		msh->av = msh->av->next;
		ft_choice_exec(msh, head, sub);
		*head = msh->av;
	}
	else
	{
		while (msh->av)
			msh->av = lstdel_relink_split(msh, msh->av, NULL, head);
	}
}

static int	ft_exec_operator(t_msh *msh, t_split **head, int sub)
{
	ft_choice_exec(msh, head, sub);
	*head = msh->av;
	while (msh->av)
	{
		if (msh->av->token == OP && ft_strcmp(msh->av->data, "&&") == 0)
			ft_exec_and(msh, head, sub);
		else if (msh->av->token == OP
			&& ft_strcmp(msh->av->data, "||") == 0)
			ft_exec_or(msh, head, sub);
		else
			msh->av = msh->av->next;
	}
	return (0);
}

int	ft_exec(t_msh *msh, int sub, t_fdpar *fd)
{
	t_split	*head;

	if (sub == 0 && ft_heredoc(msh) == 130)
		return (1);
	head = msh->av;
	if (fd)
	{
		msh->fd.in = fd->in;
		msh->fd.out = fd->out;
	}
	else
	{
		msh->fd.in = -1;
		msh->fd.out = -1;
	}
	ft_exec_operator(msh, &head, sub);
	if (sub == 0)
		ft_unlink_heredoc(msh->p.hdoc);
	mcgic(NULL, FLUSH, NO_ENV, msh);
	return (0);
}
