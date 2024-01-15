/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_make_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 10:13:19 by galambey          #+#    #+#             */
/*   Updated: 2024/01/15 11:01:34 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
Count the numbers of elements that we will put in the cmd (cmd + option +
	arguments)
*/
static int	ft_count_cmd(t_msh *msh)
{
	t_split	*head;
	int		count;

	head = msh->av;
	count = 0;
	while (msh->av && msh->av->token != PIPE && msh->av->token != OP)
	{
		if (msh->av->token == CMD)
			count++;
		msh->av = msh->av->next;
	}
	msh->av = head;
	if (count == 0)
		return (-1);
	return (count);
}

static int	ft_copy_cmd(t_msh *msh, t_head *save, int *i, int *cmd_nb)
{
	if (msh->av->type)
	{
		msh->p.cmd_t = ft_handle_expand(msh, i, cmd_nb);
		if (msh->status == 255)
			return (255);
		if (ck_char(msh->p.cmd_t[*i], '*'))
		{
			ft_reset_msh_av(msh, msh->p.cmd_t[*i]);
			if (msh->status == 255)
				return (255);
			return (0);
		}
	}
	else if (ck_char(msh->av->data, '*'))
		msh->p.cmd_t = ft_expand_wildcard(msh, cmd_nb, i);
	else
		msh->p.cmd_t[*i] = mcgic(mlcp(ft_strdup(msh->av->data), 1), ADD, PIP,
				msh);
	if (msh->status == 255)
		return (255);
	(*i)++;
	msh->av = lstdel_relink_split(msh, msh->av, save->prev, &save->head);
	return (0);
}

static int	ft_build_cmd(t_msh *msh, t_head *save, int *cmd_nb)
{
	int	i;

	i = 0;
	while (msh->av && msh->av->token != PIPE && msh->av->token != OP)
	{
		if (msh->av->token == CMD)
		{
			if (ft_copy_cmd(msh, save, &i, cmd_nb) == 255)
				return (255);
		}
		else
		{
			save->prev = msh->av;
			msh->av = msh->av->next;
		}
	}
	return (0);
}

/*
Create the tab of string containing the cmd + option + arguments
*/
char	**ft_make_cmd(t_msh *msh, int sub, int fd1, int fd2)
{
	t_head	save;
	int		cmd_nb;

	save.head = msh->av;
	save.prev = NULL;
	cmd_nb = ft_count_cmd(msh);
	if (cmd_nb == -1)
	{
		if (msh->av && msh->av->quote)
			write(2, "minishell: : command not found\n", 32);
		return (NULL);
	}
	msh->p.cmd_t = mcgic(mlcp(NULL, sizeof(char *) * (cmd_nb + 1)), MLC, PIP,
			msh);
	if (!msh->p.cmd_t)
		ft_exit_bis(msh, sub, fd1, fd2);
	msh->p.cmd_t[0] = NULL;
	if (ft_build_cmd(msh, &save, &cmd_nb) == 255)
		ft_exit_bis(msh, sub, fd1, fd2);
	msh->p.cmd_t[cmd_nb] = NULL;
	msh->av = save.head;
	return (msh->p.cmd_t);
}
