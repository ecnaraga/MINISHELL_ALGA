/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 14:13:14 by athiebau          #+#    #+#             */
/*   Updated: 2023/11/30 15:56:26 by athiebau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_print_export(t_msh *minish)
{
	t_env	*tmp;

	tmp = *(minish->export_env);
	while (tmp)
	{
		printf("%s%s\n", (char *)tmp->name, (char *)tmp->content);
		tmp = tmp->next;
	}
}

static int	get_statut(char *cmd)
{
	int	i;

	i = -1;
	while(cmd[++i])
	{
		if (cmd[i] == '=')
			return (2);
	}
	return (1);	
}

static int	change_export_env(char *str, t_msh *minish)
{
	t_env	*new;
	int	key;
	int	name_size;
	int	content_size;

	name_size = get_name_size(str);
	content_size = ft_strlen(str + name_size);
	new = ft_lst_new_malloc(name_size + 1, content_size + 2 + 1);
	if (!new)
	{
		ft_magic_malloc(FLUSH, 0, NULL, ENV);
		return (1);
	}
	ft_strlcpy(new->name, str, name_size + 1);
	ft_exstrlcpy(new->content, str + name_size, content_size + 2 + 1);
	ft_lstadd_back_env(minish->export_env, new);
}

void	builtin_export(char **str, t_msh *minish)
{
	char	*name;
	char	*content;
	int	i;

	i = 1;
	if(!str[i])
		ft_print_export(minish);
	else
	{
		while(str[i])
		{
			if(get_statut(str[i]) == 1)
			{
				change_export_env(str[i], minish);
			}
			else
			{
				
			}

		}
		
	}
}
