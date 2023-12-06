/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 17:05:52 by athiebau          #+#    #+#             */
/*   Updated: 2023/12/06 18:06:29 by athiebau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	del_node(t_env *node, t_env **env)
{
	t_env	*tmp;
	
	tmp = *env;
	if (tmp == node)
	{
		ft_magic_malloc(FREE, 0, tmp->name, ENV);
		ft_magic_malloc(FREE, 0, tmp->content, ENV);
		*env = tmp->next;
		ft_magic_malloc(FREE, 0, tmp, ENV);
		return ;
	}
	while(tmp)
	{
		if(tmp->next == node)
		{
			ft_magic_malloc(FREE, 0, tmp->next->name, ENV);
			ft_magic_malloc(FREE, 0, tmp->next->content, ENV);
			tmp->next = tmp->next->next;
			ft_magic_malloc(FREE, 0, node, ENV);
			break ;
		}
		tmp = tmp->next;
	}	
}

void	del_env(char *cmd, t_msh *minish)
{
	t_env	*tmp;
	int		i;

	i = 0;
	tmp = *(minish->env);
	while(tmp)
	{
		if(!strcmp(cmd, tmp->name))
		{
			printf("suppr env\n");
			del_node(tmp, minish->env);
			break;
		}
		tmp = tmp->next;
	}
	tmp = *(minish->export_env);
	while(tmp)
	{
		if(!strcmp(cmd, tmp->name))
		{
			printf("suppr export\n");
			del_node(tmp, minish->export_env);
			break;
		}
		tmp = tmp->next;
	}
}

void	builtin_unset(char **str, t_msh *minish)
{
	int i;

	i = 0;
	// if (minish->p.cmd_opt[++i])
	// {
	// 	while (minish->p.cmd_opt[i])
	// 	{
	// 		del_env(minish->p.cmd_opt[i], minish);
	// 		i++;
	// 	}
	// }
	printf("entre dans unset\n");
	if (str[++i])
	{
		while (str[i])
		{
			del_env(str[i], minish);
			i++;
		}
	}
}