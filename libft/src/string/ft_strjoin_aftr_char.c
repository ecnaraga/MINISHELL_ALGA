/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_aftr_char.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 12:16:55 by galambey          #+#    #+#             */
/*   Updated: 2024/01/10 18:42:32 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

char	*ft_strjoin_aftr_char(char const *s1, char const *s2, char c)
{
	size_t	len;
	char	*s;
	size_t	i;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		len = ft_strlen_afr_char(s2, c);
	else if (!s2)
		len = ft_strlen(s1);
	else
		len = ft_strlen(s1) + ft_strlen_afr_char(s2, c);
	s = (char *)malloc(sizeof(char) * (len + 1));
	if (!s)
		return (NULL);
	s[0] = '\0';
	if (s1)
		ft_strcat(s, s1);
	if (s2)
	{
		i = ft_strlen(s1) - ft_strlen_afr_char(s2, c);
		ft_strlcat(s + i, s2, len + 1);
	}
	return (s);
}
