/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 14:47:27 by athiebau          #+#    #+#             */
/*   Updated: 2024/01/15 11:32:31 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	get_size(char *s1, char *s2, int *len, t_msh *msh)
{
	if (!s1)
	{
		s1 = mcgic(mlcp(NULL, sizeof(char)), MLC, PIP, msh);
		s1[0] = '\0';
		*len = ft_strlen(s2);
	}
	else if (!s2)
		*len = ft_strlen(s1);
	else
		*len = ft_strlen(s1) + ft_strlen(s2);
}

char	*ft_strjoin3(t_msh *msh, char *s1, char *s2)
{
	int		len;
	char	*s;

	if (!s1 && !s2)
		return (NULL);
	get_size(s1, s2, &len, msh);
	s = mcgic(mlcp(NULL, sizeof(char) * (len + 2)), MLC, PIP, msh);
	if (!s)
		return (NULL);
	s[0] = '\0';
	if (s1)
		ft_strcat(s, s1);
	if (s2)
		ft_strcat(s, s2);
	s[len++] = '\t';
	s[len] = '\0';
	mcgic(mlcp(s1, 0), FREE, PIP, msh);
	return (s);
}

int	is_flag_a(char *cmd)
{
	int	i;

	i = -1;
	if (!cmd)
		return (0);
	while (cmd[++i])
	{
		if (cmd[i] == 'a')
			return (1);
	}
	return (0);
}

char	**make_in_order(char **str)
{
	char	*tmp;
	int		i;

	i = 0;
	while (str[i])
	{
		if (str[i + 1] && ft_strcmp_cas(str[i], str[i + 1]) > 0)
		{
			tmp = str[i];
			str[i] = str[i + 1];
			str[i + 1] = tmp;
			i = 0;
		}
		else
			i++;
	}
	return (str);
}
