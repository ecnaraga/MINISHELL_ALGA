/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_minish.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 11:03:33 by garance           #+#    #+#             */
/*   Updated: 2024/01/10 15:31:49 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_init_var_bis(int *n1, int *n2, int *n3, int *n4)
{
	*n1 = 0;
	*n2 = 0;
	*n3 = 0;
	*n4 = 0;
}

/*
Compte le nb de mots et le renvoie
Separateurs = Isspaces si PAS entre double ou single quote(d_q ou s_q)
*/
static int	ft_countwords(const char *s)
{
	int	wd;
	int	i;
	int	d_q;
	int	s_q;

	ft_init_var_bis(&wd, &i, &d_q, &s_q);
	while (s[i])
	{
		while (s[i] && d_q % 2 == 0 && s_q % 2 == 0 && ft_isspace(s[i]) == 0)
			ft_inc_quote(s[i++], &d_q, &s_q);
		if (s[i] && (d_q % 2 == 1 || s_q % 2 == 1
				|| (d_q % 2 == 0 && s_q % 2 == 0 && ft_isspace(s[i]) == 1)))
		{
			ft_inc_quote(s[i++], &d_q, &s_q);
			wd++;
		}
		while (s[i] && (d_q % 2 == 1 || s_q % 2 == 1
				|| (d_q % 2 == 0 && s_q % 2 == 0 && ft_isspace(s[i]) == 1)))
			ft_inc_quote(s[i++], &d_q, &s_q);
	}
	return (wd);
}

/*
Boucle tant que le nb de mots contenus dans s n'est pas atteint et alloue de la
	memoire pour copie le nb de mots dans la data de la structure.
Tableau de structure strs->type : strs[i]->type[d]
	Il sera rempli dans ft_strlcpy_msh
	Il y a autant de case que de dollar a interpreter dans le mot :
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
*/
static void	ft_split_strs(const char *s, t_split **strs, int wd, t_msh *msh)
{
	int			j;
	t_letter	l;
	t_quote		q;
	int			i;
	t_split		*new;

	i = 0;
	j = -1;
	while (s[i] && ++j < wd)
	{
		l = ft_count_letter(s, &q, &i);
		new = ft_lstnew_split(msh, q);
		new->data = mcgic(
				mlcp(NULL, sizeof(char) * (l.lt + 1)), MLC, NO_ENV, msh);
		if (new->data == NULL)
			ft_exit(-1, -1, -1, msh);
		new->token = TO_DEFINE;
		ft_alloc_type(new, msh, l.lt);
		if (l.lt >= 0)
			ft_strlcpy_msh(new, s + i - l.k - 1, l.lt + 1, i - l.k - 1);
		else
			new->data[0] = '\0';
		ft_lstadd_back_split(strs, new);
	}
}

/*
Split la string et retourne un tableau de structure
data = mot;
type = Voir explication de ft_split_strs (ci-dessus)
Separateurs = Isspaces si PAS entre double ou single quote(d_q ou s_q)
Renvoie NULL en cas d'erreur de malloc
*/
t_split	*ft_split_msh(char const *s, t_msh *msh)
{
	int		wd;
	t_split	*strs;

	if (!s)
		return (NULL);
	wd = ft_countwords(s);
	strs = NULL;
	ft_split_strs(s, &strs, wd, msh);
	return (strs);
}
