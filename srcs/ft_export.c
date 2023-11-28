/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 14:13:14 by athiebau          #+#    #+#             */
/*   Updated: 2023/11/28 14:37:56 by athiebau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	fill_env(t_list **export_env, char **str)
{
	int		i;
	t_list	*new;

	i = 0;
	if (!str || str[0] == NULL)
	{
		ft_magic_malloc(FREE, 0, env, ENV);
		return (1);
	}
	while (str[i])
	{
		new = ft_lst_new_malloc(ft_strlen(str[i]) + 1);
		if (!new)
		{
			ft_magic_malloc(FLUSH, 0, NULL, ENV);
			return (1);
		}
		ft_strlcpy(new->content, str[i], ft_strlen(str[i]) + 1);
		ft_lstadd_back(env, new);
		i++;
	}
	return (0);
}

void	builtin_export(char **str, t_msh *minish)
{
	if (!minish->export_env)
	{
		minish->export_env = ft_magic_malloc(MALLOC, sizeof(t_list), NULL, ENV);
		if (!minish->export_env)
			return ;
		minish->export_env = NULL;
		if (fill_env(minish->export_env, str) == 1)
		return (NULL);
	}	
}