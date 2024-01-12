/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_std_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 15:27:50 by galambey          #+#    #+#             */
/*   Updated: 2024/01/09 15:34:18 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_init_var_std(t_head *save, t_fd *fd, t_msh *msh)
{
	save->head = msh->av;
	save->prev = NULL;
	fd->file = -2;
	msh->ambiguous = 0;
}

void	ft_next(t_msh *msh, t_head *save)
{
	save->prev = msh->av;
	msh->av = msh->av->next;
}
