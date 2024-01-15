/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garance <garance@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 15:16:29 by galambey          #+#    #+#             */
/*   Updated: 2024/01/13 13:02:18 by garance          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_exit_bis(t_msh *msh, int sub, int fd1, int fd2)
{
	ft_close_fd(&msh->fd, 0, fd1, fd2);
	if (sub == 0 && msh->p.hdoc)
		ft_unlink_heredoc(msh->p.hdoc);
	rl_clear_history();
	mcgic(NULL, QUIT, 0, msh);
	exit(msh->status);
}

void	ft_exit(int fd1, int fd2, int fd3, t_msh *msh)
{
	ft_close_fd(&msh->fd, 0, fd1, fd2);
	if (fd3 > -1)
		close(fd3);
	rl_clear_history();
	mcgic(NULL, QUIT, 0, msh);
	exit(msh->status);
}
