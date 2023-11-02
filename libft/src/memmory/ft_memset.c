/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 09:30:02 by galambey          #+#    #+#             */
/*   Updated: 2023/06/23 13:01:29 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t	i;
	char	*cpy;

	i = 0;
	cpy = s;
	while (i < n)
	{
		cpy[i] = c;
		i++;
	}
	return (s);
}
