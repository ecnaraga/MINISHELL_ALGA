/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_structtablen.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 11:55:08 by galambey          #+#    #+#             */
/*   Updated: 2023/11/01 12:27:30 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_structtablen(t_split *tab)
{
	int	i;

	i = 0;
	if (!tab)
		return (-1);
	while (tab[i].data)
		i++;
	return (i);
}
