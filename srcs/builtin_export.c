/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 14:13:14 by athiebau          #+#    #+#             */
/*   Updated: 2024/01/05 15:41:27 by athiebau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	doublon_handler_bis(t_msh *msh, char *str, t_env *tmp, t_intel i)
{
	char	*tmp2;
	int		content_size;

	content_size = ft_strlen(str + (i.name_size + 1));
	if (i.statut == 4)
	{
		tmp2 = tmp->content;
		tmp->content = mcgic(mlcp(NULL, sizeof(char) * (content_size + 1)),
				MLC, ENV, msh);
		mcgic(mlcp(tmp2, 0), FREE, ENV, msh);
		ft_strlcpy(tmp->content, str + (i.name_size + 1), content_size + 1);
	}
	if (i.statut == 5)
	{
		tmp2 = tmp->content;
		tmp->content = mcgic(mlcp(ft_exstrjoin(msh, tmp->content, str
						+ i.name_size + 1), 1), ADD, ENV, msh);
		mcgic(mlcp(tmp2, 0), FREE, ENV, msh);
	}
}

void	doublon_handler(t_msh *m, char *str, t_env **env, t_intel i)
{
	t_env	*t;
	char	*tmp2;
	int		c;

	t = *env;
	c = ft_strlen(str + (i.name_size + 1));
	while (ft_strncmp(str, t->name, i.name_size))
		t = t->next;
	if (i.statut == 2)
		return ;
	if (i.statut == 3)
	{
		tmp2 = t->content;
		t->content = mcgic(mlcp(NULL, sizeof(char) * (c + 2 + 1)), MLC, ENV, m);
		mcgic(mlcp(tmp2, 0), FREE, ENV, m);
		ft_exstrlcpy(t->content, str + i.name_size, c + 2 + 1);
	}
	doublon_handler_bis(m, str, t, i);
	if (i.statut == 6)
	{
		tmp2 = t->content;
		t->content = mcgic(mlcp(ft_strjoin2(t->content, str + i.name_size
						+ 1), 1), ADD, ENV, m);
		mcgic(mlcp(tmp2, 0), FREE, ENV, m);
	}
}

int	new_env_node_env(t_msh *msh, char *str, int statut, t_env **env)
{
	t_env	*new;
	t_intel	i;
	int		content_size;

	i.name_size = get_name_size(str);
	content_size = ft_strlen2(str + i.name_size);
	if (!node_exist(env, str, i.name_size))
	{
		new = ft_lst_new_malloc(msh, i.name_size + 1, content_size + 1);
		if (!new)
			return (255);
		ft_strlcpy(new->name, str, i.name_size + 1);
		strlcpy_enjoyer(str + (i.name_size + 1), new, statut + 2, content_size);
		change_env_export(new, env, 2);
	}
	else
	{
		i.statut = statut + 2;
		doublon_handler(msh, str, env, i);
	}
	return (0);
}

int	new_env_node_export(t_msh *msh, char *str, int statut, t_env **env)
{
	t_env	*new;
	t_intel	i;
	int		content_size;

	i.name_size = get_name_size(str);
	content_size = ft_strlen2(str + i.name_size);
	if (!node_exist(env, str, i.name_size))
	{
		new = ft_lst_new_malloc(msh, i.name_size + 1, content_size + 2 + 1);
		if (!new)
			return (255);
		ft_strlcpy(new->name, str, i.name_size + 1);
		strlcpy_enjoyer(str + (i.name_size + 1), new, statut + 1, content_size);
		change_env_export(new, env, 1);
	}
	else
	{
		i.statut = statut + 1;
		doublon_handler(msh, str, env, i);
	}
	return (0);
}

static int	builtin_export_exec(int i, t_msh *msh)
{
	if (get_statut(msh->p.cmd_t[i]) == 1)
	{
		if (new_env_node_export(msh, msh->p.cmd_t[i], 1, msh->export_env) != 0)
			return (1);
	}
	else if (get_statut(msh->p.cmd_t[i]) == 2)
	{
		if (new_env_node_export(msh, msh->p.cmd_t[i], 2, msh->export_env) != 0)
			return (1);
		if (new_env_node_env(msh, msh->p.cmd_t[i], 2, msh->env) != 0)
			return (1);
	}
	else if (get_statut(msh->p.cmd_t[i]) == 4)
	{
		if (new_env_node_export(msh, msh->p.cmd_t[i], 4, msh->export_env) != 0)
			return (1);
		if (new_env_node_env(msh, msh->p.cmd_t[i], 4, msh->env) != 0)
			return (1);
	}
	return (0);
}

void	builtin_export(t_msh *msh)
{
	int	i;

	i = 0;
	if (!msh->p.cmd_t[1])
		ft_print_export(msh);
	else
	{
		while (msh->p.cmd_t[++i])
		{
			if (!valide_key(msh->p.cmd_t[i]))
			{
				msh->status = 1;
				error_export(msh, msh->p.cmd_t[i]);
			}
			else
			{
				if (builtin_export_exec(i, msh) == 1)
					return ;
			}
		}
	}
}
