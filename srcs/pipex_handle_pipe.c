/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_handle_pipe.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 10:01:52 by garance           #+#    #+#             */
/*   Updated: 2023/12/19 10:37:54 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_child_pipe_exec(t_msh *msh)
{
	t_split *head;

	head = msh->av;
	if (msh->av->token == PAR_OPEN)
	{
		ft_exec_par(msh, &head, CMD_ALONE, 1); // OK PROTEGER
		ft_exit(-1, -1, -1);
	}
	else
	{
		msh->p.cmd_opt = ft_make_cmd(msh, 1, -1, -1);
		if (!msh->p.cmd_opt) // OK PROTEGER
			ft_exit(-1, -1, -1);
		if (!msh->p.cmd_opt[0]) // OK PROTEGER
			(ft_perr(E_NO_CMD, msh->av->data), ft_exit(-1, -1, -1));
		if (ft_search_builtin(msh) != 0) // OK PROTEGER
			ft_exit(-1, -1, -1);
		ft_child_exec(msh); // OK PROTEGER
	}
}

void	ft_first_pipe(t_msh *msh)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1) // OK FD CLOSED IN PARENT
	{
		perror("fork");
		status = 255;
		return ;
	}
	if (pid == 0)
	{
		close(msh->p.fd_p[0][0]);
		redef_stdin(msh, FIRST, 0, 1); // OK PROTEGER
		redef_stdout(msh, FIRST, 0, 1); // OK PROTEGER
		close(msh->p.fd_p[0][1]);
		ft_child_pipe_exec(msh); // OK PROTEGER
	}
	else
		ft_parent(msh, msh->p.fd_p[0][1], -1, PIP);
}

void	ft_middle_pipe(t_msh *msh, int j)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1) // OK FD CLOSED IN PARENT
	{
		perror("fork");
		status = 255;
		return;
	}
	if (pid == 0)
	{
		close(msh->p.fd_p[j][0]);
		redef_stdin(msh, MID, j, 1); // OK PROTEGER
		close(msh->p.fd_p[j - 1][0]);
		redef_stdout(msh, MID, j, 1); // OK PROTEGER
		close(msh->p.fd_p[j][1]);
		ft_child_pipe_exec(msh); // OK PROTEGER
	}
	else
		ft_parent(msh, msh->p.fd_p[j - 1][0], msh->p.fd_p[j][1], PIP);
}

void	ft_last_pipe(t_msh *msh, int j)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1) // OK FD CLOSED IN PARENT
	{
		perror("fork");
		close(msh->p.fd_p[j - 1][0]);
		status = 255;
		return; // IF FORK FAILED IN LAST ON RETURN DANS PIPEX MULTI
	}
	if (pid == 0)
	{
		redef_stdin(msh, LAST, j, 1); // OK PROTEGER
		close(msh->p.fd_p[j - 1][0]);
		redef_stdout(msh, LAST, j, 1); // OK PROTEGER
		ft_child_pipe_exec(msh);
	}
	else
		ft_parent(msh, msh->p.fd_p[j - 1][0], -1, PIP);
}
