/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 10:01:52 by garance           #+#    #+#             */
/*   Updated: 2023/12/18 18:25:40 by galambey         ###   ########.fr       */
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
	// t_split *head;

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
		dprintf(2, "REDEF OK\n");
		ft_child_pipe_exec(msh);
		// dprintf(2, "msh->av->data %s\n", msh->av->data);
		// head = msh->av;
		// if (msh->av->token == PAR_OPEN)
		// {
		// 	ft_exec_par(msh, &head, CMD_ALONE, 1); // OK PROTEGER
		// 	ft_exit(-1, -1, -1);
		// }
		// else
		// {
		// 	msh->p.cmd_opt = ft_make_cmd(msh, 1, -1, -1);
		// 	if (!msh->p.cmd_opt) // OK PROTEGER
		// 		ft_exit(-1, -1, -1);
		// 	if (!msh->p.cmd_opt[0]) // OK PROTEGER
		// 		(ft_perr(E_NO_CMD, msh->av->data), ft_exit(-1, -1, -1));
		// 	if (ft_search_builtin(msh) != 0) // OK PROTEGER
		// 		ft_exit(-1, -1, -1);
		// 	ft_child_exec(msh); // OK PROTEGER
		// }
	}
	else
		ft_parent(msh, msh->p.fd_p[0][1], -1, PIP);
}

void	ft_middle_pipe(t_msh *msh, int j)
{
	pid_t	pid;
	// t_split *head;

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
		ft_child_pipe_exec(msh);
		// head = msh->av;
		// if (msh->av->token == PAR_OPEN)
		// {
		// 	ft_exec_par(msh, &head, CMD_ALONE, 1); // OK PROTEGER
		// 	ft_exit(-1, -1, -1);
		// }
		// else
		// {
		// 	msh->p.cmd_opt = ft_make_cmd(msh, 1, -1, -1); // OK PROTEGER
		// 	if (!msh->p.cmd_opt)
		// 		ft_exit(-1, -1, -1);
		// 	if (!msh->p.cmd_opt[0])
		// 		(ft_perr(E_NO_CMD, msh->av->data), ft_exit(-1, -1, -1)); // OK PROTEGER
		// 	if (ft_search_builtin(msh) != 0)
		// 		ft_exit(-1, -1, -1);
		// 	ft_child_exec(msh);
		// }
	}
	else
		ft_parent(msh, msh->p.fd_p[j - 1][0], msh->p.fd_p[j][1], PIP);
}

void	ft_last_pipe(t_msh *msh, int j)
{
	pid_t	pid;
	// t_split *head;

	// printf("LAST_PIPE\n");
	pid = fork();
	if (pid == -1) // OK FD CLOSED IN PARENT
	{
		perror("fork");
		status = 255;
		return;
	}
	if (pid == 0)
	{
		redef_stdin(msh, LAST, j, 1); // OK PROTEGER
		close(msh->p.fd_p[j - 1][0]);
		redef_stdout(msh, LAST, j, 1); // OK PROTEGER
		ft_child_pipe_exec(msh);
		// head = msh->av;
		// if (msh->av->token == PAR_OPEN)
		// {
		// 	ft_exec_par(msh, &head, CMD_ALONE, 1); // OK PROTEGER
		// 	ft_exit(-1, -1, -1);
		// }
		// else
		// {
		// 	msh->p.cmd_opt = ft_make_cmd(msh, 1, -1, -1); // OK PROTEGER
		// 	if (!msh->p.cmd_opt)
		// 		ft_exit(-1, -1, -1);
		// 	if (!msh->p.cmd_opt[0])
		// 		((E_NO_CMD, msh->av->data), ft_exit(-1, -1, -1));
		// 	if (ft_search_builtin(msh) != 0) // OK PROTEGER
		// 		ft_exit(-1, -1, -1);
			// ft_child_exec(msh);
		// }
	}
	else
		ft_parent(msh, msh->p.fd_p[j - 1][0], -1, PIP);
}
