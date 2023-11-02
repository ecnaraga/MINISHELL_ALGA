/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 13:47:39 by galambey          #+#    #+#             */
/*   Updated: 2023/06/23 16:36:54 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/libft.h"

size_t	ft_strlen_gnl(const char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}

void	ft_strcpy_gnl(char *dst, const char *src)
{
	size_t	i;

	i = 0;
	if (src)
	{	
		while (src[i])
		{
			dst[i] = src[i];
			i++;
		}
	dst[i] = '\0';
	}
}

char	*ft_strjoin_gnl(char const *s1, char const *s2, int i)
{
	int			len;
	char		*s;

	if (!s1 && i == -1)
		len = ft_strlen_gnl(s2);
	else if (!s1)
		len = i + 1;
	else if (!s2)
		len = ft_strlen_gnl(s1);
	else if (i == -1)
		len = ft_strlen_gnl(s1) + ft_strlen_gnl(s2);
	else
		len = ft_strlen_gnl(s1) + i + 1;
	s = (char *)malloc(sizeof(char) *(len + 1));
	if (!s)
		return (NULL);
	s[0] = '\0';
	if (s1)
		ft_strlcat(s, s1, ft_strlen_gnl(s1) + 1);
	if (s2)
		ft_strlcat(s, s2, len + 1);
	return (s);
}
