/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_par.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 14:20:52 by galambey          #+#    #+#             */
/*   Updated: 2024/01/12 17:08:16 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_redef_std_sub(t_msh *msh, t_fdpar *fd)
{
	while (msh->av && msh->av->token != OPERATOR)
	{
		if (msh->av->token == HDOC)
		{
			ft_close_fd(fd, 1, -1, -1);
			if (redef_stdin(msh, PAR_OPEN, 0, 1) == -1)
				ft_exit(-1, -1, -1, msh);
			msh->av = msh->av->next;
		}
		else if (msh->av->token == INFILE)
		{
			ft_close_fd(fd, 1, -1, -1);
			if (redef_stdin(msh, PAR_OPEN, 0, 1) == -1)
				ft_exit(-1, -1, -1, msh);
		}
		else if (msh->av->token == OUTFILE_TRUNC
			|| msh->av->token == OUTFILE_NO_TRUNC)
		{
			ft_close_fd(fd, 2, -1, -1);
			if (redef_stdout(msh, PAR_OPEN, 0, 1) == -1)
				ft_exit(-1, -1, -1, msh);
		}
		else
			msh->av = msh->av->next;
	}
}

static void	ft_init_var(t_msh *msh, t_msh *sub_msh, int sub)
{
	sub_msh->line = ft_strtrim_msh(msh, &msh->line, sub);
	sub_msh->env = msh->env;
	sub_msh->status = msh->status;
	sub_msh->export_env = msh->export_env;
}

static void	ft_create_sub_msh(t_msh *sub_msh, t_msh *msh, int sub, t_fdpar *fd)
{
	pid_t	pid;

	ft_init_var(msh, sub_msh, sub);
	pid = fork();
	if (pid == -1)
	{
		if (fd && fd->in > -1)
			close(fd->in);
		if (fd && fd->out > -1)
			close(fd->out);
		msh->status = 255;
		ft_exit_bis(msh, sub, -1, -1);
	}
	else if (pid == 0)
	{
		msh->sub = sub;
		// if (fd)
		// {
		// 	sub_msh->fd.in = fd->in;
		// 	sub_msh->fd.out = fd->out;
		// 	dprintf(2, "sub_msh->fd.in %d sub_msh->fd.out %d\n", sub_msh->fd.in, sub_msh->fd.out);
		// }
		// dprintf(2, "EXEC_PAR PID = %d sub->msh->line |%s|\n", getpid(), sub_msh->line);
		(ft_redef_std_sub(msh, fd), ft_minishell(sub_msh, 1, fd));
	}
	else
	{
		if (fd && fd->in > -1)
			close(fd->in);
		if (fd && fd->out > -1)
			close(fd->out);
	}
}

void	ft_exec_par(t_msh *msh, t_split **head, int sub, t_fdpar *fd)
{
	t_msh	sub_msh;
	t_env	*head_hd;

	dprintf(2, "EXEC PAR msh->fd.in %d msh->fd.out %d\n", msh->fd.in, msh->fd.out);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	sub_msh.p.hdoc = ft_copy_heredoc(msh, msh->p.hdoc, sub);
	*head = msh->av;
	head_hd = NULL;
	ft_skip_subelem(msh, head_hd, head);
	while (msh->av && msh->av->token == PAR_CLOSE)
		msh->av = lstdel_relink_split(msh, msh->av, NULL, head);
	ft_create_sub_msh(&sub_msh, msh, sub, fd);
	while (msh->av && msh->av->token != OPERATOR)
	{
		if (msh->av->token == HDOC)
			ft_handle_hdoc(msh, head_hd);
		msh->av = lstdel_relink_split(msh, msh->av, NULL, head);
	}
	while (wait(&msh->status) > 0)
		;
	if (WIFEXITED(msh->status))
		msh->status = WEXITSTATUS(msh->status);
	msh->av = *head;
}
