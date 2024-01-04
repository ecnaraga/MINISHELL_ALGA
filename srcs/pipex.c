/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 09:52:34 by galambey          #+#    #+#             */
/*   Updated: 2024/01/04 10:23:40 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_count_pipe(t_msh *msh, t_split *head)
{
	int par;
	int count;

	par = 0;
	count = 0;
	while (msh->av && msh->av->token != OPERATOR)
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
					break;
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
	p->fd_p = mlcgic(mlcp(NULL, sizeof(int *) * (size + 1)), MALLOC, PIP, msh);
	if (!p->fd_p)
		ft_exit_bis(msh, sub, -1, -1); // IF MALLOC KO ON QUITTE
	while (++j < size)
	{
		p->fd_p[j] = mlcgic(mlcp(NULL, sizeof(int *) * 2), MALLOC, PIP, msh);
		if (!p->fd_p)
			ft_exit_bis(msh, sub, -1, -1); // IF MALLOC KO ON QUITTE
	}
	p->fd_p[size] = NULL;
}

static void	ft_pipex(t_msh *msh, size_t nb_pipe, t_index index, t_lpid **pid_l)
{
	while (++index.i < nb_pipe)
	{
		if (pipe(msh->p.fd_p[index.j]) == -1) // IF PIPE FAIL ON RETURN DANS PIPEX MULTI
		{
			perror("pipe");
			msh->status = 255;
			if (index.j > 0)
				close(msh->p.fd_p[index.j - 1][0]);
			return ;
		}
		if (index.i == 0)
			ft_first_pipe(msh, pid_l);
		else if (index.i >= 1)
			ft_middle_pipe(msh, index.j, pid_l);
		if (msh->status == 255) // IF FORK FAILED IN FIRST OR MID ON RETURN DANS PIPEX MULTI
			return ;
		ft_close_fd(&msh->fd, 1);
		index.j++;
	}
	ft_last_pipe(msh, index.j, pid_l);
	ft_close_fd(&msh->fd, 2);
}

int	pipex_multi(t_msh *msh, int sub)
{
	size_t	nb_pipe;
	t_split *head;
	t_index	index;
	t_lpid	*pid_l;
	int free;
	int tmp;
	
	pid_l = NULL;
	ft_parse(msh, sub); // IF MALLOC KO ON QUITTE A L INTERIEUR
	head = msh->av;
	nb_pipe = ft_count_pipe(msh, head);
	ft_create_fd_p(msh, sub, nb_pipe, &msh->p); // IF MALLOC KO ON QUITTE A L INTERIEUR
	index.i = -1;
	index.j = 0;
	ft_pipex(msh, nb_pipe, index, &pid_l);
	free = msh->status; // IF PIPE KO OR FORK KO ON QUITTE LE PROCESS ACTUEL
	tmp = 0;
	while (pid_l)
	{
		waitpid(pid_l->pid, &msh->status, WUNTRACED);
		if (WIFEXITED(msh->status))
			msh->status = WEXITSTATUS(msh->status);
		else if (WIFSIGNALED(msh->status)/*  && tmp == 0 */)
		{
			msh->status = WTERMSIG(msh->status) + 128;
			if (tmp == 0 && msh->status == 131)
			{
				write(2, "Quit (core dumped)\n", 20);
				tmp = 2;
			}
			if (tmp == 0 && msh->status == 130 && tmp == 0)
			{
				write(2, "\n", 1);
				tmp = 1;
			}
		}
		pid_l = pid_l->next;
	}
	if (free == 255) // IF PIPE KO OR FORK KO DANS F_PIPEX ON QUITTE LE PROCESS ACTUEL
	{
		msh->status = 255;
		ft_exit_bis(msh, sub, -1, -1);
	}
	//dprintf(2, "status = %d\n", msh->status);
	ft_signal_handler_msh();
	sign = 0;
	mlcgic(NULL, FLUSH, PIP, msh);
	return (0);
}
