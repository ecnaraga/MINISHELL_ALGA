/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_multi.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 09:52:34 by galambey          #+#    #+#             */
/*   Updated: 2023/11/30 17:12:04 by galambey         ###   ########.fr       */
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
	ft_init_var(&msh->p);
	msh->p.path = ft_research_path(msh->env);
}

static void	ft_create_fd_p(int size, t_pipex *p)
{
	int	j;

	j = -1;
	printf("test121 size = %d\n", size);
	p->fd_p = ft_magic_malloc(MALLOC, sizeof(int *) * (size + 1), NULL, NO_ENV);
	if (!p->fd_p)
		(perror("malloc: error")/*, ft_exit(p, -1, -1, -1)*/, exit(1)); //implementer F_EXIT
	while (++j < size)
	{
		printf("test121\n");
		p->fd_p[j] = ft_magic_malloc(MALLOC, sizeof(int) * 2, NULL, NO_ENV);
		if (!p->fd_p)
			(perror("malloc: error")/*, ft_exit(p, -1, -1, j)*/, exit(1)); //implementer F_EXIT
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
	// printf("test11\n");
	while (msh->av && msh->av->token != OPERATOR)
	{
		if (msh->av->token == PIPE)
			index.j += 1;
		msh->av = msh->av->next;
	}
	msh->av = head;
	// printf("test12\n");
	ft_create_fd_p(index.j, &msh->p);
	// printf("test13\n");
	while (++index.i < index.j)
	{
		// printf("test14\n");
		printf("index.d %d\n", index.d);
		if (pipe(msh->p.fd_p[index.d]) == -1)
		{
			(perror("pipe"));
			if (index.d > 0)
				close(msh->p.fd_p[index.d - 1][0]);
			return ;
		}
		// printf("test15\n");
		if (index.i == 0)
		{
			// printf("test16\n");
			ft_first_pipe(msh);//
			// printf("test17\n");
		}
		else if (index.i >= 1)
		{
			// printf("test18\n");
			ft_middle_pipe(msh, index.d);//
			// printf("test19\n");
		}
		index.d++;
	}
	// printf("test19bis\n");
	ft_last_pipe(msh, index.d);//
	// printf("test19ter\n");
}

int	pipex_multi(t_msh *msh)
{
	// printf("test0\n");
	ft_parse(msh);
	// printf("test1\n");
	ft_pipex(msh);
	// printf("test2\n");
	// sleep(10);
	while (wait(NULL) > 0)
		;
	// printf("test3\n");
	if (msh->p.path)
		ft_free_split(msh->p.path);
	// printf("test4\n");
	// ft_free_fd_p(msh->p.fd_p, -1); // A IMPLEMENTER
	// while (msh->p.here_doc) // A voir ou unlink et free les here doc
	// {
	// 	unlink(msh->p.here_doc->content);
	// 	ft_magic_malloc(FREE, 0, msh->p.here_doc, NO_ENV);
	// }
	return (0);
}
