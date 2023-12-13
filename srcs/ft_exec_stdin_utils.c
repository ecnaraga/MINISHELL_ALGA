/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_stdin_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 08:53:13 by garance           #+#    #+#             */
/*   Updated: 2023/12/13 13:23:41 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_dup_pipe(t_msh *msh, int rule, int j)
{
	if (rule == MID || rule == LAST)
	{
		if (dup2(msh->p.fd_p[j - 1][0], STDIN_FILENO) == -1)
		{
			perror("dup2");  // EXIT STATUS ?
			if (rule == MID)
				ft_exit(msh->p.fd_p[j - 1][0], msh->p.fd_p[j][1], -1);
			else
				ft_exit(msh->p.fd_p[j - 1][0], -1, -1);
		}
	}
}

/*
fail open gere dans ft_invalid_infile
*/
static void	ft_open_infile(t_msh *msh, int *fd_infile, t_head *save/*  t_split *prev, t_split **head */)
{
	if (*fd_infile != -2)
		close(*fd_infile);
	if (msh->av->type)
		msh->av->data = ft_expand(msh, msh->av->data, INFILE);
	*fd_infile = open(msh->av->data, O_RDONLY);
	if (*fd_infile > -1)
		msh->av = ft_lstdel_and_relink_split(msh->av, save->prev, &save->head);
}

/*
fail open gere dans ft_invalid_infile
*/
static void ft_open_heredoc(t_msh *msh, int *fd_infile, t_head *save)
{
	if (*fd_infile != -2)
		close(*fd_infile);
	save->head_hd = msh->p.here_doc;
	while (msh->p.here_doc && msh->av && (ft_strcmp(msh->p.here_doc->name, msh->av->data) != 0 || (ft_strcmp(msh->p.here_doc->name, msh->av->data) == 0 && msh->p.here_doc->read == 1)))//
		msh->p.here_doc = msh->p.here_doc->next;
	*fd_infile = open(msh->p.here_doc->content, O_RDONLY);
	msh->p.here_doc->read = 1;
	msh->p.here_doc = save->head_hd;
}

int	ft_invalid_infile(t_msh *msh, int rule, int j, t_head *save)
{
	char *str;
	
	dprintf(2, "INVALID\n");
	str = ft_magic_malloc(ADD, 0, ft_strjoin("minishell: ", msh->av->data), PIP);
	//IF ERROR MALLOC POUR LE CMD_ALONE? 
	if (str)
	{
		perror(str);
		status = 1;
		msh->av = ft_lstdel_and_relink_split(msh->av, save->prev, &save->head);
		ft_magic_malloc(FREE, 0, str, PIP);
	}
	if (rule == CMD_ALONE)
		return (-1);
		// ft_exit(-1, -1, -1, rule);
	if (rule == FIRST)
		ft_exit(msh->p.fd_p[0][1], -1, -1);
	if (rule == MID)
		ft_exit(msh->p.fd_p[j - 1][0], msh->p.fd_p[j][1], -1);
	else
		ft_exit(msh->p.fd_p[j - 1][0], -1, -1);
	return (-1);
}

//laisser en static
static int	ft_error_duptwo(t_msh *msh, int rule, t_fd fd, int j)
{
	perror("dup2"); // EXIT STATUS ?
	if (rule == CMD_ALONE)
	{
		close(fd.file);
		close(fd.old_std);
		return (-1);
	}
	if (rule == FIRST)
		ft_exit(msh->p.fd_p[0][1], fd.file, -1);
	if (rule == MID)
		ft_exit(msh->p.fd_p[j - 1][0], msh->p.fd_p[j][1], fd.file);
	else
		ft_exit(msh->p.fd_p[j - 1][0], fd.file, -1);
	return (-1);
}
int	ft_dup_stdin(t_msh *msh, t_fd *fd, int rule, int j)
{
	fd->old_std = -2;
	if (rule == CMD_ALONE)
		fd->old_std = dup(STDIN_FILENO);
	if (fd->file != -2 && dup2(fd->file, STDIN_FILENO) == -1)
		return (ft_error_duptwo(msh, rule, *fd, j));
	if (fd->file != -2)
		close(fd->file);
	return (fd->old_std);
}
/*
Open and close in a loop all the heredoc and infile until a pipe, operator or
	the end of commandline.
The last heredoc or infile encountered is dup2 to stdin_fileno before being
	closed
*/
int	redef_stdin(t_msh *msh, int rule, int j)
{
	t_fd	fd;
	t_head	save;

	save.head = msh->av;
	save.prev = NULL;
	fd.file = -2;
	ft_dup_pipe(msh, rule, j);
	while (msh->av && msh->av->token != PIPE && msh->av->token != OPERATOR && msh->av->token != PAR_CLOSE)
	{
		if (msh->av->token == INFILE)
			ft_open_infile(msh, &fd.file, &save);
		else if (msh->av->token == HERE_DOC)
		{
			ft_open_heredoc(msh, &fd.file, &save);
			save.prev = msh->av;
			msh->av = msh->av->next;
		}
		else
		{
			save.prev = msh->av;
			msh->av = msh->av->next;
		}
		if (fd.file == -1)
			return (ft_invalid_infile(msh, rule, j, &save));
	}
	msh->av = save.head;
	return (ft_dup_stdin(msh, &fd, rule, j));
}

