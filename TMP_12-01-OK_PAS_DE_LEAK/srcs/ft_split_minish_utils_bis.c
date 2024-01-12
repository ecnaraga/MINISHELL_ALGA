/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_minish_utils_bis.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 11:03:33 by garance           #+#    #+#             */
/*   Updated: 2024/01/10 15:28:16 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_init_var(int *n1, int *n2, t_quote *q)
{
	*n1 = 0;
	*n2 = 0;
	q->d = 0;
	q->s = 0;
	q->wildcard = 0;
	q->mod_dollar = 0;
	q->dollar = 0;
}

/*
Compte le nb de char a copies et le renvoie + compte le nb de potentielles
	variables d'environnement 
lt = nb de char a copies
k = nb de char qui seront copiees + nb de quote(double et single) qui ne seront
	pas copies car non suivies/ou precedees selon si fermant/ou ouvrant d'un
	issspace // incrementation a faire pour passer au mot suivant
dollar = nb de potentielles variables d environnement (qui commencent par un $)
	ps : une suite de dollar est comptee comme un seul dollar
*/
t_letter	ft_count_letter(const char *s, t_quote *q, int *i)
{
	t_letter	l;

	ft_init_var(&l.lt, &l.k, q);
	while (s[*i] && q->d % 2 == 0 && q->s % 2 == 0 && (s[*i] == '"'
			|| s[*i] == 39 || ft_isspace(s[*i]) == 0))
		ft_inc_quote(s[(*i)++], &q->d, &q->s);
	if (i == 0 && s[*i] && ft_test(s[*i], &s[*i + 1], NULL, q) == 0)
		ft_count_dlw(s, i, q, &l);
	while (s[*i] && ft_test(s[*i], &s[*i + 1], &s[*i - 1], q) == 0)
	{
		if (s[*i] != '$')
			q->mod_dollar = 0;
		ft_count_dlw(s, i, q, &l);
	}
	return (l);
}
