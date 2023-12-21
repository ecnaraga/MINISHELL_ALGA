/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_handle_pipe.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 10:01:52 by garance           #+#    #+#             */
/*   Updated: 2023/12/21 15:13:52 by galambey         ###   ########.fr       */
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

static void	ft_child_pipe_exec(t_msh *msh)
{
	t_split *head;

	head = msh->av;
	if (msh->av->token == PAR_OPEN)
	{
		ft_exec_par(msh, &head, CMD_ALONE, 1); // OK PROTEGER
		ft_exit(-1, -1, -1, msh);
	}
	else
	{
		msh->p.cmd_opt = ft_make_cmd(msh, 1, -1, -1);
		if (!msh->p.cmd_opt) // OK PROTEGER
			ft_exit(-1, -1, -1, msh);
		if (!msh->p.cmd_opt[0]) // OK PROTEGER
			(ft_perr(msh, E_NO_CMD, msh->av->data), ft_exit(-1, -1, -1, msh));
		if (ft_search_builtin(msh) != 0) // OK PROTEGER
			ft_exit(-1, -1, -1, msh);
		ft_child_exec(msh); // OK PROTEGER
	}
}

void	ft_first_pipe(t_msh *msh, t_lpid **pid_l)
{
	pid_t	pid;
	t_lpid	*new;

	pid = fork();
	if (pid == -1) // OK FD CLOSED IN PARENT
		ft_error_fork(msh, msh->p.fd_p[0][0], msh->p.fd_p[0][1], -1);// IF FORK FAILED IN LAST ON RETURN DANS PIPEX MULTI
	else if (pid == 0)
	{
		if (ft_signal_handler_msh_bis() == 1)
			ft_exit_bis(msh, 1, msh->p.fd_p[0][0], msh->p.fd_p[0][1]);
		close(msh->p.fd_p[0][0]);
		redef_stdin(msh, FIRST, 0, 1); // OK PROTEGER
		redef_stdout(msh, FIRST, 0, 1); // OK PROTEGER
		close(msh->p.fd_p[0][1]);
		ft_child_pipe_exec(msh); // OK PROTEGER
	}
	else
	{
		new = mlcgic(mlcp(ft_lpidnew(pid), 1), ADD, PIP, msh);
		// new = ft_magic_malloc(ADD, 0, ft_lpidnew(pid), PIP);
		if (msh->status == 255)
			return;
		*pid_l = new;
		ft_parent(msh, msh->p.fd_p[0][1], -1, PIP);
	}
}

void	ft_middle_pipe(t_msh *msh, int j, t_lpid **pid_l)
{
	pid_t	pid;
	t_lpid	*new;

	pid = fork();
	if (pid == -1) // OK FD CLOSED IN PARENT
		ft_error_fork(msh, msh->p.fd_p[j][0], msh->p.fd_p[j][1], msh->p.fd_p[j - 1][0]);// IF FORK FAILED IN LAST ON RETURN DANS PIPEX MULTI
	else if (pid == 0)
	{
		if (ft_signal_handler_msh_bis() == 1)
			(close(msh->p.fd_p[j - 1][0]),
				ft_exit_bis(msh, 1, msh->p.fd_p[j][0], msh->p.fd_p[j][1]));
		close(msh->p.fd_p[j][0]);
		redef_stdin(msh, MID, j, 1); // OK PROTEGER
		close(msh->p.fd_p[j - 1][0]);
		redef_stdout(msh, MID, j, 1); // OK PROTEGER
		(close(msh->p.fd_p[j][1]), ft_child_pipe_exec(msh)); // OK PROTEGER
	}
	else
	{
		new = mlcgic(mlcp(ft_lpidnew(pid), 1), ADD, PIP, msh);
		// new = ft_magic_malloc(ADD, 0, ft_lpidnew(pid), PIP);
		if (msh->status == 255)
			return;
		ft_lpidadd_back(pid_l, new);
		ft_parent(msh, msh->p.fd_p[j - 1][0], msh->p.fd_p[j][1], PIP);
	}
}

void	ft_last_pipe(t_msh *msh, int j, t_lpid **pid_l)
{
	pid_t	pid;
	t_lpid	*new;

	pid = fork();
	if (pid == -1) // OK FD CLOSED IN PARENT
		ft_error_fork(msh, msh->p.fd_p[j - 1][0], -1, -1);// IF FORK FAILED IN LAST ON RETURN DANS PIPEX MULTI
	else if (pid == 0)
	{
		if (ft_signal_handler_msh_bis() == 1)
			ft_exit_bis(msh, 1, msh->p.fd_p[j - 1][0], -1);
		redef_stdin(msh, LAST, j, 1);
		close(msh->p.fd_p[j - 1][0]);
		redef_stdout(msh, LAST, j, 1); // OK PROTEGER
		ft_child_pipe_exec(msh);
	}
	else
	{
		new = mlcgic(mlcp(ft_lpidnew(pid), 1), ADD, PIP, msh);
		// new = ft_magic_malloc(ADD, 0, ft_lpidnew(pid), PIP);
		if (msh->status == 255)
			return;
		ft_lpidadd_back(pid_l, new);
		ft_parent(msh, msh->p.fd_p[j - 1][0], -1, PIP);
	}
}
