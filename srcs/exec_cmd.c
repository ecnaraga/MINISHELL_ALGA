/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 15:35:28 by galambey          #+#    #+#             */
/*   Updated: 2023/12/01 15:53:26 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_exec_cmd(t_msh *msh)
{
	pid_t	pid;
	
	pid = fork();
	if (pid == -1)
		perror("fork");
	if (pid == 0)
	{
		redef_stdin(msh, CMD_ALONE, 0);
		redef_stout(msh, CMD_ALONE, 0);
		msh->p.cmd_opt = ft_make_cmd(msh);
		// if (!msh->p.cmd_opt) // NORMALEMENT CAS DE FIGURE GERE DANS FT_MAKE_CMD
		// {
		// 	dprintf(2, "c est  moi first \n");
		// 	ft_exit(-1, -1, -1);
		// }
		if (!msh->p.cmd_opt[0])//cela pourrait il arriver???????????????????????????????????? Pour l instant en commentaire a voir plus tard
			(ft_perr(E_NO_CMD, msh->av->data), ft_exit(-1, -1, -1));
		ft_child_exec(msh);//****************************************************************************
	}
	ft_parent(pid, msh, -1, -1);
}

int	ft_cmd_alone(t_msh *msh)
{
	ft_parse(msh);
	ft_exec_cmd(msh);
	// sleep(10);
	while (wait(&status) > 0)
		;
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	printf("status %d\n", status);
	// if (msh->p.path)
	// 	ft_free_split(msh->p.path);
	ft_unlink_heredoc(msh->p.here_doc);
	ft_magic_malloc(FLUSH, 0, NULL, NO_ENV);
	return (0);
}