/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_bis.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 17:47:35 by athiebau          #+#    #+#             */
/*   Updated: 2023/12/12 11:56:08 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_print_export(t_msh *minish)
{
	t_env	*tmp;

	tmp = *(minish->export_env);
	while (tmp)
	{
		printf("export %s%s\n", (char *)tmp->name, (char *)tmp->content);
		tmp = tmp->next;
	}
}

int	get_statut(char *cmd)
{
	int	i;

	i = -1;
	while(cmd[++i])
	{
		if (cmd[i] == '=')
		{
			if(cmd[i - 1] == '+')
				return (4);
			return (2);
		}
	}
	return (1);	
}

int	valide_key(char *key)
{
	int	i;

	i = 1;
	if (ft_isalpha(key[0]) || key[0] == '_')
	{
		while(key[i] != '=' && key[i])
		{
			if (ft_isalnum(key[i]) || key[i] == '_')
				return (1);
			else
				return (0);	
		}
		return (1);
	}
	return (0);
}