/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_stdout.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 08:53:13 by garance           #+#    #+#             */
/*   Updated: 2024/01/15 11:01:38 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_dup_pipe(t_msh *msh, int rule, int j)
{
	if (rule == FIRST || rule == MID)
	{
		if (dup2(msh->p.fd_p[j][1], STDOUT_FILENO) == -1)
		{
			perror("dup2");
			msh->status = 1;
			ft_exit(msh->p.fd_p[j][1], -1, -1, msh);
		}
	}
}

/*
fail open gere dans ft_invalid_outfile
*/
static int	ft_open_outfile_tr(t_msh *msh, int *fd_outfile, t_head *save)
{
	if (msh->av->data[0] == '*')
		return (msh->ambiguous = 1, 0);
	if (*fd_outfile != -2)
		close(*fd_outfile);
	if (msh->av->type)
		msh->av->data = ft_expand(msh, msh->av->data, OUTFILE_TRUNC);
	if (msh->status == 255)
		return (255);
	if (!msh->av->data[0] || msh->av->data[0] == '*')
		return (msh->ambiguous = 1, 0);
	*fd_outfile = open(msh->av->data, O_CREAT | O_TRUNC | O_WRONLY, 0744);
	if (*fd_outfile > -1)
		msh->av = lstdel_relink_split(msh, msh->av, save->prev, &save->head);
	return (0);
}

/*
fail open gere dans ft_invalid_outfile
*/
static int	ft_open_outfile_notr(t_msh *msh, int *fd_outfile, t_head *save)
{
	if (msh->av->data[0] == '*')
		return (msh->ambiguous = 1, 0);
	if (*fd_outfile != -2)
		close(*fd_outfile);
	if (msh->av->type)
		msh->av->data = ft_expand(msh, msh->av->data, OUTFILE_NO_TRUNC);
	if (msh->status == 255)
		return (255);
	if (!msh->av->data[0] || msh->av->data[0] == '*')
		return (msh->ambiguous = 1, 0);
	*fd_outfile = open(msh->av->data, O_CREAT | O_APPEND | O_WRONLY, 0744);
	msh->av = lstdel_relink_split(msh, msh->av, save->prev, &save->head);
	return (0);
}

int	redef_stdout(t_msh *msh, int rule, int j, int sub)
{
	t_head	save;
	t_fd	fd;

	ft_init_var_std(&save, &fd, msh);
	ft_dup_pipe(msh, rule, j);
	while (msh->av && msh->av->token != PIPE && msh->av->token != OP
		&& msh->av->token != PAR_CLOSE)
	{
		if (msh->av->token == OUTFILE_TRUNC)
		{
			if (ft_open_outfile_tr(msh, &fd.file, &save) == 255)
				ft_exit_stdout_error_malloc(msh, rule, j, sub);
		}
		else if (msh->av->token == OUTFILE_NO_TRUNC)
		{
			if (ft_open_outfile_notr(msh, &fd.file, &save) == 255)
				ft_exit_stdout_error_malloc(msh, rule, j, sub);
		}
		else
			ft_next(msh, &save);
		if (fd.file == -1 || msh->ambiguous == 1)
			return (ft_invalid_outfile(msh, rule, j, &save));
	}
	msh->av = save.head;
	return (ft_dup_stdout(msh, &fd, rule, j));
}
