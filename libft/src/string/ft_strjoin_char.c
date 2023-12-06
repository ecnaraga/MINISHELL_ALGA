/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_char.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 12:16:55 by galambey          #+#    #+#             */
/*   Updated: 2023/12/06 15:12:42 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

static int	ft_strcpy_mode(char *dst, const char *src)
{
	size_t	i;

	i = 0;
	while (src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (i);
}

char	*ft_strjoin_char(char const *s1, char c)
{
	int			len;
	int			i;
	char		*s;

	i = 0;
	if (!s1)
		len = 1;
	else
		len = ft_strlen(s1) + 1;
	s = (char *)malloc(sizeof(char) *(len + 1));
	if (!s)
		return (NULL);
	s[0] = '\0';
	if (s1)
		i = ft_strcpy_mode(s, s1);
	s[i] = c;
	s[i + 1] = '\0';
	return (s);
}
