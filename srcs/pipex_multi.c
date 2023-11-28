/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_multi.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 09:52:34 by galambey          #+#    #+#             */
/*   Updated: 2023/11/28 17:20:12 by galambey         ###   ########.fr       */
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

static void	ft_parse(t_msh *msh)
{
	t_split	*head;
	ft_init_var(&msh->p);
	msh->p.path = ft_research_path(msh->env);
}

static void	ft_create_fd_p(int size, t_pipex *p)
{
	int	j;

	j = -1;
	p->fd_p = ft_magic_malloc(MALLOC, sizeof(int *) * (size), NULL, NO_ENV);
	if (!p->fd_p)
		(perror("malloc: error"), ft_exit(p, -1, -1, -1));
	while (++j < size - 1)
	{
		p->fd_p[j] = ft_magic_malloc(MALLOC, sizeof(int) * 2, NULL, NO_ENV);
		if (!p->fd_p)
			(perror("malloc: error"), ft_exit(p, -1, -1, j));
	}
	p->fd_p[size - 1] = NULL;// ??
}

static void	ft_pipex(t_msh *msh, int ac, char **av, t_pipex *p)
{
	t_index	index;
	t_split *head;

	index.i = 0;
	index.j = 0;
	index.d = 0;
	head = msh->av;
	while (msh->av && msh->av->token != OPERATOR)
		if (msh->av->token == PIPE)
			index.d += 1;
	msh->av = head;
	ft_create_fd_p(index.d, &msh->p);
	while (++index.i < index.d)
	{
		if (pipe(p->fd_p[index.j]) == -1)
		{
			(perror("pipe"));
			if (index.j > 0)
				close(p->fd_p[index.j - 1][0]);
			return ;
		}
		if (index.i == 0)
			ft_first_pipe(msh, av, *p);//
		else if (index.i >= 1)
			ft_middle_pipe(av, *p, index.j);//
		index.j++;
	}
	ft_last_pipe(av, *p, index.j);//
}

int	pipex_multi(t_msh *msh)
{
	ft_parse(msh);
	ft_pipex(msh);
	while (wait(NULL) > 0)
		;
	if (msh->p.path)
		ft_free_split(msh->p.path);
	ft_free_fd_p(msh->p.fd_p, -1);
	// while (msh->p.here_doc) // A voir ou unlink et free les here doc
	// {
	// 	unlink(msh->p.here_doc->content);
	// 	ft_magic_malloc(FREE, 0, msh->p.here_doc, NO_ENV);
	// }
	exit(0);
}
