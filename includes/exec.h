/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 14:09:00 by galambey          #+#    #+#             */
/*   Updated: 2023/11/28 14:11:47 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "../includes/minishell.h"

/*ft_exec.c*/
int	ft_exec(t_msh *msh);

/*heredoc.c*/
void	ft_heredoc(t_msh *msh);

#endif