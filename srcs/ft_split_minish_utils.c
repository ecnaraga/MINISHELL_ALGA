/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_minish_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garance <garance@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 11:01:59 by garance           #+#    #+#             */
/*   Updated: 2023/11/27 22:37:29 by garance          ###   ########.fr       */
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
	if (c == '"' && ((c1 && *c1 && ft_isspace(*c1) == 1) || (cm1
				&& ft_isspace(*cm1) == 1)))
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
int	ft_alloc_type(t_split *new)
{
	int	d;

	if (new->dollar > 0)
	{
		new->type = ft_magic_malloc(MALLOC, sizeof(t_dollar)
				* new->dollar, NULL, NO_ENV);
		if (new->type == NULL)
			return (1);
		d = -1;
		while (++d < new->dollar)
			new->type[d].expnd = TO_DEFINE;
	}
	return (0);
}
