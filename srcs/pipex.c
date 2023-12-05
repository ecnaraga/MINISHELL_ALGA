/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 09:52:34 by galambey          #+#    #+#             */
/*   Updated: 2023/12/01 18:39:15 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_init_var(t_pipex *p)
{
	p->good_path = NULL;
	p->cmd_opt = NULL;
	p->fd_p = NULL;
	// p->prompt = 0; //revoir qund mettre le p-> prompt a 0 ou 1 selon la place du here doc est ce que cela change ? 
	// Pour l instant p->prompt set a 1 si presence de heredoc dans ft_heredoc called dans ft_exec
}

void	ft_parse(t_msh *msh)
{
	ft_init_var(&msh->p);
	msh->p.path = ft_research_path(msh->env);
}

static void	ft_create_fd_p(int size, t_pipex *p)
{
	int	j;

	j = -1;
	p->fd_p = ft_magic_malloc(MALLOC, sizeof(int *) * (size + 1), NULL, PIP);
	if (!p->fd_p)
		ft_exit(-1, -1, -1);
	while (++j < size)
	{
		p->fd_p[j] = ft_magic_malloc(MALLOC, sizeof(int) * 2, NULL, PIP);
		if (!p->fd_p)
			ft_exit(-1, -1, -1);
	}
	p->fd_p[size] = NULL;// ??
}

static void	ft_pipex(t_msh *msh)
{
	t_index	index;
	t_split *head;

	index.i = -1;
	index.j = 0;
	index.d = 0;
	head = msh->av;
	while (msh->av && msh->av->token != OPERATOR && msh->av->token != PAR_OPEN && msh->av->token != PAR_CLOSE)
	{
		if (msh->av->token == PIPE)
			index.j += 1;
		msh->av = msh->av->next;
	}
	msh->av = head;
	ft_create_fd_p(index.j, &msh->p);
	while (++index.i < index.j)
	{
		if (pipe(msh->p.fd_p[index.d]) == -1)
		{
			(perror("pipe"));
			if (index.d > 0)
				close(msh->p.fd_p[index.d - 1][0]);
			return ;
		}
		if (index.i == 0)
			ft_first_pipe(msh);//
		else if (index.i >= 1)
			ft_middle_pipe(msh, index.d);//
		index.d++;
	}
	ft_last_pipe(msh, index.d);//
}

int	pipex_multi(t_msh *msh)
{
	ft_parse(msh);
	ft_pipex(msh);
	// sleep(10);
	while (wait(&status) > 0)
		;
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	printf("status %d\n", status);
	// if (msh->p.path)
	// 	ft_free_split(msh->p.path);
	// ft_unlink_heredoc(msh->p.here_doc);
	ft_magic_malloc(FLUSH, 0, NULL, PIP);
	return (0);
}
