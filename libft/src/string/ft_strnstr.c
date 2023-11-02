/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 12:20:52 by galambey          #+#    #+#             */
/*   Updated: 2023/06/23 12:57:34 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;
	size_t	len_little;
	size_t	len_big;

	i = -1;
	j = 0;
	if (!big && little && len == 0)
		return (0);
	len_little = ft_strlen(little);
	len_big = ft_strlen(big);
	if (len >= len_little)
	{
		while (++i < len_big && i <= len - len_little)
		{
			while (little[j] && big[i + j] == little[j])
				j++;
			if (little[j] == '\0')
				return ((char *) big + i);
			j = 0;
		}
	}
	if (little[j] == '\0')
		return ((char *) big + i);
	return (0);
}
