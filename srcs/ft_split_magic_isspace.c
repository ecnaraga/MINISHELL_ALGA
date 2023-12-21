/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_magic_isspace.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 11:01:59 by garance           #+#    #+#             */
/*   Updated: 2023/12/21 11:41:32 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_countword(const char *s)
{
	int	c_wd;
	int	i;

	c_wd = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] && (s[i] == ' ' || (s[i] >= '\t' && s[i] <= '\r')))
			i++;
		if (s[i] && (s[i] != ' ' && (s[i] < '\t' || s[i] > '\r')))
		{
			c_wd++;
			i++;
		}
		while (s[i] && (s[i] != ' ' && (s[i] < '\t' || s[i] > '\r')))
			i++;
	}
	return (c_wd);
}

static void	*ft_free_strs(t_msh *msh, char **strs, int j)
{
	int	i;

	i = 0;
	while (i < j)
	{
		mlcgic(mlcp(strs[i], 0), FREE, PIP, msh);
		// ft_magic_malloc(FREE, 0, strs[i], PIP);
		i++;
	}
	mlcgic(mlcp(strs, 0), FREE, PIP, msh);
	// ft_magic_malloc(FREE, 0, strs, PIP);
	return (NULL);
}

static char	**ft_split_strs(t_msh *msh, const char *s, char **strs, int c_wd)
{
	int	j;
	int	c_lt;

	j = -1;
	while (*s && ++j < c_wd)
	{
		c_lt = 0;
		while (*s && (*s == ' ' || (*s >= '\t' && *s <= '\r')))
			s++;
		while (*s && (*s != ' ' && (*s < '\t' || *s > '\r')))
		{
			c_lt++;
			s++;
		}
		if (c_lt > 0)
		{
			strs[j] = mlcgic(mlcp(NULL, sizeof(char *) * (c_lt + 1)), MALLOC, PIP, msh);
			// strs[j] = ft_magic_malloc(MALLOC, (sizeof(char) * (c_lt + 1)), 0, PIP);
			if (strs[j] == NULL)
				return (ft_free_strs(msh, strs, j));
			ft_strlcpy(strs[j], s - c_lt, c_lt + 1);
		}
	}
	return (strs);
}

char	**ft_split_isspace_magic_malloc(t_msh *msh, char const *s)
{
	int		c_wd;
	char	**strs;

	if (!s)
		return (NULL);
	c_wd = ft_countword(s);
	strs = mlcgic(mlcp(NULL, sizeof(char *) * (c_wd + 1)), MALLOC, PIP, msh);
	// strs = ft_magic_malloc(MALLOC, (sizeof(char *) * (c_wd + 1)), 0, PIP);
	if (strs == NULL)
		return (NULL);
	if (ft_split_strs(msh, s, strs, c_wd) == NULL)
		return (NULL);
	strs[c_wd] = NULL;
	return (strs);
}
