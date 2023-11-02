/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 12:17:10 by galambey          #+#    #+#             */
/*   Updated: 2023/06/23 12:56:55 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	len;

	i = 0;
	if (dst == NULL && size == 0)
		return (ft_strlen(src) + size);
	len = ft_strlen(dst);
	if (len > size - 1 || size == 0)
		return (ft_strlen(src) + size);
	while (src[i] && len + i < size - 1)
	{
		dst[len + i] = src[i];
		i++;
	}
	dst[len + i] = '\0';
	return (ft_strlen(src) + len);
}
