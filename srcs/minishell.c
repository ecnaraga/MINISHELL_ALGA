/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 15:10:21 by galambey          #+#    #+#             */
/*   Updated: 2024/01/15 10:54:02 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
Call all the parsing function + split the line + tokenisation
*/
static int	ft_parsing(t_msh *msh, int sub)
{
	if (sub == 0 && ft_parse_line(msh) != 0)
		return (mcgic(NULL, FLUSH, NO_ENV, msh), 1);
	if (sub == 0 && ft_parse_par(msh) != 0)
		return (mcgic(NULL, FLUSH, NO_ENV, msh), 1);
	msh->av = ft_split_msh(msh->line, msh);
	if (!msh->av)
		return (mcgic(NULL, FLUSH, NO_ENV, msh), 1);
	msh->ac = ft_lstsize_split(msh->av);
	if (msh->ac == 0)
		return (mcgic(NULL, FLUSH, NO_ENV, msh), 1);
	ft_token(msh);
	if (sub == 0 && ft_parse_error_token(msh) != 0)
		return (mcgic(NULL, FLUSH, NO_ENV, msh), 1);
	return (0);
}

void	ft_minishell(t_msh *msh, int sub, t_fdpar *fd)
{
	if (ft_parsing(msh, sub) != 0)
		return ;
	if (ft_exec(msh, sub, fd) != 0)
		return ;
	if (sub != 0)
		ft_exit(-1, -1, -1, msh);
}
