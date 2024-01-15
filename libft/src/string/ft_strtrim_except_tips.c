/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim_except_tips.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 10:25:25 by garance           #+#    #+#             */
/*   Updated: 2024/01/15 11:35:02 by galambey         ###   ########.fr       */
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

static	unsigned int	ft_count_deb(char const *s1, char const *set, int rule)
{
	unsigned int	i;
	unsigned int	count;

	i = 0;
	count = 0;
	while (s1 && ft_present_char(s1[i], set) == 0)
	{
		count ++;
		i++;
	}
	if (count > 0 && rule != 2)
		return (count - 1);
	return (count);
}

static	unsigned int	ft_count_end(char const *s1, char const *set,
		unsigned int len, int rule)
{
	unsigned int	i;
	unsigned int	count;

	i = 0;
	count = 0;
	while ((len - i - 1 != 0) && ft_present_char(s1[len - i - 1], set) == 0)
	{
		count ++;
		i++;
	}
	if (count > 0 && rule != 1)
		return (count - 1);
	return (count);
}

/*
if rule == 0 : trim set but leave the tips at the begining and end
if rule == 1 : trim set but leave the tips at the begining only
if rule == 2 : trim set but leave the tips at the end only
*/
char	*ft_strtrim_except_tips(char const *s1, char const *set, int rule)
{
	unsigned int	count_deb;
	unsigned int	l_s1;
	char			*s2;
	unsigned int	l_s2;

	if (!s1)
		return (NULL);
	if (!set)
		s2 = ft_strdup(s1);
	else
	{
		l_s1 = ft_strlen(s1);
		count_deb = ft_count_deb(s1, set, rule);
		if (count_deb == l_s1)
			l_s2 = 1;
		else
			l_s2 = l_s1 + 1 - count_deb - ft_count_end(s1, set, l_s1, rule);
		s2 = (char *)malloc(sizeof(char) * l_s2);
		if (s2 == NULL)
			return (NULL);
		ft_strlcpy(s2, s1 + count_deb, l_s2);
	}
	return (s2);
}
