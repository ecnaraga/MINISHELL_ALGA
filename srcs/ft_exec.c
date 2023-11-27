/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 14:20:52 by galambey          #+#    #+#             */
/*   Updated: 2023/11/27 17:28:15 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


int	ft_exec(t_msh *msh)
{
	t_shell	exec;
	int	i;

	i = -1;
	exec.infile = NULL;
	exec.outfile = NULL;
	exec.cmd = NULL;
	while (msh->av[++i].data)
	{
		if (msh->av[i].token == INFILE || msh->av[i].token == HERE_DOC)
			ft_lstadd_back(&exec.infile, ft_magic_malloc(ADD, 0, lstaddnew(msh->av[i]), NO_ENV));
		else if (msh->av[i].token == OUTFILE_TRUNC || msh->av[i].token == OUTFILE_NO_TRUNC)
			ft_lstadd_back(&exec.outfile, ft_magic_malloc(ADD, 0, lstaddnew(msh->av[i]), NO_ENV));
		else if (msh->av[i].token == CMD)
		{
			while ()
		} 
	}
}