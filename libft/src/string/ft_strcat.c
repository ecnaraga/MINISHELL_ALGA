/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 13:39:22 by garance           #+#    #+#             */
/*   Updated: 2023/06/23 16:42:21 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

char	*ft_strcat(char *dst, const char *src)
{
	size_t	i;
	size_t	len;

	i = -1;
	len = ft_strlen(dst);
	while (src[++i])
		dst[len + i] = src[i];
	dst[len + i] = '\0';
	return (dst);
}
