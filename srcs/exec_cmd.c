/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 15:35:28 by galambey          #+#    #+#             */
/*   Updated: 2024/01/15 10:53:43 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static pid_t	ft_exec_cmd_fork(t_msh *msh, int old_stdout, int old_stdin,
		int sub)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		msh->status = 255;
		ft_exit_bis(msh, sub, old_stdout, old_stdin);
	}
	if (pid == 0)
	{
		close(old_stdout);
		close(old_stdin);
		if (ft_signal_handler_msh_child(msh) == 255)
			ft_exit_bis(msh, 1, -1, -1);
		ft_child_exec(msh);
	}
	return (pid);
}

static void	ft_exec_cmd_bis(t_msh *msh, int old_stdout, int old_stdin, int sub)
{
	pid_t	pid;
	int		built;

	built = ft_search_builtin(msh);
	if (msh->status == 255 || built == 2)
		ft_exit_bis(msh, sub, old_stdout, old_stdin);
	if (built == 0)
		pid = ft_exec_cmd_fork(msh, old_stdout, old_stdin, sub);
	if (built != 0 || (built == 0 && pid != 0))
	{
		if (dup2(old_stdout, 1) == -1)
		{
			write(2, "dup2 : critical error : failed to open old_stdout", 50);
			msh->status = 255;
			ft_exit_bis(msh, sub, old_stdin, old_stdout);
		}
		if (dup2(old_stdin, 0) == -1)
		{
			write(2, "dup2 : critical error : failed to open old_stdin", 49);
			msh->status = 255;
			ft_exit_bis(msh, sub, old_stdin, old_stdout);
		}
		ft_parent(msh, old_stdout, old_stdin, CMD_ALONE);
	}
}

static int	ft_return_error(t_msh *msh, int old_std[2], int rule, int sub)
{
	ft_close_fd(&msh->fd, 0, -1, -1);
	if (old_std[I] > -1)
	{
		if (dup2(old_std[I], 0) == -1)
		{
			write(2, "dup2 : critical error : failed to open old_stdin", 49);
			msh->status = 255;
			ft_exit_bis(msh, sub, old_std[I], old_std[O]);
		}
	}
	if (old_std[O] > -1)
	{
		if (dup2(old_std[O], 1) == -1)
		{
			write(2, "dup2 : critical error : failed to open old_stdout", 50);
			msh->status = 255;
			ft_exit_bis(msh, sub, old_std[I], old_std[O]);
		}
	}
	ft_parent(msh, old_std[I], old_std[O], rule);
	return (msh->status);
}

int	ft_exec_cmd(t_msh *msh, int sub)
{
	int	old_std[2];

	old_std[I] = -1;
	old_std[O] = -1;
	if (ft_dup_fd(msh, 0) == 1)
		ft_return_error(msh, old_std, CMD_ALONE, sub);
	old_std[I] = redef_stdin(msh, CMD_ALONE, 0, sub);
	if (old_std[I] == -1)
		return (ft_return_error(msh, old_std, CMD_ALONE, sub));
	old_std[O] = redef_stdout(msh, CMD_ALONE, 0, sub);
	if (old_std[O] == -1)
		return (ft_return_error(msh, old_std, CMD_ALONE, sub));
	msh->p.cmd_t = ft_make_cmd(msh, sub, old_std[O], old_std[I]);
	if (!msh->p.cmd_t)
		return (ft_return_error(msh, old_std, CMD_ALONE, sub));
	if (!msh->p.cmd_t[0])
	{
		if (ft_perr(msh, E_NO_CMD, msh->av->data) == 255)
			ft_exit_bis(msh, sub, old_std[I], old_std[O]);
		return (ft_return_error(msh, old_std, CMD_ALONE, sub));
	}
	if (!msh->p.cmd_t[0][0])
		return (ft_return_error(msh, old_std, CMD_ALONE, sub));
	ft_exec_cmd_bis(msh, old_std[O], old_std[I], sub);
	return (msh->status);
}

int	ft_cmd_alone(t_msh *msh, int sub)
{
	ft_parse(msh, sub);
	ft_exec_cmd(msh, sub);
	while (wait(&msh->status) > 0)
	{
		if (WIFEXITED(msh->status))
			msh->status = WEXITSTATUS(msh->status);
		else if (WIFSIGNALED(msh->status))
		{
			msh->status = WTERMSIG(msh->status) + 128;
			if (msh->status == 131)
				write(2, "Quit (core dumped)\n", 20);
			else if (msh->status == 130)
				write(2, "\n", 1);
		}
	}
	g_sign = 0;
	del_env("_", msh, 1);
	mcgic(NULL, FLUSH, PIP, msh);
	return (0);
}
