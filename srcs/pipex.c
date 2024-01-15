/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 09:52:34 by galambey          #+#    #+#             */
/*   Updated: 2024/01/15 11:02:12 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_count_pipe(t_msh *msh, t_split *head)
{
	int	par;
	int	count;

	par = 0;
	count = 0;
	while (msh->av && msh->av->token != OP)
	{
		if (msh->av->token == PAR_OPEN)
		{
			while (msh->av)
			{
				if (msh->av->token == PAR_OPEN)
					par++;
				else if (msh->av->token == PAR_CLOSE)
					par--;
				if (msh->av->token == PAR_CLOSE && par == 0)
					break ;
				msh->av = msh->av->next;
			}
		}
		else if (msh->av->token == PIPE)
			count += 1;
		msh->av = msh->av->next;
	}
	return (msh->av = head, count);
}

static void	ft_create_fd_p(t_msh *msh, int sub, int size, t_pipex *p)
{
	int	j;

	j = -1;
	p->fd_p = mcgic(mlcp(NULL, sizeof(int *) * (size + 1)), MLC, PIP, msh);
	if (!p->fd_p)
		ft_exit_bis(msh, sub, -1, -1);
	while (++j < size)
	{
		p->fd_p[j] = mcgic(mlcp(NULL, sizeof(int *) * 2), MLC, PIP, msh);
		if (!p->fd_p)
			ft_exit_bis(msh, sub, -1, -1);
	}
	p->fd_p[size] = NULL;
}

static void	ft_pipex(t_msh *msh, size_t nb_pipe, t_index index, t_lpid **pid_l)
{
	while (++index.i < nb_pipe)
	{
		if (pipe(msh->p.fd_p[index.j]) == -1)
		{
			perror("pipe");
			msh->status = 255;
			if (index.j > 0)
				close(msh->p.fd_p[index.j - 1][0]);
			return ;
		}
		if (index.i == 0)
			ft_first_cmd(msh, pid_l);
		else if (index.i >= 1)
			ft_middle_cmd(msh, index.j, pid_l);
		if (msh->status == 255)
			return ;
		ft_close_fd(&msh->fd, 1, -1, -1);
		index.j++;
	}
	ft_last_cmd(msh, index.j, pid_l);
	ft_close_fd(&msh->fd, 2, -1, -1);
}

void	ft_waitpid_loop(t_msh *msh, t_lpid *pid_l)
{
	int	tmp;

	tmp = 0;
	while (pid_l)
	{
		waitpid(pid_l->pid, &msh->status, WUNTRACED);
		if (WIFEXITED(msh->status))
			msh->status = WEXITSTATUS(msh->status);
		else if (WIFSIGNALED(msh->status))
		{
			msh->status = WTERMSIG(msh->status) + 128;
			if (tmp == 0 && msh->status == 131)
			{
				write(2, "Quit (core dumped)\n", 20);
				tmp = 2;
			}
			if (tmp == 0 && msh->status == 130)
			{
				write(2, "\n", 1);
				tmp = 1;
			}
		}
		pid_l = pid_l->next;
	}
}

int	pipex_multi(t_msh *msh, int sub)
{
	size_t	nb_pipe;
	t_split	*head;
	t_index	index;
	t_lpid	*pid_l;
	int		free;

	pid_l = NULL;
	ft_parse(msh, sub);
	head = msh->av;
	nb_pipe = ft_count_pipe(msh, head);
	ft_create_fd_p(msh, sub, nb_pipe, &msh->p);
	index.i = -1;
	index.j = 0;
	msh->sub = sub;
	ft_pipex(msh, nb_pipe, index, &pid_l);
	free = msh->status;
	ft_waitpid_loop(msh, pid_l);
	if (free == 255 || ft_signal_handler_msh(msh) == 255)
	{
		msh->status = 255;
		ft_exit_bis(msh, sub, -1, -1);
	}
	del_env("_", msh, 1);
	return (g_sign = 0, mcgic(NULL, FLUSH, PIP, msh), 0);
}
