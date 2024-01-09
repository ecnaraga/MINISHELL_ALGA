/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 17:57:04 by athiebau          #+#    #+#             */
/*   Updated: 2024/01/09 11:56:37 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	builtin_env(t_msh *minish)
{
	t_env	*new;

	new = *(minish->env);	
	while (new)
	{
		if (new->content[0])
			printf("%s=%s\n", new->name, new->content);
		new = new->next;
	}
}
