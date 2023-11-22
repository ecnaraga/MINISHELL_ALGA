/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_split_minish.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 09:39:11 by garance           #+#    #+#             */
/*   Updated: 2023/11/21 10:21:30 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	*ft_free_split_msh(t_split *strs)
{
	int	i;

	i = 0;
	while (strs[i].data)
	{
		free(strs[i].data);
		if (strs[i].type)
			free(strs[i].type);
		i++;
	}
	free(strs);
	strs = NULL;
	return (NULL);
}
