/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy_minish.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 11:03:47 by garance           #+#    #+#             */
/*   Updated: 2024/01/10 15:33:42 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_init_var(t_index *x, t_quote *q)
{
	x->i = 1;
	x->j = 0;
	x->d = 0;
	x->mod_dollar = 0;
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
	if (c != '"' && c != 39 && ft_isspace(c) == 1)
		return (0);
	return (1);
}

/*
Fonction permettant de deficir si le char courant (src[x->i]) fait ou non parti
	du nom de la variable d'environnement marquee par un $
Renvoie 0 si OUI
Renvoie 1 si NON 
*/
static int	ft_test_four(t_split *strs, const char *src, t_index *x, t_quote *q)
{
	if (!strs->type)
		return (1);
	if (ft_inc_d(strs, &x->d, src[x->i]) == 0)
		return (1);
	if (x->d >= strs->dollar)
		return (1);
	if (strs->type[x->d].expnd != EXPAND)
		return (1);
	if (src[x->i] != '$')
		return (0);
	else if (q->d % 2 == 1)
		return (0);
	else if (!src[x->i + 1] || (src[x->i + 1] != '"' && src[x->i + 1] != '\''))
		return (0);
	return (1);
}

static void	ft_exec_strlcpy_msh(t_split *strs, const char *src, t_index *x,
		t_quote *q)
{
	if (ft_test_four(strs, src, x, q) == 0)
		strs->type[x->d].len_variable += 1;
	if (src[x->i] == '$' && strs->type
		&& strs->type[x->d].expnd == MULTI_DOLLAR)
		x->i++;
	else if (ft_test_bis(src[x->i], q->d, q->s) == 0)
	{
		strs->data[x->j] = src[x->i++];
		if (strs->wildcard > 0)
		{
			if (q->s % 2 == 1 || q->d % 2 == 1 || strs->data[x->j] != '*')
				strs->wild[x->j] = 1;
			else
				strs->wild[x->j] = 0;
		}
		x->j++;
	}
	else
		x->i++;
}

/*
Copie dans dst size - 1 char de src. Saute les quotes non compris entre quotes
	et non suivi/precede d'un isspace selon si fermant ou ouvrant
+ Remplit le tableau de structure strs.type si 1 ou plusieurs dollars sont
	presents dans le mot copie, et precise l'instruction a suivre quand il
	faudra le traiter :
	Il y a autant de case que de potentielles variables d'envirronement a
	interpreter dans le mot :
	1. Si strs[i]->type == NULL => PAS DE DOLLAR DANS LE MOT
	2. Si strs[i]->type[d].expnd == EXPAND :
		- Si strs[i]->type[d].len_variable == 0 -> Ne pas afficher le $
		- Si strs[i]->type[d].len_variable == 1 -> Afficher 1 $
		- Si strs[i]->type[d].len_variable > 1 -> Expand la variable.
			Le nom de la variable sera compose des (len_variable - 1) char qui
			suivent le $
	3. Si strs[i]->type[d].expnd == NOT_EXPAND : Ne pas expand, afiicher
		normalement le $ et les char qui suivent
	4. Si strs[i]->type[d].expnd == MULTI_DOLLAR : Afficher 1 $ et sauter les
		suivants
begin ==
	-1 > cas de figure : "" ou '' ou "$USUSUSUS" ( a la difference de $USUSUSUS)
*/
void	ft_strlcpy_msh(t_split *strs, const char *src, size_t size, int begin)
{
	t_index	x;
	t_quote	q;

	ft_init_var(&x, &q);
	strs->quote = 0;
	if (begin > -1)
	{
		if (src[0] == '"' || src[0] == '\'')
			strs->quote = 1;
		ft_inc_quote(src[0], &q.d, &q.s);
	}
	if (begin == -1 && (src[1] == '"' || src[1] == '\''))
		strs->quote = 1;
	while (size > 0 && src[x.i] && x.j < (size - 1))
	{
		if (src[x.i] != '$')
			x.mod_dollar = 0;
		ft_inc_quote(src[x.i], &q.d, &q.s);
		ft_dollar(strs, src, &x, q);
		if (ft_test_ter(src[x.i], q) == 0)
			ft_exec_strlcpy_msh(strs, src, &x, &q);
		else
			ft_inc_d(strs, &x.d, src[x.i++]);
	}
	strs->data[x.j] = '\0';
}
