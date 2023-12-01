/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 10:01:52 by garance           #+#    #+#             */
/*   Updated: 2023/12/01 19:00:16 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_child_exec(t_msh *msh)
{
	int	err;

	err = ft_access_cmd(msh->p.path, msh->p.cmd_opt[0], &msh->p.good_path);
	if (err > 0) //A IMPLEMENTER SI ERREUR
		(ft_perr(err, msh->p.cmd_opt[0]), ft_exit(-1, -1, -1));
	execve(msh->p.good_path, msh->p.cmd_opt, NULL);
	(perror("execve"), ft_exit(-1, -1, -1)); //implementer F_EXIT);
}

//TO DO : free tous les elemnts jusqu au pipe ou prochain operateur
void	ft_parent(pid_t pid, t_msh *msh, int fd_1, int fd_2)
{
	t_split *head;
	
	if (pid != 0)
	{
		if (fd_1 > -1)
			close(fd_1);
		if (fd_2 > -1)
			close(fd_2);
		head = msh->av;
		while (msh->av && msh->av->token != PIPE && msh->av->token != OPERATOR && msh->av->token != PAR_OPEN && msh->av->token != PAR_CLOSE)
		{
			printf("PARENT msh->av->data %s\n", msh->av->data);
			msh->av = ft_lstdel_and_relink_split(msh->av, NULL, &head);
		}
		if (msh->av && msh->av->token == PIPE)
			msh->av = ft_lstdel_and_relink_split(msh->av, NULL, &head);
		if (msh->av)
			printf("END PARENT msh->av->data %s\n", msh->av->data);
	}
}

//redefinir stdout si outfile avant le pipe
void	ft_first_pipe(t_msh *msh)
{
	pid_t	pid;

	// printf("FIRST_PIPE\n");
	pid = fork();
	if (pid == -1)
		perror("fork");
	if (pid == 0)
	{
		close(msh->p.fd_p[0][0]);
		redef_stdin(msh, FIRST, 0);
		redef_stout(msh, FIRST, 0);
		close(msh->p.fd_p[0][1]);
		msh->p.cmd_opt = ft_make_cmd(msh);
		// if (!msh->p.cmd_opt) // NORMALEMENT CAS DE FIGURE GERE DANS FT_MAKE_CMD
		// {
		// 	dprintf(2, "c est  moi first \n");
		// 	ft_exit(-1, -1, -1);
		// }
		if (!msh->p.cmd_opt[0])//cela pourrait il arriver???????????????????????????????????? Pour l instant en commentaire a voir plus tard
		{
			// dprintf(2, "c est  moi first \n");
			(ft_perr(E_NO_CMD, msh->av->data), ft_exit(-1, -1, -1));
		}	
		ft_child_exec(msh);//****************************************************************************
	}
	ft_parent(pid, msh, msh->p.fd_p[0][1], -1);
}

void	ft_middle_pipe(t_msh *msh, int j)
{
	pid_t	pid;

	// printf("MIDDLE_PIPE\n");
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
		msh->p.cmd_opt = ft_make_cmd(msh);
		// if (!msh->p.cmd_opt)
		// {
		// 	dprintf(2, "c est  moi last \n");
		// 	ft_exit(-1, -1, -1); //implementer F_EXIT);
		// }
		if (!msh->p.cmd_opt[0])
			(ft_perr(E_NO_CMD, msh->av->data), ft_exit(-1, -1, -1)); //implementer F_EXIT);
		ft_child_exec(msh);
	}
	ft_parent(pid, msh, msh->p.fd_p[j - 1][0], msh->p.fd_p[j][1]);
}

void	ft_last_pipe(t_msh *msh, int j)
{
	pid_t	pid;

	// printf("LAST_PIPE\n");
	pid = fork();
	if (pid == -1)
		perror("fork");
	if (pid == 0)
	{
		redef_stdin(msh, LAST, j);
		close(msh->p.fd_p[j - 1][0]);
		redef_stout(msh, LAST, j);
		msh->p.cmd_opt = ft_make_cmd(msh);
		// if (!msh->p.cmd_opt)
		// 	ft_exit(-1, -1, -1); //implementer F_EXIT);
		if (!msh->p.cmd_opt[0])
			(ft_perr(E_NO_CMD, msh->av->data), ft_exit(-1, -1, -1)); //implementer F_EXIT);
		ft_child_exec(msh);
	}
	ft_parent(pid, msh, msh->p.fd_p[j - 1][0], -1);
	// printf("msh->av->data %s\n", msh->av->data);
}
