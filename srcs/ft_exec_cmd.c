/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 15:35:28 by galambey          #+#    #+#             */
/*   Updated: 2023/12/13 13:13:43 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_exec_cmd_bis(t_msh *msh, int old_stdout, int old_stdin)
{
	pid_t	pid;
	int		built;

	built = ft_search_builtin(msh);
	if (built == 0)
	{
		pid = fork();
		if (pid == -1)
			(dup2(old_stdout, 1), dup2(old_stdin, 0), perror("fork")); // return ? fin propre?
		if (pid == 0)
		{
			close(old_stdout);
			close(old_stdin);
			ft_child_exec(msh);
		}
	}
	if (built != 0 || (built == 0 && pid != 0))
	{
		dup2(old_stdout, 1);
		dup2(old_stdin, 0);
		ft_parent(msh, old_stdout, old_stdin, CMD_ALONE);
	}
}

int	ft_exec_cmd(t_msh *msh)
{
	int	old_stdin;
	int	old_stdout;

	printf("msh->av->data %s\n", msh->av->data);
	old_stdin = redef_stdin(msh, CMD_ALONE, 0);
	if (old_stdin == -1)
		return (ft_parent(msh, -1, -1, CMD_ALONE), status);
	old_stdout = redef_stout(msh, CMD_ALONE, 0);
	if (old_stdout == -1)
		return (dup2(old_stdout, 1), dup2(old_stdin, 0), ft_parent(msh, old_stdin, -1, CMD_ALONE), status);
	msh->p.cmd_opt = ft_make_cmd(msh);
	if (!msh->p.cmd_opt)
		return (dup2(old_stdout, 1), dup2(old_stdin, 0), ft_parent(msh, old_stdin, old_stdout, CMD_ALONE), status);
	if (!msh->p.cmd_opt[0])
	{
		ft_perr(E_NO_CMD, msh->av->data);
		return (dup2(old_stdout, 1), dup2(old_stdin, 0), ft_parent(msh, old_stdout, old_stdin, CMD_ALONE), status);
	}
	if (!msh->p.cmd_opt[0][0])
	{
		return (dup2(old_stdout, 1), dup2(old_stdin, 0), ft_parent(msh, old_stdout, old_stdin, CMD_ALONE), status);
	}
	ft_exec_cmd_bis(msh, old_stdout, old_stdin);
	return (status);
}

int	ft_cmd_alone(t_msh *msh)
{
	ft_parse(msh);
	ft_signal_handler_msh_bis();
	ft_exec_cmd(msh);
	while (wait(&status) > 0)
		;
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	dprintf(2, "status %d\n", status);
	ft_magic_malloc(FLUSH, 0, NULL, PIP);
	return (0);
}
