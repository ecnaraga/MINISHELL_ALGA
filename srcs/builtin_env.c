/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 17:57:04 by athiebau          #+#    #+#             */
/*   Updated: 2024/01/09 17:32:31 by athiebau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	builtin_env(t_msh *minish)
{
	t_env	*new;

	new = *(minish->env);
	new_env_node_env(minish, "_=/usr/bin/env", 2, minish->env);
	while (new)
	{
		if (new->print == 1)
			printf("%s=%s\n", new->name, new->content);
		new = new->next;
	}
}
