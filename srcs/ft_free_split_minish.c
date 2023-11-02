/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_split_minish.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 09:39:11 by garance           #+#    #+#             */
/*   Updated: 2023/10/31 13:16:16 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	*ft_free_split_minish(t_split *strs)
{
	int	i;

	i = 0;
	while (strs[i].data)
	{
		free(strs[i].data);
		i++;
	}
	free(strs);
	return (NULL);
}
