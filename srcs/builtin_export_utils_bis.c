/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils_bis.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 14:47:45 by athiebau          #+#    #+#             */
/*   Updated: 2024/01/11 14:53:19 by athiebau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

size_t	ft_strlen2(const char *s)
{
	size_t	i;

	i = 0;
	if (*s == '=')
		s++;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strjoin2(char const *s1, char const *s2, t_msh *msh)
{
	int		len;
	char	*s;

	if (!s1 && !s2)
		return (NULL);
	if (*s2 == '=')
		s2++;
	if (!s1)
		len = ft_strlen2(s2);
	else if (!s2)
		len = ft_strlen(s1);
	else
		len = ft_strlen(s1) + ft_strlen2(s2);
	s = mcgic(mlcp(NULL, sizeof(char) * (len + 1)), MLC, ENV, msh);
	if (msh->status == 255)
		return (NULL);
	s[0] = '\0';
	if (s1)
		ft_strcat(s, s1);
	if (s2)
		ft_strcat(s, s2);
	return (s);
}

char	*ft_exstrjoin(t_msh *msh, char const *s1, char const *s2)
{
	int		len;
	char	*s;

	if (!s1 && !s2)
		return (NULL);
	if (*s2 == '=')
		s2++;
	if (!s1)
		len = ft_strlen2(s2);
	else if (!s2)
		len = ft_strlen(s1);
	else
		len = ft_strlen(s1) + ft_strlen(s2);
	s = mcgic(mlcp(NULL, sizeof(char) * (len + 1)), MLC, ENV, msh);
	if (msh->status == 255)
		return (NULL);
	s[0] = '\0';
	if (s1)
		ft_strlcat(s, s1, ft_strlen(s1));
	if (s2)
		ft_strcat(s, s2);
	s[len - 1] = '"';
	s[len] = '\0';
	return (s);
}

void	strlcpy_enjoyer(char *str, t_env *node, int statut, int size)
{
	if (statut == 2)
		ft_bzero(node->content, 2 + 1);
	else if (statut == 3 || statut == 5)
		ft_exstrlcpy(node->content, str, size + 2 + 1);
	else if (statut == 4 || statut == 6)
		ft_strlcpy(node->content, str, size + 1);
}

void	change_env_export(t_env *node, t_env **env, int info)
{
	ft_lstadd_back_env(env, node);
	if (info == 1)
		order_export_env(env);
}
