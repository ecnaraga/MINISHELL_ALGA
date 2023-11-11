/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garance <garance@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 10:50:24 by galambey          #+#    #+#             */
/*   Updated: 2023/11/11 10:32:28 by garance          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


/*
Si (expnd = EXPAND = 1 et len variable = 1) ou si (expnd = MULTIDOLLAR = 3) => afficher 1 $
Si (expnd = NO_EXPAND = 2) => afficher les char tel quel et ne pas expand
Si (expnd = EXPAND = 1 et len_variable = 0) => ne pas afficher le dollar
Si (expnd = EXPAND = 1 et len_variable > 1) => expand : remplacer la variable d environnement par sa valeur dans l 'env
	exemple : si expnd = 1 && len_variable = 5 => remplacer $ + 4 char (ex : $USER) par la variable d environnement dans env
*/
int	ft_expand(t_msh *minish)
{
	int	i;

	i = -1;
	while (minish->line[++i])
	{
		if (minish->line[i] == '$')
	}
}
