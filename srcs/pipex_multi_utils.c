/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_multi_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 10:01:52 by garance           #+#    #+#             */
/*   Updated: 2023/11/30 18:31:44 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_child_exec(t_msh *msh)
{
	int	err;

	err = ft_access_cmd(msh->p.path, msh->p.cmd_opt[0], &msh->p.good_path);
	// if (err > 0) //A IMPLEMENTER SI ERREUR
	// 	(ft_perr(err, av[i]), ft_exit(&p, -1, -1, -1));
	// dprintf(2, "PIPE END\n");
	execve(msh->p.good_path, msh->p.cmd_opt, NULL);
	(perror("execve1")/*, ft_exit(&msh->p, -1, -1, -1)*/, exit(1)); //implementer F_EXIT);
}

//TO DO : free tous les elemnts jusqu au pipe ou prochain operateur
static void	ft_parent(pid_t pid, t_msh *msh, int fd_1, int fd_2)
{
	t_split *head;
	
	if (pid != 0)
	{
		if (fd_1 > -1)
			close(fd_1);
		if (fd_2 > -1)
			close(fd_2);
		head = msh->av;
		while (msh->av && msh->av->token != PIPE)
		{
			// printf("test18 FT PARENT 1\n");
			msh->av = ft_lstdel_and_relink_split(msh->av, NULL, &head);
			// msh->av = msh->av->next;
		}
		if (msh->av && msh->av->token == PIPE)
		{
			// printf("test18 FT PARENT 2\n");
			msh->av = ft_lstdel_and_relink_split(msh->av, NULL, &head);
		}
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
		if (dup2(msh->p.fd_p[0][1], STDOUT_FILENO) == -1) //redefinir stdout si outfile avant le pipe
			(perror("dup2")/*, ft_exit(&msh->p, msh->p.fd_p[0][1], -1, -1)*/, exit(1)); //implementer F_EXIT); //penser a free
		close(msh->p.fd_p[0][1]);
		msh->p.cmd_opt = ft_make_cmd(msh);
		// msh->p.cmd_opt = ft_split_iss(av[2 + p.prompt]);
		if (!msh->p.cmd_opt)
			(write(2, "ft_make_cmd: error\n", 21)/*, ft_exit(&msh->p, -1, -1, -1))*/, exit(1)); // implementer ft_exit + penser a free
		if (!msh->p.cmd_opt[0])//cela pourrait il arriver???????????????????????????????????? Pour l instant en commentaire a voir plus tard
			(/*ft_perr(E_NO_CMD, av[2 + msh->p.prompt]), ft_exit(&msh->p, -1, -1, -1)*/exit(1)); //implementer F_EXIT)
		// dprintf(2, "FIRST PIPE -> CHILD EXEC\n");
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
		redef_stdin(msh, MIDLAST, j);
		close(msh->p.fd_p[j - 1][0]);
		if (dup2(msh->p.fd_p[j][1], STDOUT_FILENO) == -1)//implementer redefstdout
			(perror("dup2")/*, ft_exit(&msh->p, msh->p.fd_p[j][1], -1, -1)*/, exit(1)); //implementer F_EXIT);
		close(msh->p.fd_p[j][1]);
		msh->p.cmd_opt = ft_make_cmd(msh);
		if (!msh->p.cmd_opt)
			(write(2, "ft_split_iss: error\n", 21)/*, ft_exit(&msh->p, -1, -1, -1)*/, exit(1)); //implementer F_EXIT);
		if (!msh->p.cmd_opt[0])
			(/*ft_perr(E_NO_CMD, av[2 + j + p.prompt]), ft_exit(&msh->p, -1, -1, -1)*/exit(1)); //implementer F_EXIT);
		ft_child_exec(msh);
	}
	ft_parent(pid, msh, msh->p.fd_p[j - 1][0], msh->p.fd_p[j][1]);
}

void	ft_last_pipe(t_msh *msh, int j)
{
	pid_t	pid;
	int		fd_outfile;

	// printf("LAST_PIPE\n");
	pid = fork();
	if (pid == -1)
		perror("fork");
	if (pid == 0)
	{
		// dprintf(2, "LAST PIPE 1 \n");
		redef_stdin(msh, MIDLAST, j);
		close(msh->p.fd_p[j - 1][0]);
		// dprintf(2, "LAST PIPE 2 \n");
		redef_stout(msh, &fd_outfile);
		// dprintf(2, "LAST PIPE 3 \n");
		// close(fd_outfile);
		// p.cmd_opt = ft_split_iss(av[2 + j + p.prompt]);
		// dprintf(2, "LAST PIPE 4 \n");
		msh->p.cmd_opt = ft_make_cmd(msh);
		// dprintf(2, "LAST PIPE 5 \n");
		if (!msh->p.cmd_opt)
		{
			// dprintf(2, "LAST PIPE 6 \n");
			(write(2, "ft_split_iss: error\n", 21)/*, ft_exit(&msh->p, -1, -1, -1)*/, exit(1)); //implementer F_EXIT);
		}
		if (!msh->p.cmd_opt[0])
		{
			// dprintf(2, "LAST PIPE 7 \n");
			(/*ft_perr(E_NO_CMD, av[2 + j + msh->p.prompt]), ft_exit(&msh->p, -1, -1, -1)*/ exit(1)); //implementer F_EXIT);
		}
		// dprintf(2, "LAST PIPE -> CHILD EXEC\n");
		ft_child_exec(msh);
	}
	ft_parent(pid, msh, msh->p.fd_p[j - 1][0], -1);
}
