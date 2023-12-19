/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 17:57:04 by athiebau          #+#    #+#             */
/*   Updated: 2023/12/19 14:16:32 by athiebau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	builtin_env(t_msh *minish)
{
	t_env *new;

	//printf("BUILTIN\n");
	new = *(minish->env);
	while (new)
	{
		if (new->content[0])
			printf("%s=%s\n", new->name, new->content);
		new = new->next;
	}
}