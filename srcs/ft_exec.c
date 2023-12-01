/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 14:20:52 by galambey          #+#    #+#             */
/*   Updated: 2023/12/01 19:18:11 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_search_pipe(t_msh *msh)
{
	t_split *head;

	head = msh->av;
	while (msh->av && msh->av->token != OPERATOR && msh->av->token != PAR_OPEN && msh->av->token != PAR_CLOSE)
	{
		if (msh->av->token == PIPE)
		{
			msh->av = head;
			return (1);
		}
		msh->av = msh->av->next;
	}
	msh->av = head;
	return (0);
}

int	ft_exec(t_msh *msh)
{
	t_split *head;
	t_split *prev;
	int pipe;
	
	printf("FT_EXEC\n");
	ft_heredoc(msh);
	head = msh->av;
	pipe = 0;
	if (ft_search_pipe(msh) == 1)
		pipex_multi(msh);
	else
		ft_cmd_alone(msh);
	if (msh->av)
		printf("msh->av->data %s\n", msh->av->data);
	head = msh->av;
	while (msh->av)
	{
		if (msh->av->token == OPERATOR && ft_strcmp(msh->av->data, "&&") == 0)
		{
			if (status == 0)
			{
				msh->av = msh->av->next;
				if (ft_search_pipe(msh) == 1)
					pipex_multi(msh);
				else
					ft_cmd_alone(msh);
				if (msh->av)
					printf("msh->av->data %s\n", msh->av->data);
				head = msh->av;
				continue;
			}
			else
			{
				while (msh->av)
					msh->av = ft_lstdel_and_relink_split(msh->av, NULL, &head);
				continue;
			}
		}
		if (msh->av->token == OPERATOR && ft_strcmp(msh->av->data, "||") == 0)
		{
			if (status != 0)
			{
				msh->av = msh->av->next;
				if (ft_search_pipe(msh) == 1)
					pipex_multi(msh);
				else
					ft_cmd_alone(msh);
				if (msh->av)
					printf("msh->av->data %s\n", msh->av->data);
				head = msh->av;
				
				// if (pipe == 0)
				// {
				// 	while (msh->av)
				// 		msh->av = ft_lstdel_and_relink_split(msh->av, prev, &head);
				// 	msh->av = head;
				// 	ft_cmd_alone(msh);
				// 	pipe = 2;
				// 	printf("msh->av->data %s\n", msh->av->data);
				// 	head = msh->av;
				// }
				// else
				// {
				// 	msh->av = ft_lstdel_and_relink_split(msh->av, NULL, &head);
				// 	pipe = 0;
				// }
				continue;
				// msh->av = ft_lstdel_and_relink_split(msh->av, NULL, &head);
				// continue;
			}
			else
			{
				while (msh->av)
					msh->av = ft_lstdel_and_relink_split(msh->av, NULL, &head);
			}
		}
		else
		{
			prev = msh->av;
			msh->av = msh->av->next;
		}
	}
	msh->av = head;
	printf("head %p\n", head);
	// if (head)
	// 	ft_cmd_alone(msh);
	ft_unlink_heredoc(msh->p.here_doc);
	return (0);
}