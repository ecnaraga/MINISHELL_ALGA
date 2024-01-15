/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_handle_pipe.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 10:01:52 by garance           #+#    #+#             */
/*   Updated: 2024/01/15 11:22:41 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_error_fork(t_msh *msh, int fd1, int fd2, int fd3)
{
	perror("fork");
	if (fd1 > -1)
		close(fd1);
	if (fd2 > -1)
		close(fd2);
	if (fd3 > -1)
		close(fd3);
	msh->status = 255;
}

static void	ft_child_pipe_exec(t_msh *msh, int fd_in, int fd_out)
{
	t_split	*head;
	t_fdpar	fd;

	head = msh->av;
	if (msh->av->token == PAR_OPEN)
	{
		fd.in = fd_in;
		fd.out = fd_out;
		ft_close_fd(&fd, 0, -1, -1);
		(ft_exec_par(msh, &head, 1, &fd), ft_exit(-1, -1, -1, msh));
	}
	else
	{
		ft_close_fd(NULL, -1, fd_in, fd_out);
		msh->p.cmd_t = ft_make_cmd(msh, 1, -1, -1);
		if (!msh->p.cmd_t)
			ft_exit(-1, -1, -1, msh);
		if (!msh->p.cmd_t[0])
			(ft_perr(msh, E_NO_CMD, msh->av->data), ft_exit(-1, -1, -1, msh));
		if (!msh->p.cmd_t[0][0])
			ft_exit(-1, -1, -1, msh);
		if (ft_search_builtin(msh) != 0)
			ft_exit(-1, -1, -1, msh);
		ft_child_exec(msh);
	}
}

void	ft_first_cmd(t_msh *msh, t_lpid **pid_l)
{
	pid_t	pid;
	t_lpid	*new;

	pid = fork();
	if (pid == -1)
		ft_error_fork(msh, msh->p.fd_p[0][0], msh->p.fd_p[0][1], -1);
	else if (pid == 0)
	{
		if (ft_signal_handler_msh_child(msh) == 255)
			ft_exit_bis(msh, 1, msh->p.fd_p[0][0], msh->p.fd_p[0][1]);
		close(msh->p.fd_p[0][0]);
		if (ft_dup_fd(msh, 1) == 1)
			ft_exit_bis(msh, 1, msh->p.fd_p[0][1], -1);
		redef_stdin(msh, FIRST, 0, 1);
		redef_stdout(msh, FIRST, 0, 1);
		ft_child_pipe_exec(msh, -1, msh->p.fd_p[0][1]);
	}
	else
	{
		new = mcgic(mlcp(ft_lpidnew(pid), 1), ADD, PIP, msh);
		if (msh->status == 255)
			return ;
		*pid_l = new;
		ft_parent(msh, msh->p.fd_p[0][1], -1, PIP);
	}
}

void	ft_middle_cmd(t_msh *msh, int j, t_lpid **pid_l)
{
	pid_t	pid;
	t_lpid	*new;

	pid = fork();
	if (pid == -1)
		ft_error_fork(msh, msh->p.fd_p[j][0], msh->p.fd_p[j][1],
			msh->p.fd_p[j - 1][0]);
	else if (pid == 0)
	{
		if (ft_signal_handler_msh_child(msh) == 255)
			(close(msh->p.fd_p[j - 1][0]),
				ft_exit_bis(msh, 1, msh->p.fd_p[j][0], msh->p.fd_p[j][1]));
		ft_close_fd(&msh->fd, 0, msh->p.fd_p[j][0], -1);
		redef_stdin(msh, MID, j, 1);
		redef_stdout(msh, MID, j, 1);
		ft_child_pipe_exec(msh, msh->p.fd_p[j - 1][0], msh->p.fd_p[j][1]);
	}
	else
	{
		new = mcgic(mlcp(ft_lpidnew(pid), 1), ADD, PIP, msh);
		if (msh->status == 255)
			return ;
		ft_lpidadd_back(pid_l, new);
		ft_parent(msh, msh->p.fd_p[j - 1][0], msh->p.fd_p[j][1], PIP);
	}
}

void	ft_last_cmd(t_msh *msh, int j, t_lpid **pid_l)
{
	pid_t	pid;
	t_lpid	*new;

	pid = fork();
	if (pid == -1)
		ft_error_fork(msh, msh->p.fd_p[j - 1][0], -1, -1);
	else if (pid == 0)
	{
		if (ft_signal_handler_msh_child(msh) == 255)
			ft_exit_bis(msh, 1, msh->p.fd_p[j - 1][0], -1);
		redef_stdin(msh, LAST, j, 1);
		if (ft_dup_fd(msh, 2) == 1)
			ft_exit_bis(msh, 1, msh->p.fd_p[j - 1][0], -1);
		redef_stdout(msh, LAST, j, 1);
		ft_child_pipe_exec(msh, msh->p.fd_p[j - 1][0], -1);
	}
	else
	{
		new = mcgic(mlcp(ft_lpidnew(pid), 1), ADD, PIP, msh);
		if (msh->status == 255)
			return ;
		ft_lpidadd_back(pid_l, new);
		ft_parent(msh, msh->p.fd_p[j - 1][0], -1, PIP);
	}
}
