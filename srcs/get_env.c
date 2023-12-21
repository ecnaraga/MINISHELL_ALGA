/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 15:32:01 by athiebau          #+#    #+#             */
/*   Updated: 2023/12/21 15:41:39 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	get_sizes(int *size_name, int *size_content, int *i, char **str)
{
	*size_name = get_name_size(str[*i]);
	*size_content = ft_strlen(str[*i] + (*size_name + 1));
}

static int	fill_env(t_msh *msh, t_env **env, char **str)
{
	int		i;
	int		size_name;
	int		size_content;
	t_env	*new;

	i = -1;

	if (check_env(env, str) == 1)
		return (0);
	while (str[++i])
	{
		get_sizes(&size_name, &size_content, &i, str);
		new = ft_lst_new_malloc(msh, size_name + 1, size_content + 1);
		if (!new)
		{
			mlcgic(NULL, FLUSH, ENV, msh);
			// ft_magic_malloc(FLUSH, 0, NULL, ENV);
			return (1);
		}
		ft_strlcpy(new->name, str[i], size_name + 1);
		ft_strlcpy(new->content, str[i] + (size_name + 1), size_content + 1);
		ft_lstadd_back_env(env, new);
	}
	return (0);
}

t_env	**get_env(char **str, t_msh *msh)
{
	t_env	**env;

	env = mlcgic(mlcp(NULL, sizeof(t_env)), MALLOC, ENV, msh);
	// env = ft_magic_malloc(MALLOC, sizeof(t_env), NULL, ENV);
	if (!env)
		return (ft_exit(-1, -1, -1, msh), NULL);
	*env = NULL;
	if (fill_env(msh, env, str) == 1)
		return (NULL);
	return (env);
}
