/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_magic_malloc.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 12:14:56 by galambey          #+#    #+#             */
/*   Updated: 2024/01/10 13:49:59 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_countwords(const char *s, char c)
{
	int	c_wd;
	int	i;

	c_wd = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		if (s[i] && s[i] != c)
		{
			c_wd++;
			i++;
		}
		while (s[i] && s[i] != c)
			i++;
	}
	return (c_wd);
}

static char	**ft_split_strs(t_msh *msh, const char *s, t_spl *st, char c)
{
	int	j;
	int	c_lt;

	j = -1;
	while (*s && ++j < st->c_wd)
	{
		c_lt = 0;
		while (*s && *s == c)
			s++;
		while (*s && *s != c)
		{
			c_lt++;
			s++;
		}
		if (c_lt > 0)
		{
			st->strs[j] = mcgic(
					mlcp(NULL, sizeof(char *) * (c_lt + 1)), MLC, PIP, msh);
			if (st->strs[j] == NULL)
				return (NULL);
			ft_strlcpy (st->strs[j], s - c_lt, c_lt + 1);
		}
	}
	return (st->strs);
}

char	**ft_split_magic_malloc(t_msh *msh, int sub, char const *s, char c)
{
	t_spl	st;

	if (!s)
		return (NULL);
	if (s[0] == '\0')
		st.c_wd = 0;
	else
		st.c_wd = ft_countwords(s, c);
	st.strs = mcgic(mlcp(NULL, sizeof(char *) * (st.c_wd + 1)), MLC, PIP, msh);
	if (st.strs == NULL)
		ft_exit_bis(msh, sub, -1, -1);
	if (ft_split_strs(msh, s, &st, c) == NULL)
		ft_exit_bis(msh, sub, -1, -1);
	st.strs[st.c_wd] = NULL;
	return (st.strs);
}
