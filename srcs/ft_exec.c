/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 14:20:52 by galambey          #+#    #+#             */
/*   Updated: 2023/11/30 10:53:30 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


int	ft_exec(t_msh *msh)
{
	printf("FT_EXEC\n");
	ft_heredoc(msh);
	pipex_multi(msh);
	printf("TEST MULTI_PIPEX\n");
	return (0);
}