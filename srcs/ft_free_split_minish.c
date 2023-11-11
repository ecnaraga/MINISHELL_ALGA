/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_split_minish.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garance <garance@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 09:39:11 by garance           #+#    #+#             */
/*   Updated: 2023/11/11 11:05:47 by garance          ###   ########.fr       */
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
	return (NULL);
}
