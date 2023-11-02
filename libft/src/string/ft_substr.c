/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 10:25:43 by garance           #+#    #+#             */
/*   Updated: 2023/06/23 12:58:05 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	len_s;
	char	*sub;

	if (!s)
		return (NULL);
	len_s = ft_strlen(s);
	if (start > len_s)
		len = 0;
	else if (len_s - start < len)
		len = len_s - start;
	sub = (char *)malloc(sizeof(char) * len + 1);
	if (sub == NULL)
		return (NULL);
	if (start <= len_s)
		ft_strlcpy(sub, s + start, len + 1);
	else
		sub[0] = '\0';
	return (sub);
}
