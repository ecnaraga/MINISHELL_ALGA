/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 15:35:28 by galambey          #+#    #+#             */
/*   Updated: 2023/12/19 11:59:09 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_exec_cmd_bis(t_msh *msh, int old_stdout, int old_stdin, int sub)
{
	pid_t	pid;
	int		built;

	built = ft_search_builtin(msh);
	if (status == 255 || built == 2)
		ft_exit_bis(msh, sub, old_stdout, old_stdin); // SI ERREUR DE MALLOC DANS UN BUILTIN ON QUITTE LE PROCESS ACTUEL
	if (built == 0)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			status = 255;
			ft_exit_bis(msh, sub, old_stdout, old_stdin); // SI ERREUR DE FORK ON QUITTE LE PROCESS ACTUEL
			return;
		}
		if (pid == 0)
		{
			close(old_stdout);
			close(old_stdin);
			ft_child_exec(msh);
		}
	}
	if (built != 0 || (built == 0 && pid != 0))
	{
		if (dup2(old_stdout, 1) == -1)
		{
			write(2, "dup2 : critical error : failed to open old_stdout", 50);
			status = 255;
			ft_exit_bis(msh, sub, old_stdin, old_stdout);
		}
		if (dup2(old_stdin, 0) == -1)
		{
			write(2, "dup2 : critical error : failed to open old_stdin", 49);
			status = 255;
			ft_exit_bis(msh, sub, old_stdin, old_stdout);
		}
		ft_parent(msh, old_stdout, old_stdin, CMD_ALONE);
	}
}

int ft_return_error(t_msh *msh, int old_std[2], int rule, int sub)
{
	if (old_std[I] > -1)
	{
		if (dup2(old_std[I], 0) == -1)
		{
			write(2, "dup2 : critical error : failed to open old_stdin", 49);
			status = 255;
			ft_exit_bis(msh, sub, old_std[I], old_std[O]);
		}
	}
	if (old_std[O] > -1)
	{
		if (dup2(old_std[O], 1) == -1)
		{
			write(2, "dup2 : critical error : failed to open old_stdout", 50);
			status = 255;
			ft_exit_bis(msh, sub, old_std[I], old_std[O]);
		}
	}
	ft_parent(msh, old_std[I], old_std[O], rule);
	return (status);
}

int	ft_exec_cmd(t_msh *msh, int sub)
{
	int old_std[2];

	old_std[I] = -1;
	old_std[O] = -1;
	old_std[I] = redef_stdin(msh, CMD_ALONE, 0, sub);
	if (old_std[I] == -1) // OK PROTEGE
		return (ft_return_error(msh, old_std, CMD_ALONE, sub));
	old_std[O] = redef_stdout(msh, CMD_ALONE, 0, sub);
	if (old_std[O] == -1) // OK PROTEGE
		return (ft_return_error(msh, old_std, CMD_ALONE, sub));
	msh->p.cmd_opt = ft_make_cmd(msh, sub, old_std[O], old_std[I]); //SI ERREUR DE MALLOC ON QUITTE A L INTERIEUR
	if (!msh->p.cmd_opt) // par exemple dans le cas ou la cmd est envoye est : ""
		return (ft_return_error(msh, old_std, CMD_ALONE, sub));
	if (!msh->p.cmd_opt[0]) // OK PROTEGE
	{
		if (ft_perr(E_NO_CMD, msh->av->data) == 255)
			ft_exit_bis(msh, sub, old_std[I], old_std[O]); //IF ERREUR DE MALLOC ON QUITTE LE PROCESS ACTUEL
		return (ft_return_error(msh, old_std, CMD_ALONE, sub));
	}
	// if (!msh->p.cmd_opt[0][0]) // OK PROTEGE EN COMMENTAIRE CAR JE NE VOIS PAS DANS QUEL CAS
	// 	return (ft_return_error(msh, old_std, CMD_ALONE, sub));
	ft_exec_cmd_bis(msh, old_std[O], old_std[I], sub);
	return (status);
}

int	ft_cmd_alone(t_msh *msh, int sub)
{
	ft_parse(msh, sub); // IF MALLOC KO ON QUITTE A L INTERIEUR
	ft_signal_handler_msh_bis();
	ft_exec_cmd(msh, sub); // OK PROTEGER
	while (wait(&status) > 0)
		;
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	dprintf(2, "status %d\n", status);
	ft_magic_malloc(FLUSH, 0, NULL, PIP);
	return (0);
}
