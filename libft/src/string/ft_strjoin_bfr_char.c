/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_bfr_char.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 12:16:55 by galambey          #+#    #+#             */
/*   Updated: 2023/09/08 12:14:06 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

char	*ft_strjoin_bfr_char(char const *s1, char const *s2, char c)
{
	int		len;
	char	*s;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		len = ft_strlen_bfr_char(s2, c);
	else if (!s2)
		len = ft_strlen(s1);
	else
		len = ft_strlen(s1) + ft_strlen_bfr_char(s2, c);
	s = (char *)malloc(sizeof(char) * (len + 1));
	if (!s)
		return (NULL);
	s[0] = '\0';
	if (s1)
		ft_strcat(s, s1);
	if (s2)
		ft_strlcat(s, s2, len + 1);
	return (s);
}
