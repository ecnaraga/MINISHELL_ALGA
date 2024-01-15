/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_stdin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 08:53:13 by garance           #+#    #+#             */
/*   Updated: 2024/01/15 11:01:37 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_dup_pipe(t_msh *msh, int rule, int j)
{
	if (rule == MID || rule == LAST)
	{
		if (dup2(msh->p.fd_p[j - 1][0], STDIN_FILENO) == -1)
		{
			perror("dup2");
			msh->status = 1;
			if (rule == MID)
				ft_exit(msh->p.fd_p[j - 1][0], msh->p.fd_p[j][1], -1, msh);
			else
				ft_exit(msh->p.fd_p[j - 1][0], -1, -1, msh);
		}
	}
}

/*
fail open gere dans ft_invalid_infile
*/
static int	ft_open_infile(t_msh *msh, int *fd_infile, t_head *save)
{
	char	*tmp;

	if (msh->av->data[0] == '*')
		return (msh->ambiguous = 1, 0);
	if (*fd_infile != -2)
		close(*fd_infile);
	if (msh->av->type)
	{
		tmp = msh->av->data;
		msh->av->data = ft_expand(msh, msh->av->data, INFILE);
	}
	if (msh->status == 255)
		return (255);
	if (!msh->av->data[0] || msh->av->data[0] == '*')
	{
		if (!msh->av->data[0])
			msh->av->data = tmp;
		return (msh->ambiguous = 1, 0);
	}
	*fd_infile = open(msh->av->data, O_RDONLY);
	if (*fd_infile > -1)
		msh->av = lstdel_relink_split(msh, msh->av, save->prev, &save->head);
	return (0);
}

/*
fail open gere dans ft_invalid_infile
*/
static void	ft_open_heredoc(t_msh *msh, int *fd_infile, t_head *save)
{
	if (*fd_infile != -2)
		close(*fd_infile);
	save->head_hd = msh->p.hdoc;
	while (msh->p.hdoc && msh->av && (ft_strcmp(msh->p.hdoc->name,
				msh->av->data) != 0 || (ft_strcmp(msh->p.hdoc->name,
					msh->av->data) == 0 && msh->p.hdoc->read == 1)))
		msh->p.hdoc = msh->p.hdoc->next;
	*fd_infile = open(msh->p.hdoc->content, O_RDONLY);
	msh->p.hdoc->read = 1;
	msh->p.hdoc = save->head_hd;
	save->prev = msh->av;
	msh->av = msh->av->next;
}

/*
Open and close in a loop all the heredoc and infile until a pipe, OP or
	the end of commandline.
The last heredoc or infile encountered is dup2 to stdin_fileno before being
	closed
*/
int	redef_stdin(t_msh *msh, int rule, int j, int sub)
{
	t_fd	fd;
	t_head	save;

	ft_init_var_std(&save, &fd, msh);
	ft_dup_pipe(msh, rule, j);
	while (msh->av && msh->av->token != PIPE && msh->av->token != OP
		&& msh->av->token != PAR_CLOSE)
	{
		if (msh->av->token == INFILE)
		{
			if (ft_open_infile(msh, &fd.file, &save) == 255)
				ft_exit_stdin_error_malloc(msh, rule, j, sub);
		}
		else if (msh->av->token == HDOC)
			ft_open_heredoc(msh, &fd.file, &save);
		else
			ft_next(msh, &save);
		if (fd.file == -1 || msh->ambiguous == 1)
			return (ft_invalid_infile(msh, rule, j, &save));
		if (rule == PAR_OPEN)
			break ;
	}
	msh->av = save.head;
	return (ft_dup_stdin(msh, &fd, rule, j));
}
