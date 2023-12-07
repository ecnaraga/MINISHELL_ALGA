/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 10:01:52 by garance           #+#    #+#             */
/*   Updated: 2023/12/07 15:20:47 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_first_pipe(t_msh *msh)
{
	pid_t	pid;
	t_split *head;

	pid = fork();
	if (pid == -1)
		perror("fork");
	if (pid == 0)
	{
		close(msh->p.fd_p[0][0]);
		redef_stdin(msh, FIRST, 0);
		redef_stout(msh, FIRST, 0);
		close(msh->p.fd_p[0][1]);
		// dprintf(2, "msh->av->data %s\n", msh->av->data);
		head = msh->av;
		if (msh->av->token == PAR_OPEN)
		{
			ft_exec_par(msh, &head, CMD_ALONE);
			ft_exit(-1, -1, -1);
		}
		else
		{
			msh->p.cmd_opt = ft_make_cmd(msh);
			if (!msh->p.cmd_opt)
				ft_exit(-1, -1, -1);
			if (!msh->p.cmd_opt[0])//cela pourrait il arriver???????????????????????????????????? Pour l instant en commentaire a voir plus tard
				(ft_perr(E_NO_CMD, msh->av->data), ft_exit(-1, -1, -1));
			if (ft_search_builtin(msh) == 1)
				ft_exit(-1, -1, -1);
			ft_child_exec(msh);
		}
	}
	else
		ft_parent(msh, msh->p.fd_p[0][1], -1);
}

void	ft_middle_pipe(t_msh *msh, int j)
{
	pid_t	pid;
	t_split *head;

	pid = fork();
	if (pid == -1)
		perror("fork");
	if (pid == 0)
	{
		close(msh->p.fd_p[j][0]);
		redef_stdin(msh, MID, j);
		close(msh->p.fd_p[j - 1][0]);
		redef_stout(msh, MID, j);
		close(msh->p.fd_p[j][1]);
		head = msh->av;
		if (msh->av->token == PAR_OPEN)
		{
			ft_exec_par(msh, &head, CMD_ALONE);
			ft_exit(-1, -1, -1);
		}
		else
		{
			msh->p.cmd_opt = ft_make_cmd(msh);
			if (!msh->p.cmd_opt)
				ft_exit(-1, -1, -1); //implementer F_EXIT);
			if (!msh->p.cmd_opt[0])
				(ft_perr(E_NO_CMD, msh->av->data), ft_exit(-1, -1, -1));
			if (ft_search_builtin(msh) == 1)
				ft_exit(-1, -1, -1);
			ft_child_exec(msh);
		}
	}
	else
		ft_parent(msh, msh->p.fd_p[j - 1][0], msh->p.fd_p[j][1]);
}

void	ft_last_pipe(t_msh *msh, int j)
{
	pid_t	pid;
	t_split *head;

	// printf("LAST_PIPE\n");
	pid = fork();
	if (pid == -1)
		perror("fork");
	if (pid == 0)
	{
		redef_stdin(msh, LAST, j);
		close(msh->p.fd_p[j - 1][0]);
		redef_stout(msh, LAST, j);
		head = msh->av;
		if (msh->av->token == PAR_OPEN)
		{
			ft_exec_par(msh, &head, CMD_ALONE);
			ft_exit(-1, -1, -1);
		}
		else
		{
			msh->p.cmd_opt = ft_make_cmd(msh);
			if (!msh->p.cmd_opt)
				ft_exit(-1, -1, -1);
			if (!msh->p.cmd_opt[0])
				(ft_perr(E_NO_CMD, msh->av->data), ft_exit(-1, -1, -1));
			if (ft_search_builtin(msh) == 1)
				ft_exit(-1, -1, -1);
			ft_child_exec(msh);
		}
	}
	else
		ft_parent(msh, msh->p.fd_p[j - 1][0], -1);
}
