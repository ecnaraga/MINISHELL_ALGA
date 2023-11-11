/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy_minish.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garance <garance@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 11:03:47 by garance           #+#    #+#             */
/*   Updated: 2023/11/11 13:07:11 by garance          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_init_var(t_index *x, t_quote *q)
{
	x->i = 1;
	x->j = 0;
	x->d = 0;
	q->d = 0;
	q->s = 0;
}

static int	ft_test_ter(char c, t_quote q)
{
	if (!c)
		return (1);
	if (q.d % 2 == 1)
		return (0);
	if (q.s % 2 == 1)
		return (0);
	if (c != '"' && c != 39 && ft_is_isspace(c) == 1)
		return (0);
	return (1);
}

static int ft_inc_d(t_split *strs, int *d, char c)
{
	if (!strs->type)
		return (1);
	if (*d >= strs->dollar)
		return (1);
	if (strs->type[*d].expnd == TO_DEFINE)
		return (1);
	if (c != '\'' && c != '"' && ft_is_isspace(c) == 1)
		return (1);
	*d += 1;
	return (0);
}

static void	ft_dollar(t_split *strs, const char *src, t_index *x, t_quote q)
{
	if (src[x->i] == '$')
	{
		if (strs->type[x->d].expnd != TO_DEFINE)
		{
			if (src[x->i - 1] == '$')
				strs->type[x->d].expnd = MULTI_DOLLAR;
			if (src[x->i + 1] != '$')
				x->d += 1;
		}
		else if (q.s % 2 == 0)
			strs->type[x->d].expnd = EXPAND;
		else
			strs->type[x->d].expnd = NO_EXPAND;
		strs->type[x->d].len_variable = 0;
	}
}
//if 1 on ne fait pas
static int	ft_test_four(t_split *strs, const char *src, t_index *x, t_quote q)
{
	if (!strs->type)
		return (1);
	if (ft_inc_d(strs, &x->d, src[x->i]) == 0)
		return (1);
	if (x->d >= strs->dollar)
		return (1);
	if (strs->type[x->d].expnd != EXPAND)
		return (1);
	if	(src[x->i] != '$')
		return (0);
	else if (q.d % 2 == 1)
		return (0);
	else if (!src[x->i + 1] || (src[x->i + 1] != '"' && src[x->i + 1] != '\''))
		return (0);
	return (1);
}

/*
Copie dans dst size - 1 char de src. Saute les quotes non compris entre quotes
	et non suivi/precede d'un isspace selon si fermant ou ouvrant
*/
void	ft_strlcpy_msh(t_split *strs, const char *src, size_t size, int begin)
{
	t_index	x;
	t_quote	q;

	ft_init_var(&x, &q);
	if (begin > -1)
		ft_inc_quote(src[0], &q.d, &q.s);
	while (size > 0 && src[x.i] && x.j < (size - 1))
	{
		ft_inc_quote(src[x.i], &q.d, &q.s);
		ft_dollar(strs, src, &x, q);
		if (ft_test_ter(src[x.i], q) == 0)
		{
			if (ft_test_four(strs, src, &x, q) == 0)
				strs->type[x.d].len_variable += 1;
			if (ft_test_bis(src[x.i], q.d, q.s) == 0)
				strs->data[x.j++] = src[x.i++];
			else
				x.i++;
		}
		else
			ft_inc_d(strs, &x.d, src[x.i++]);
	}
	strs->data[x.j] = '\0';
}
