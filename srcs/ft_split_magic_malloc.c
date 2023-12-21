/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_magic_malloc.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 12:14:56 by galambey          #+#    #+#             */
/*   Updated: 2023/12/21 15:19:36 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_countwords(const char *s)
{
	int	c_wd;
	int	i;

	c_wd = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] && s[i] == ':')
			i++;
		if (s[i] && s[i] != ':')
		{
			c_wd++;
			i++;
		}
		while (s[i] && s[i] != ':')
			i++;
	}
	return (c_wd);
}

// static void	*ft_free_strs(char **strs, int j)
// {
// 	int	i;

// 	i = 0;
// 	while (i < j)
// 	{
// 		ft_magic_malloc(FREE, 0, strs[i], PIP);
// 		i++;
// 	}
// 	ft_magic_malloc(FREE, 0, strs, PIP);
// 	return (NULL);
// }

static char	**ft_split_strs(t_msh *msh, const char *s, char **strs, int c_wd)
{
	int	j;
	int	c_lt;

	j = -1;
	while (*s && ++j < c_wd)
	{
		c_lt = 0;
		while (*s && *s == ':')
			s++;
		while (*s && *s != ':')
		{
			c_lt++;
			s++;
		}
		if (c_lt > 0)
		{
			strs[j] = mlcgic(mlcp(NULL, sizeof(char *) * (c_lt + 1)), MALLOC, PIP, msh);
			// strs[j] = ft_magic_malloc(MALLOC, sizeof(char) * (c_lt + 1), NULL, PIP);
			if (strs[j] == NULL)
				return (NULL); // TOUT SERA FREE DANS LE GARBAGGE COLLECTOR
				// return (ft_free_strs(strs, j));
			ft_strlcpy (strs[j], s - c_lt, c_lt + 1);
		}
	}
	return (strs);
}

char	**ft_split_magic_malloc(t_msh *msh, int sub, char const *s)
{
	int		c_wd;
	char	**strs;

	if (!s)
		return (NULL);
	if (s[0] == '\0')
	// if (!c && s[0] == '\0')
			c_wd = 0;
	else
		c_wd = ft_countwords(s);
	strs = mlcgic(mlcp(NULL, sizeof(char *) * (c_wd + 1)), MALLOC, PIP, msh);
	// strs = ft_magic_malloc(MALLOC, sizeof(char *) * (c_wd + 1), NULL, PIP);
	if (strs == NULL)
		ft_exit_bis(msh, sub, -1, -1); // SI MALLOC KO ON QUITTE LE PROCESS ACTUEL
	if (ft_split_strs(msh, s, strs, c_wd) == NULL)
		ft_exit_bis(msh, sub, -1, -1); // SI MALLOC KO  DAN FT_SPLIT_STRS ON QUITTE LE PROCESS ACTUEL
	strs[c_wd] = NULL;
	return (strs);
}
