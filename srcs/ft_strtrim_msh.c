/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim_msh.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 10:25:25 by garance           #+#    #+#             */
/*   Updated: 2023/12/04 16:32:35 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static unsigned int	ft_count_deb(char *s1)
{
	unsigned int	i;
	unsigned int	count;

	i = 0;
	count = 0;
	while (s1 && s1[i] != '(')
	{
		count++;
		i++;
	}
	return (count);
}

static unsigned int	ft_count_end(char *s1)
{
	unsigned int	i;
	unsigned int	count;
	unsigned int	par;

	i = 0;
	count = 0;
	par = 0;
	while (s1[i++] != '(')
		;
	while (s1[i++])
	{
		if (s1[i] == '(')
			par++;
		else if (s1[i] == ')')
		{
			if (par == 0)
			{
				count += 2;
				break ;
			}
			par--;
		}
		count++;
	}
	return (count);
}

/*
Take the line command, modify it and return a sub-line command
The line command becomes all that exist after the sub_line command (so after
	the first parenthesis encountered)
The sub-line command contains the content that is between the first parenthesis
	encountered
*/
char	*ft_strtrim_msh(char **s1)
{
	unsigned int	count;
	char			*s2;
	unsigned int	len2;
	char			*tmp;

	if (!s1)
		return (NULL);
	else
	{
		count = ft_count_deb(*s1) + 1;
		if (count == ft_strlen(*s1))
			len2 = 1;
		else
			len2 = ft_count_end(*s1) + 1;
		s2 = ft_magic_malloc(MALLOC, sizeof(char) * len2, NULL, NO_ENV);
		if (s2 == NULL)
			return (NULL);
		ft_strlcpy(s2, *s1 + count, len2);
		tmp = *s1;
		*s1 = ft_magic_malloc(ADD, 0, ft_strdup(*s1 + count + len2 - 1), NO_ENV);
		ft_magic_malloc(FREE, 0, tmp, NO_ENV);
	}
	return (s2);
}
