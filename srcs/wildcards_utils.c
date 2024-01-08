/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 14:47:27 by athiebau          #+#    #+#             */
/*   Updated: 2024/01/08 15:34:56 by athiebau         ###   ########.fr       */
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