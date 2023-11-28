/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_multi.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 09:52:34 by galambey          #+#    #+#             */
/*   Updated: 2023/11/28 10:01:33 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

#include "../include/pipex_bonus.h"

static void	ft_init_var(t_pipex *p)
{
	p->good_path = NULL;
	p->cmd_opt = NULL;
	p->fd_p = NULL;
	p->name_here_doc = NULL;
	p->prompt = 0;
}

static void	ft_parse(t_msh *msh, t_pipex *p)
{
	t_split	*head;
	ft_init_var(p);
	head = msh->av;
	while (msh->av || msh->av->token != OPERATOR)
	{
		if (msh->av->token == HERE_DOC)
			ft_prompt(av, p);
		msh->av = msh->av->next;
	}
	p->path = ft_research_path(env);
} //ok

static void	ft_create_fd_p(int size, t_pipex *p)
{
	int	j;

	j = -1;
	p->fd_p = (int **)malloc(sizeof(int *) * (size));
	if (!p->fd_p)
		(perror("malloc: error"), ft_exit(p, -1, -1, -1));
	while (++j < size - 1)
	{
		p->fd_p[j] = (int *)malloc(sizeof(int) * 2);
		if (!p->fd_p)
			(perror("malloc: error"), ft_exit(p, -1, -1, j));
	}
	p->fd_p[size - 1] = NULL;
}

static void	ft_pipex(int ac, char **av, t_pipex *p)
{
	t_index	index;

	index.i = 1;
	index.j = 0;
	ft_create_fd_p(ac - 3 - p->prompt, p);
	while (++index.i + p->prompt < ac - 2)
	{
		if (pipe(p->fd_p[index.j]) == -1)
		{
			(perror("pipe"));
			if (index.j > 0)
				close(p->fd_p[index.j - 1][0]);
			return ;
		}
		if (index.i == 2)
			ft_first_pipe(av, *p);
		else if (index.i + p->prompt >= 2)
			ft_middle_pipe(av, *p, index.j);
		index.j++;
	}
	ft_last_pipe(av, *p, index.j);
}

int	pipex_multi(t_msh *msh)
{
	t_pipex	p;

	ft_parse(msh &p);
	ft_pipex(ac, av, &p);
	while (wait(NULL) > 0)
		;
	if (p.path)
		ft_free_split(p.path);
	ft_free_fd_p(p.fd_p, -1);
	if (p.name_here_doc)
	{
		unlink(p.name_here_doc);
		free(p.name_here_doc);
	}
	exit(0);
}
