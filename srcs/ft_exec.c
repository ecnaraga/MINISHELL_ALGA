/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 14:20:52 by galambey          #+#    #+#             */
/*   Updated: 2023/12/01 15:54:33 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


int	ft_exec(t_msh *msh)
{
	printf("FT_EXEC\n");
	ft_heredoc(msh);
	printf("FT_MULTI_PIPE\n");
	// pipex_multi(msh);
	ft_cmd_alone(msh);
	printf("TEST MULTI_PIPEX\n");
	return (0);
}