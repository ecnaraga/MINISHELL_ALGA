/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 10:50:24 by galambey          #+#    #+#             */
/*   Updated: 2023/10/31 13:15:37 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_expand(t_msh *minish)
{
	int	i;

	i = -1;
	while (minish->line[++i])
	{
		if (minish->line[i] == '$')
	}
}
