/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy_minish_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 11:03:47 by garance           #+#    #+#             */
/*   Updated: 2024/01/10 15:32:12 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
Incremente la case du tableau de struct strs.type lorsque est rencontree la fin
	d'une potentielle variable d'environnement marquee par un quote ou isspace
*/
int	ft_inc_d(t_split *strs, int *d, char c)
{
	if (!strs->type)
		return (1);
	if (*d >= strs->dollar)
		return (1);
	if (strs->type[*d].expnd == TO_DEFINE)
		return (1);
	if (c != '\'' && c != '"' && ft_isspace(c) == 1)
		return (1);
	*d += 1;
	return (0);
}

/*
Si le char courant (src[x->i]) == '$', definit le type de l expnd:
MULTI_DOLLAR : Suite de dollar
EXPAND : Variable d environnement
NO_EXPAND : String normale 
*/
void	ft_dollar(t_split *strs, const char *src, t_index *x, t_quote q)
{
	if (src[x->i] == '$')
	{
		if (strs->type[x->d].expnd != TO_DEFINE)
		{
			if (x->mod_dollar % 2 == 1)
				strs->type[x->d].expnd = MULTI_DOLLAR;
			else
				x->d += 1;
		}
		if (strs->type[x->d].expnd == TO_DEFINE && q.s % 2 == 0)
			strs->type[x->d].expnd = EXPAND;
		else if (strs->type[x->d].expnd == TO_DEFINE)
			strs->type[x->d].expnd = NO_EXPAND;
		strs->type[x->d].len_variable = 0;
		x->mod_dollar += 1;
	}
}
