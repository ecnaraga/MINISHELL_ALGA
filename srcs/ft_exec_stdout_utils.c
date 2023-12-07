/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_stdout_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 08:53:13 by garance           #+#    #+#             */
/*   Updated: 2023/12/07 17:48:48 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_dup_pipe(t_msh *msh, int rule, int j)
{
	if (rule == FIRST || rule == MID)
	{
		if (dup2(msh->p.fd_p[j][1], STDOUT_FILENO) == -1)
		{
			perror("dup2"); // EXIT STATUS ? 
			ft_exit(msh->p.fd_p[j][1], -1, -1);
		}
	}
}

/*
fail open gere dans ft_invalid_infile
*/
static void	ft_open_outfile_tr(t_msh *msh, int *fd_outfile, t_head *save/*  t_split *prev, t_split **head */)
{
	if (*fd_outfile != -2)
		close(*fd_outfile);
	*fd_outfile = open(msh->av->data, O_CREAT | O_TRUNC | O_WRONLY, 0744);
	if (*fd_outfile > -1)
		msh->av = ft_lstdel_and_relink_split(msh->av, save->prev, &save->head);
}

/*
fail open gere dans ft_invalid_infile
*/
static void	ft_open_outfile_notr(t_msh *msh, int *fd_outfile, t_head *save/*  t_split *prev, t_split **head */)
{
	if (*fd_outfile != -2)
		close(*fd_outfile);
	*fd_outfile = open(msh->av->data, O_CREAT | O_APPEND | O_WRONLY, 0744);
	if (*fd_outfile > -1)
		msh->av = ft_lstdel_and_relink_split(msh->av, save->prev, &save->head);
}

int	ft_invalid_outfile(t_msh *msh, int rule, int j, t_head *save)
{
	char *str;
	
	str = ft_magic_malloc(ADD, 0, ft_strjoin("minishell: ", msh->av->data), PIP);
	if (str)
	{
		perror(str);
		status = 1;
		msh->av = ft_lstdel_and_relink_split(msh->av, save->prev, &save->head);
		ft_magic_malloc(FREE, 0, str, PIP);
	}
	if (rule == CMD_ALONE)
		return (-1);
	if (rule == FIRST)
		ft_exit(msh->p.fd_p[0][1], -1, -1);
	if (rule == MID)
		ft_exit(msh->p.fd_p[j][1], -1, -1);
	else
		ft_exit(-1, -1, -1);
	return (-1);
}

//laisser en static
static int	ft_error_duptwo(t_msh *msh, int rule, t_fd fd, int j)
{
	perror("dup2"); // EXIT STATUS ?
	if (rule == CMD_ALONE)
	{
		close (fd.file);
		close (fd.old_std);
		return (-1);
	}
	if (rule == FIRST)
		ft_exit(msh->p.fd_p[0][1], fd.file, -1);
	if (rule == MID)
		ft_exit(msh->p.fd_p[j][1], fd.file, -1);
	else
		ft_exit(fd.file, -1, -1);
	return (-1);
}

int	ft_dup_stdout(t_msh *msh, t_fd *fd, int rule, int j)
{
	fd->old_std = -2;
	if (rule == CMD_ALONE)
		fd->old_std = dup(STDOUT_FILENO);
	if (fd->file != -2 && dup2(fd->file, STDOUT_FILENO) == -1)
		return (ft_error_duptwo(msh, rule, *fd, j));
	if (fd->file != -2)
		close(fd->file);
	return (fd->old_std);
}

int	redef_stout(t_msh *msh, int rule, int j)
{
	t_head	save;
	t_fd	fd;
	
	save.head = msh->av;
	save.prev = NULL;
	fd.file = -2;
	ft_dup_pipe(msh, rule, j);
	while (msh->av && msh->av->token != PIPE && msh->av->token != OPERATOR /* && msh->av->token != PAR_CLOSE */) //voir si PAR CLOSE OU PAS CAR DANS STDIN PARCLOSE ET PAS LA
	{
		if (msh->av->token == OUTFILE_TRUNC)
			ft_open_outfile_tr(msh, &fd.file, &save);
		else if (msh->av->token == OUTFILE_NO_TRUNC)
			ft_open_outfile_notr(msh, &fd.file, &save);
		else
		{
			save.prev = msh->av;
			msh->av = msh->av->next;
		}
		if (fd.file == -1)
			return (ft_invalid_outfile(msh, rule, j, &save));
	}
	msh->av = save.head;
	return (ft_dup_stdout(msh, &fd, rule, j));
}
