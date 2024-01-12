/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_minish_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 11:01:59 by garance           #+#    #+#             */
/*   Updated: 2024/01/10 15:30:26 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
Teste les conditions de la boucle de ft_count_letter pour compter le nb de k
Renvoie 0 si ok pour rentrer dans la boucle, sinon 1
c : char courant dans la string
c1 : char + 1 dans la string
cm1 : char -1 dans la string
*/
int	ft_test(char c, const char *c1, const char *cm1, t_quote *q)
{
	if (!c)
		return (1);
	if (q->d % 2 == 1)
		return (0);
	if (q->s % 2 == 1)
		return (0);
	if (ft_isspace(c) == 0)
		return (1);
	if (c != '"' && c != 39)
		return (0);
	if (c == '"' && ((c1 && *c1 && ft_isspace(*c1) == 1)
			|| (cm1 && ft_isspace(*cm1) == 1)))
		return (0);
	if (c == 39 && ((c1 && *c1 && ft_isspace(*c1) == 1) || (cm1
				&& ft_isspace(*cm1) == 1)))
		return (0);
	else
		return (1);
}

/*
Test all the conditions in ft_count_letter to count the number of lt
Renvoie 0 if ok to get into the loop, otherwise 1
*/
int	ft_test_bis(char c, int d_q, int s_q)
{
	if (c != '"' && c != 39)
		return (0);
	if (c == '"' && s_q % 2 == 1)
		return (0);
	if (c == 39 && d_q % 2 == 1)
		return (0);
	else
		return (1);
}

/*
Alloue dynamiquement un tableau de structure de la taille du nb potentielles
	variables d environnement (dollar) presentes dans le mot qui permettra de
	preciser s'il faudra expand ou non la variable potentielle
Dynamically allocates a structure array of the size of the number of potential
	environment variables (dollar) present in the word, which will be used to
	specify whether or not expand the potential variable
*/
void	ft_alloc_type(t_split *new, t_msh *msh, int l)
{
	int	d;

	if (new->dollar > 0)
	{
		new->type = mcgic(
				mlcp(NULL, sizeof(t_dollar) * new->dollar), MLC, NO_ENV, msh);
		if (new->type == NULL)
			ft_exit(-1, -1, -1, msh);
		d = -1;
		while (++d < new->dollar)
			new->type[d].expnd = TO_DEFINE;
	}
	new->wild = NULL;
	if (new->wildcard > 0)
	{
		new->wild = mcgic(mlcp(NULL, sizeof(int) * l), MLC, NO_ENV, msh);
		if (new->wild == NULL)
			ft_exit(-1, -1, -1, msh);
	}
}

void	ft_count_dlw(const char *s, int *i, t_quote *q, t_letter *l)
{
	ft_inc_quote(s[*i], &q->d, &q->s);
	if (q->wildcard == 0 && s[*i] == '*')
		q->wildcard += 1;
	if (s[*i] == '$')
	{
		if (*i == 0 || s[*i - 1] != '$'
			|| (q->mod_dollar % 2 == 0 && s[*i - 1] == '$'))
			q->dollar += 1;
		q->mod_dollar += 1;
	}
	if (ft_test_bis(s[(*i)++], q->d, q->s) == 0)
		l->lt++;
	l->k += 1;
}
