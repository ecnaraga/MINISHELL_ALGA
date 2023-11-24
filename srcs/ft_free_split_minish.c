/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_split_minish.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 09:39:11 by garance           #+#    #+#             */
/*   Updated: 2023/11/24 16:28:20 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	*ft_free_split_msh(t_split *strs)
{
	int	i;

	i = 0;
	while (strs[i].data)
	{
		ft_magic_malloc(FREE, 0, strs[i].data);
		if (strs[i].type)
			ft_magic_malloc(FREE, 0, strs[i].type);
		i++;
	}
	ft_magic_malloc(FREE, 0, strs);
	return (NULL);
}
