/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim_msh.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 10:25:25 by garance           #+#    #+#             */
/*   Updated: 2023/12/04 18:07:54 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_init_var(int *i, size_t *count, int *par)
{
	*i = 0;
	*count = 0;
	*par = 0;
}

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
	int	i;
	size_t	count;
	int	par;

	ft_init_var(&i, &count, &par);
	while (s1[i] != '(')
		i++;
	while (s1[i])
	{
		if (s1[i] == '(')
			par++;
		else if (s1[i] == ')')
		{
			if (par == 1)
			{
				count += 1;
				break ;
			}
			par--;
		}
		count++;
		i++;
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
	size_t	count;
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
