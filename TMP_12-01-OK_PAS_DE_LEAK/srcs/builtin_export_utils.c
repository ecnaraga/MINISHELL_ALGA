/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 17:47:35 by athiebau          #+#    #+#             */
/*   Updated: 2024/01/11 14:07:32 by athiebau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_print_export(t_msh *minish)
{
	t_env	*tmp;

	tmp = *(minish->export_env);
	while (tmp)
	{
		if (tmp->content[0])
			printf("export %s=%s\n", (char *)tmp->name, (char *)tmp->content);
		else
			printf("export %s\n", (char *)tmp->name);
		tmp = tmp->next;
	}
}

int	get_statut(char *cmd)
{
	int	i;

	i = -1;
	while (cmd[++i])
	{
		if (cmd[i] == '=')
		{
			if (cmd[i - 1] == '+')
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
		while (key[i] && key[i] != '=')
		{
			if (ft_isalnum(key[i]) || key[i] == '_'
				|| (key[i] == '+' && key[i + 1] == '='))
				i++;
			else
				return (0);
		}
		return (1);
	}
	return (0);
}

int	node_exist(t_env **env, char *str, int size, t_msh *msh)
{
	t_env	*tmp;
	char	*temp;

	tmp = *env;
	temp = mcgic(mlcp(ft_substr(str, 0, size), 1), ADD, PIP, msh);
	while (tmp)
	{
		if (ft_strcmp(temp, tmp->name) == 0)
		{
			mcgic(mlcp(temp, 0), FREE, PIP, msh);
			return (1);
		}
		tmp = tmp->next;
	}
	mcgic(mlcp(temp, 0), FREE, PIP, msh);
	return (0);
}

void	error_export(t_msh *msh, char *str)
{
	char	*message;
	char	*tmp;

	tmp = mcgic(mlcp(ft_strjoin("minishell: export: `", str), 1), ADD, NO_ENV,
			msh);
	message = mcgic(mlcp(ft_strjoin(tmp, "\': not a valid identifier\n"), 1),
			ADD, NO_ENV, msh);
	mcgic(mlcp(tmp, 0), FREE, NO_ENV, msh);
	ft_putstr_fd(message, 2);
	mcgic(mlcp(message, 0), FREE, NO_ENV, msh);
}
