/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim_exept_set.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garance <garance@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 10:25:25 by garance           #+#    #+#             */
/*   Updated: 2023/12/03 12:44:14 by garance          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

static unsigned int	ft_present_char(char c, char const *set)
{
	unsigned int	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (0);
		i++;
	}
	return (1);
}

static	unsigned int	ft_count_deb(char const *s1, char const *set)
{
	unsigned int	i;
	unsigned int	count;

	i = 0;
	count = 0;
	while (s1 && ft_present_char(s1[i], set) != 0)
	{
		count ++;
		i++;
	}
	return (count);
}

static	unsigned int	ft_count_end(char const *s1, char const *set,
		unsigned int len)
{
	unsigned int	i;
	unsigned int	count;

	i = 0;
	count = 0;
	while ((len - i - 1 != 0) && ft_present_char(s1[len - i - 1], set) != 0)
	{
		count ++;
		i++;
	}
	return (count);
}

char	*ft_strtrim_exept_set(char const *s1, char const *set)
{
	unsigned int	count_deb;
	unsigned int	len_s1;
	char			*s2;
	unsigned int	len_s2;

	if (!s1)
		return (NULL);
	if (!set)
		s2 = ft_strdup(s1);
	else
	{
		len_s1 = ft_strlen(s1);
		count_deb = ft_count_deb(s1, set);
		if (count_deb == len_s1)
			len_s2 = 1;
		else
			len_s2 = len_s1 + 1 - count_deb - ft_count_end(s1, set, len_s1);
		s2 = (char *)malloc(sizeof(char) * len_s2);
		if (s2 == NULL)
			return (NULL);
		ft_strlcpy(s2, s1 + count_deb, len_s2);
	}
	return (s2);
}
