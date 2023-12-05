/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 14:13:14 by athiebau          #+#    #+#             */
/*   Updated: 2023/12/05 14:11:13 by athiebau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_env	strlcpy_enjoyer(char *str, t_env *node, int statut, int size)
{
	if (statut == 2)
		ft_bzero(node->content, 2 + 1);
	else if (statut == 3 || statut == 5)
		ft_exstrlcpy(node->content, str, size + 2 + 1);
	else if (statut == 4 || statut == 6)
		ft_strlcpy(node->content, str, size + 1);
}

static int	node_exist(t_env **env, char *str, int size)
{
	t_env	*tmp;

	tmp = *env;
	while(tmp)
	{
		if (ft_strncmp(str, tmp->name, size) == 0)
			return (1);
	}
	return (0);
}

static t_env	*new_env_node(char *str, int statut, t_env **env, int info)
{
	t_env	*new;
	int	name_size;
	int	content_size;

	if(!valide_key(str))
		return (NULL); //GERER SI L'INDENTIFIER N'EST PAS Ok : bash: export: `=': not a valid identifier
	name_size = get_name_size(str);
	content_size = ft_strlen(str + name_size);
	if(!node_exist(env, str, name_size))
	{
		if (info == 2)
			new = ft_lst_new_malloc(name_size + 1, content_size + 1);
		else 
			new = ft_lst_new_malloc(name_size + 1, content_size + 2 + 1);
		if (!new)
		{
			ft_magic_malloc(FLUSH, 0, NULL, ENV);
			return (NULL);
		}
		ft_strlcpy(new->name, str, name_size + 1);
		strlcpy_enjoyer(str + name_size, new, statut + info, content_size);
	}
	else
		doublon_handler();
	return (new);
}

int	change_env(t_env *node, t_env **env, int info)
{
	t_env *tmp;

	tmp = *env;
	if (!node)
		return (1);
	ft_lstadd_back_env(env, node);
	if (info == 1)
		order_export_env(env);
	return (0);
}

void	builtin_export(char **str, t_msh *minish)
{
	char	*name;
	char	*content;
	int	i;
	t_env	*new;

	i = 1;
	if(!str[i])
		ft_print_export(minish);
	else
	{
		while(str[i])
		{
			if(get_statut(str[i]) == 1)
			{
				change_env(new_env_node(str[i], 1, minish->export_env, 1), minish->export_env, 1);
			}
			else if (get_statut(str[i]) == 2)
			{
				change_env(new_env_node(str[i], 2, minish->export_env, 1), minish->export_env, 1);
				change_env(new_env_node(str[i], 2, minish->env, 2), minish->env, 0);
			}
			else if (get_statut(str[i]) == 4)
			{
				change_export_env(str[i], minish);
				fill_env(minish->env, str[i]);
			}
		}
	}
}
