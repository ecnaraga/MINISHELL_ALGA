/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 15:32:01 by athiebau          #+#    #+#             */
/*   Updated: 2023/11/28 17:29:08 by athiebau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	get_maj(char *str)
{
	int	i;
	
	i = -1;
	while(str[++i])
	{
		if (str[i] == '=')
			return (i);
	}
	return (-1);
}

static int	fill_env(t_env **env, char **str)
{
	int		i;
	int		maj;
	int		size;
	t_env	*new;

	i = 0;
	if (!str || str[0] == NULL)
	{
		ft_magic_malloc(FREE, 0, env, ENV);
		return (1);
	}
	while (str[i])
	{
		maj = get_maj(str[i]);
		size = ft_strlen(str[i] + maj);
		new = ft_lst_new_malloc(maj + 1, size + 1);
		if (!new)
		{
			ft_magic_malloc(FLUSH, 0, NULL, ENV);
			return (1);
		}
		ft_strlcpy(new->name, str[i], maj + 1);
		ft_strlcpy(new->content, str[i] + maj, size + 1);
		ft_lstadd_back_env(env, new);
		i++;
	}
	return (0);
}

t_env	**get_env(char **str)
{
	t_env	**env;

	env = ft_magic_malloc(MALLOC, sizeof(t_env), NULL, ENV);
	if (!env)
		return (NULL);
	*env = NULL;
	if (fill_env(env, str) == 1)
		return (NULL);
	return (env);
}
