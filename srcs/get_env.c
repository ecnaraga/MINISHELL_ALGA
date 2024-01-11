/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 15:32:01 by athiebau          #+#    #+#             */
/*   Updated: 2024/01/09 17:27:50 by athiebau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_maj_shlvl(t_msh *msh, char **env_tab, int i)
{
	int		shlvl;
	char	*tmp;

	if (ft_strncmp(env_tab[i], "SHLVL", 5) == 0)
	{
		shlvl = ft_atoi(env_tab[i] + 6) + 1;
		tmp = mcgic(mlcp(ft_itoa(shlvl), 1), ADD, ENV, msh);
		if (msh->status == 255)
			return (1);
		mcgic(mlcp(env_tab[i], 0), FREE, ENV, msh);
		env_tab[i] = mcgic(mlcp(ft_strjoin("SHLVL=", tmp), 1), ADD, ENV, msh);
		if (msh->status == 255)
			return (255);
		mcgic(mlcp(tmp, 0), FREE, ENV, msh);
		return (1);
	}
	return (0);
}

void	ft_inc_shlvl(char **env_tab, t_msh *msh)
{
	int	i;

	i = -1;
	while (env_tab[++i])
	{
		if (ft_maj_shlvl(msh, env_tab, i) > 0)
			return ;
	}
	env_tab[i] = mcgic(mlcp(ft_strjoin("SHLVL=", "0"), 1), ADD, ENV, msh);
	if (msh->status == 255)
		return ;
	env_tab[++i] = NULL;
}

static void	get_sizes(int *size_name, int *size_content, int *i, char **str)
{
	*size_name = get_name_size(str[*i]);
	*size_content = ft_strlen(str[*i] + (*size_name + 1));
}

static void	fill_env(t_msh *msh, t_env **env, char **str)
{
	int		i;
	int		size_name;
	int		size_content;
	t_env	*new;

	i = -1;
	if (check_env(str) == 1)
		return ;
	while (str[++i])
	{
		get_sizes(&size_name, &size_content, &i, str);
		new = ft_lst_new_malloc(msh, size_name + 1, size_content + 1);
		if (!new)
			ft_exit(-1, -1, -1, msh);
		ft_strlcpy(new->name, str[i], size_name + 1);
		ft_strlcpy(new->content, str[i] + (size_name + 1), size_content + 1);
		new->print = 1;
		ft_lstadd_back_env(env, new);
	}
}

t_env	**get_env(char **str, t_msh *msh)
{
	t_env	**env;

	ft_inc_shlvl(str, msh);
	if (msh->status == 255)
		return (ft_exit(-1, -1, -1, msh), NULL);
	env = mcgic(mlcp(NULL, sizeof(t_env)), MLC, ENV, msh);
	if (!env)
		return (ft_exit(-1, -1, -1, msh), NULL);
	*env = NULL;
	fill_env(msh, env, str);
	return (env);
}
