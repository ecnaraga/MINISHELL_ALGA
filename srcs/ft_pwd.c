/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 14:42:04 by athiebau          #+#    #+#             */
/*   Updated: 2023/12/11 17:46:59 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	builtin_pwd(t_msh *minish)
{
	t_env	*tmp;

	tmp = *(minish->env);
	while (tmp)
	{
		if (!ft_strcmp(tmp->name, "PWD"))
		{
			// printf("%s%s\n", (char *)tmp->name, (char *)tmp->content);
			break ;
		}
		tmp = tmp->next;
	}
}
