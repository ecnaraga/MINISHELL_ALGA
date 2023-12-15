/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 09:52:34 by galambey          #+#    #+#             */
/*   Updated: 2023/12/15 13:16:40 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_init_var(t_pipex *p)
{
	p->good_path = NULL;
	p->cmd_opt = NULL;
	p->fd_p = NULL;
}

void	ft_parse(t_msh *msh, int sub)
{
	ft_init_var(&msh->p);
	msh->p.path = ft_research_path(msh, msh->env, sub); // IF MALLOC KO ON QUITTE A L INTERIEUR
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

static void	ft_pipex(t_msh *msh, int sub)
{
	t_index	index;
	t_split *head;
	int par;

	index.i = -1;
	index.j = 0;
	index.d = 0;
	par = 0;
	head = msh->av;
	// while (msh->av && msh->av->token != OPERATOR && msh->av->token != PAR_OPEN && msh->av->token != PAR_CLOSE)
	// {
	// 	if (msh->av->token == PIPE)
	// 		index.j += 1;
	// 	msh->av = msh->av->next;
	// }
	/*Boucle qui compte le nb de pipe*/
	while (msh->av && msh->av->token != OPERATOR /*&& msh->av->token != PAR_OPEN && msh->av->token != PAR_CLOSE*/)
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
			index.j += 1;
		msh->av = msh->av->next;
	}
	msh->av = head;
	ft_create_fd_p(msh, sub, index.j, &msh->p); // IF MALLOC KO ON QUITTE A L INTERIEUR
	ft_signal_handler_msh_bis();
	while (++index.i < index.j)
	{
		if (pipe(msh->p.fd_p[index.d]) == -1) // IF PIPE FAIL ON RETURN DANS PIPEX MULTI
		{
			perror("pipe");
			status = 255;
			if (index.d > 0)
				close(msh->p.fd_p[index.d - 1][0]);
			return ;
		}
		if (index.i == 0)
			ft_first_pipe(msh); 
		else if (index.i >= 1)
			ft_middle_pipe(msh, index.d);
		if (status == 255) // IF FORK FAILED IN FIRST OR MID ON RETURN DANS PIPEX MULTI
		{
			close(msh->p.fd_p[index.d][0]);
			close(msh->p.fd_p[index.d][1]);
			if (index.d > 0)
				close(msh->p.fd_p[index.d - 1][0]);
			return ;
		}
		index.d++;
	}
	ft_last_pipe(msh, index.d);
	if (status == 255) // IF FORK FAILED IN LAST ON RETURN DANS PIPEX MULTI
	{
		close(msh->p.fd_p[index.d - 1][0]);
		return ;
	}
}

int	pipex_multi(t_msh *msh, int sub)
{
	ft_parse(msh, sub); // IF MALLOC KO ON QUITTE A L INTERIEUR
	ft_pipex(msh, sub);
	// sleep(10);
	while (wait(&status) > 0)
		;
	if (status == 255) // IF PIPE KO OR FORK KO ON QUITTE LE PROCESS ACTUEL
		ft_exit_bis(msh, sub, -1, -1);
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	printf("status %d\n", status);
	// if (msh->p.path)
	// 	ft_free_split(msh->p.path);
	// ft_unlink_heredoc(msh->p.here_doc);
	ft_magic_malloc(FLUSH, 0, NULL, PIP);
	return (0);
}
