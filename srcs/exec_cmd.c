/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 15:35:28 by galambey          #+#    #+#             */
/*   Updated: 2024/01/09 12:18:28 by athiebau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

pid_t	ft_exec_cmd_fork(t_msh *msh, int old_stdout, int old_stdin, int sub)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		msh->status = 255;
		ft_exit_bis(msh, sub, old_stdout, old_stdin); // SI ERREUR DE FORK ON QUITTE LE PROCESS ACTUEL
	}
	if (pid == 0)
	{
		close(old_stdout);
		close(old_stdin);
		signal(SIGINT, SIG_DFL); // A PROTEGER
		signal(SIGQUIT, SIG_DFL); // A PROTEGER
		signal(SIGQUIT, &ft_free); // A PROTEGER
		ft_child_exec(msh);
	}
	return (pid);
}

void	ft_exec_cmd_bis(t_msh *msh, int old_stdout, int old_stdin, int sub)
{
	pid_t	pid;
	int		built;

	built = ft_search_builtin(msh);
	if (msh->status == 255 || built == 2)
		ft_exit_bis(msh, sub, old_stdout, old_stdin); // SI ERREUR DE MALLOC DANS UN BUILTIN ON QUITTE LE PROCESS ACTUEL
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

int ft_return_error(t_msh *msh, int old_std[2], int rule, int sub)
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
	int old_std[2];

	old_std[I] = -1;
	old_std[O] = -1;
	if (ft_dup_fd(msh, 0) == 1)
		ft_return_error(msh, old_std, CMD_ALONE, sub);
	old_std[I] = redef_stdin(msh, CMD_ALONE, 0, sub);
	if (old_std[I] == -1) // OK PROTEGE
		return (ft_return_error(msh, old_std, CMD_ALONE, sub));
	old_std[O] = redef_stdout(msh, CMD_ALONE, 0, sub);
	if (old_std[O] == -1) // OK PROTEGE
		return (ft_return_error(msh, old_std, CMD_ALONE, sub));
	msh->p.cmd_t = ft_make_cmd(msh, sub, old_std[O], old_std[I]); //SI ERREUR DE MALLOC ON QUITTE A L INTERIEUR
	if (!msh->p.cmd_t) // par exemple dans le cas ou la cmd est envoye est : ""
		return (ft_return_error(msh, old_std, CMD_ALONE, sub));
	if (!msh->p.cmd_t[0]) // OK PROTEGE
	{
		if (ft_perr(msh, E_NO_CMD, msh->av->data) == 255)
			ft_exit_bis(msh, sub, old_std[I], old_std[O]); //IF ERREUR DE MALLOC ON QUITTE LE PROCESS ACTUEL
		return (ft_return_error(msh, old_std, CMD_ALONE, sub));
	}
	if (!msh->p.cmd_t[0][0]) // OK NECESSAIRE cas : $use par exemple
		return (ft_return_error(msh, old_std, CMD_ALONE, sub));
	ft_exec_cmd_bis(msh, old_std[O], old_std[I], sub);
	return (msh->status);
}

int	ft_cmd_alone(t_msh *msh, int sub)
{
	ft_parse(msh, sub); // IF MALLOC KO ON QUITTE A L INTERIEUR
	ft_exec_cmd(msh, sub); // OK PROTEGER
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
	sign = 0;
	del_env("_", msh, 1);
	mcgic(NULL, FLUSH, PIP, msh);
	return (0);
}
