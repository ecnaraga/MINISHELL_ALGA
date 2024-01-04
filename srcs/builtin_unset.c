/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 17:05:52 by athiebau          #+#    #+#             */
/*   Updated: 2024/01/04 16:54:26 by athiebau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	del_node(t_env *node, t_env **env, t_msh *msh)
{
	t_env	*tmp;
	
	tmp = *env;
	if (tmp == node)
	{
		mcgic(mlcp(tmp->name, 0), FREE, ENV, msh);
		// ft_magic_malloc(FREE, 0, tmp->name, ENV);
		mcgic(mlcp(tmp->content, 0), FREE, ENV, msh);
		// ft_magic_malloc(FREE, 0, tmp->content, ENV);
		*env = tmp->next;
		mcgic(mlcp(tmp, 0), FREE, ENV, msh);
		// ft_magic_malloc(FREE, 0, tmp, ENV);
		return ;
	}
	while(tmp)
	{
		if(tmp->next == node)
		{
			mcgic(mlcp(tmp->next->name, 0), FREE, ENV, msh);
			// ft_magic_malloc(FREE, 0, tmp->next->name, ENV);
			mcgic(mlcp(tmp->next->content, 0), FREE, ENV, msh);
			// ft_magic_malloc(FREE, 0, tmp->next->content, ENV);
			tmp->next = tmp->next->next;
			mcgic(mlcp(node, 0), FREE, ENV, msh);
			// ft_magic_malloc(FREE, 0, node, ENV);
			break ;
		}
		tmp = tmp->next;
	}	
}

void	del_env(char *cmd, t_msh *msh)
{
	t_env	*tmp;
	// int		i;

	// i = 0;
	tmp = *(msh->env);
	while(tmp)
	{
		if(!strcmp(cmd, tmp->name))
		{
			del_node(tmp, msh->env, msh);
			break;
		}
		tmp = tmp->next;
	}
	tmp = *(msh->export_env);
	while(tmp)
	{
		if(!strcmp(cmd, tmp->name))
		{
			del_node(tmp, msh->export_env, msh);
			break;
		}
		tmp = tmp->next;
	}
}

void	builtin_unset(t_msh *msh)
{
	int i;

	i = 0;
	printf("BUILTIN\n");
	if (msh->p.cmd_t[++i])
	{
		while (msh->p.cmd_t[i])
		{
			del_env(msh->p.cmd_t[i], msh);
			i++;
		}
	}
}