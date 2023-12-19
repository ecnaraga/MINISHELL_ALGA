/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 09:52:34 by galambey          #+#    #+#             */
/*   Updated: 2023/12/19 18:59:33 by galambey         ###   ########.fr       */
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
	p->fd_p = ft_magic_malloc(MALLOC, sizeof(int *) * (size + 1), NULL, PIP);
	if (!p->fd_p)
		ft_exit_bis(msh, sub, -1, -1); // IF MALLOC KO ON QUITTE
	while (++j < size)
	{
		p->fd_p[j] = ft_magic_malloc(MALLOC, sizeof(int) * 2, NULL, PIP);
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
			status = 255;
			if (index.j > 0)
				close(msh->p.fd_p[index.j - 1][0]);
			return ;
		}
		if (index.i == 0)
			ft_first_pipe(msh, pid_l);
		else if (index.i >= 1)
			ft_middle_pipe(msh, index.j, pid_l);
		if (status == 255) // IF FORK FAILED IN FIRST OR MID ON RETURN DANS PIPEX MULTI
		{
			(close(msh->p.fd_p[index.j][0]), close(msh->p.fd_p[index.j][1]));
			if (index.j > 0)
				close(msh->p.fd_p[index.j - 1][0]);
			return ;
		}
		index.j++;
	}
	ft_last_pipe(msh, index.j, pid_l);
}

int	pipex_multi(t_msh *msh, int sub)
{
	size_t	nb_pipe;
	t_split *head;
	t_index	index;
	t_lpid	*pid_l;
	int tmp;
	
	pid_l = NULL;
	ft_parse(msh, sub); // IF MALLOC KO ON QUITTE A L INTERIEUR
	head = msh->av;
	nb_pipe = ft_count_pipe(msh, head);
	ft_create_fd_p(msh, sub, nb_pipe, &msh->p); // IF MALLOC KO ON QUITTE A L INTERIEUR
	// ft_signal_handler_msh_bis();
	index.i = -1;
	index.j = 0;
	ft_pipex(msh, nb_pipe, index, &pid_l);
	tmp = 0;
	while (pid_l)
	{
		waitpid(pid_l->pid, &status, WUNTRACED);
		// dprintf(2, "child status %d\n", WEXITSTATUS(status));
		if (WIFSIGNALED(status))
			status += 128;
		if (status == 130 && tmp == 0)
		{
			write(1, "\n", 1);
			tmp = 1;
		}
		pid_l = pid_l->next;
	}
	// while (wait(&status) > 0) // passer en waitpid
	// 	;

	if (status == 255) // IF PIPE KO OR FORK KO ON QUITTE LE PROCESS ACTUEL
		ft_exit_bis(msh, sub, -1, -1);
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		status += 128;
	ft_signal_handler_msh();
	dprintf(2, "status %d\n", status);
	ft_magic_malloc(FLUSH, 0, NULL, PIP);
	return (0);
}
