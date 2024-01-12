/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_stdin_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 08:53:13 by garance           #+#    #+#             */
/*   Updated: 2024/01/09 15:42:32 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_exit_invalidfile(t_msh *msh, int rule, int j)
{
	if (rule == CMD_ALONE && msh->status == 255)
		ft_exit(-1, -1, -1, msh);
	if (rule == FIRST)
		ft_exit(msh->p.fd_p[0][1], -1, -1, msh);
	if (rule == MID)
		ft_exit(msh->p.fd_p[j - 1][0], msh->p.fd_p[j][1], -1, msh);
	if (rule == LAST)
		ft_exit(msh->p.fd_p[j - 1][0], -1, -1, msh);
	return (-1);
}

int	ft_invalid_infile(t_msh *msh, int rule, int j, t_head *save)
{
	char	*str;

	str = mcgic(mlcp(ft_strjoin("minishell: ", msh->av->data), 1), ADD, PIP,
			msh);
	if (str && msh->ambiguous == 1)
	{
		msh->status = 1;
		str = mcgic(mlcp(ft_strjoin(str, ": ambiguous redirect\n"), 1), ADD,
				PIP, msh);
		if (str)
			write(2, str, ft_strlen(str));
		msh->av = lstdel_relink_split(msh, msh->av, save->prev, &save->head);
		mcgic(mlcp(str, 0), FREE, PIP, msh);
	}
	else if (str)
	{
		perror(str);
		msh->status = 1;
		msh->av = lstdel_relink_split(msh, msh->av, save->prev, &save->head);
		mcgic(mlcp(str, 0), FREE, PIP, msh);
	}
	return (ft_exit_invalidfile(msh, rule, j));
}

static int	ft_error_duptwo(t_msh *msh, int rule, t_fd fd, int j)
{
	perror("dup2");
	msh->status = 1;
	if (rule == CMD_ALONE)
	{
		close(fd.file);
		close(fd.old_std);
		return (-1);
	}
	if (rule == PAR_OPEN)
		ft_exit(fd.file, -1, -1, msh);
	if (rule == FIRST)
		ft_exit(msh->p.fd_p[0][1], fd.file, -1, msh);
	if (rule == MID)
		ft_exit(msh->p.fd_p[j - 1][0], msh->p.fd_p[j][1], fd.file, msh);
	else
		ft_exit(msh->p.fd_p[j - 1][0], fd.file, -1, msh);
	return (-1);
}

int	ft_dup_stdin(t_msh *msh, t_fd *fd, int rule, int j)
{
	fd->old_std = -2;
	if (rule == CMD_ALONE)
	{
		fd->old_std = dup(STDIN_FILENO);
		if (fd->old_std == -1)
		{
			perror("dup");
			msh->status = 1;
			close(fd->file);
			return (-1);
		}
	}
	if (fd->file != -2 && dup2(fd->file, STDIN_FILENO) == -1)
		return (ft_error_duptwo(msh, rule, *fd, j));
	if (fd->file != -2)
		close(fd->file);
	return (fd->old_std);
}

void	ft_exit_stdin_error_malloc(t_msh *msh, int rule, int j, int sub)
{
	if (sub == 0)
		ft_unlink_heredoc(msh->p.hdoc);
	if (rule == CMD_ALONE || rule == PAR_OPEN)
		ft_exit(-1, -1, -1, msh);
	if (rule == FIRST)
		ft_exit(msh->p.fd_p[0][1], -1, -1, msh);
	if (rule == MID)
		ft_exit(msh->p.fd_p[j - 1][0], msh->p.fd_p[j][1], -1, msh);
	else
		ft_exit(msh->p.fd_p[j - 1][0], -1, -1, msh);
}
