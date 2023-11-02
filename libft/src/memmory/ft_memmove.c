/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 14:23:25 by galambey          #+#    #+#             */
/*   Updated: 2023/06/23 13:02:14 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	if (n > 0 && src != dest)
	{
		if (src < dest)
		{
			while (n > 0)
			{
				((char *) dest)[n - 1] = ((char *) src)[n - 1];
				n--;
			}
		}
		else if (src > dest)
		{
			while (n > 0)
			{
				((char *) dest)[i] = ((char *) src)[i];
				i++;
				n--;
			}
		}
	}
	return (dest);
}
