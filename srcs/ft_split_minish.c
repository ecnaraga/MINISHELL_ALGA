/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_minish.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 11:01:59 by garance           #+#    #+#             */
/*   Updated: 2023/11/02 14:02:43 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
TO DO : A BIEN TESTER : VERIF COUNTWORD + FT_TEST MINISHELL ET STRLCPYMINISHELL
PAS LES MME + FAIRE MASSE DE TESTS X VERIF QUE CA TIENT LA ROUTE
*/

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

	wd = 0;
	i = 0;
	d_q = 0;
	s_q = 0;
	while (s[i])
	{
		ft_inc_quote(s[i], &d_q, &s_q);
		while (s[i] && d_q % 2 == 0 && s_q % 2 == 0 && ft_is_isspace(s[i]) == 0)
			i++;
		if (s[i] && (d_q % 2 == 1 || s_q % 2 == 1 || (d_q % 2 == 0 && s_q % 2 == 0 && ft_is_isspace(s[i]) == 1)))
		{
			ft_inc_quote(s[i], &d_q, &s_q);
			wd++;
		}
		while (s[i] && (d_q % 2 == 1 || s_q % 2 == 1 || (d_q % 2 == 0 && s_q % 2 == 0 && ft_is_isspace(s[i]) == 1)))
			ft_inc_quote(s[i++], &d_q, &s_q);
	}
	return (wd);
}

/*
Compte le nb de char a copies et le renvoie
lt = nb de char a copies
k = nb de char qui seront copiees + nb de quote(double et single) qui ne seront
	pas copies car non suivies/ou precedees selon si fermant/ou ouvrant d'un
	issspace
*/
static int	ft_count_letter(const char *s, t_quote *q, int *i, int *k)
{
	int	lt;

	lt = 0;
	q->d = 0;
	q->s = 0;
	*k = 0;
	while (s[*i] && q->d % 2 == 0 && q->s % 2 == 0
		&& (s[*i] == '"' || s[*i] == 39 || ft_is_isspace(s[*i]) == 0))
	{
		ft_inc_quote(s[*i], &q->d, &q->s);
		*i += 1;
	}
	while (ft_test(s[*i], s[*i + 1], q->d, q->s) == 0)
	{
		ft_inc_quote(s[*i], &q->d, &q->s);
		if (ft_test_bis(s[*i], q->d, q->s) == 0)
		{
			lt++;
		}
		*i += 1;
		*k += 1;
	}
	return (lt);
}

/*
Boucle tant que le nb de mots contenus dans s n'est pas atteint et alloue de la
	memoire pour copie le nb de mots dans la data de la structure.
Dans le type de la structure, renseigne si le mot etait ou non dans des quotes
	ou "s'il contenait" des quotes qui ont ete effacees pour le parsing:
	- si type = ARG => Il y avait des quotes donc operateurs/dollar/... ne
		devront pas etre interpretes comme char speciaux
	- si type = TO_define => Il n'y avait pas de quotes
*/
static t_split	*ft_split_strs(const char *s, t_split *strs, int wd)
{
	int		j;
	int		lt;
	t_quote	q;
	int		k;
	int		i;

	i = 0;
	j = -1;
	while (s[i] && ++j < wd)
	{
		lt = ft_count_letter(s, &q, &i, &k);
		strs[j].data = (char *)malloc(sizeof(char) * (lt + 1)); // MALLOC DANS BOUCLE
		if (strs[j].data == NULL)
			return (ft_free_strs(strs, j));
		if (q.d > 0 || q.s > 0)
			strs[j].type = ARG;
		else
			strs[j].type = TO_DEFINE;
		if (lt > 0)
			ft_strlcpy_minish(strs[j].data, s + i - k - 1, lt + 1, i - k - 1);
		else
			strs[j].data[0] = '\0';
	}
	return (strs);
}

/*
Split la string et retourne un tableau de structure
data = mot;
type = Voir explication de ft_split_strs (ci-dessus)
Separateurs = Isspaces si PAS entre double ou single quote(d_q ou s_q)
Renvoie NULL en cas d'erreur de malloc
*/
t_split	*ft_split_minish(char const *s)
{
	int		wd;
	t_split	*strs;

	if (!s)
		return (NULL);
	wd = ft_countwords(s);
	strs = (t_split *)malloc(sizeof(t_split) * (wd + 1)); //MALLOC
	if (strs == NULL)
		return (NULL);
	if (ft_split_strs(s, strs, wd) == NULL) //MALLOC
		return (NULL);
	strs[wd].data = NULL;
	strs[wd].type = TO_DEFINE;
	return (strs);
}

// int	main(void)
// {
// 	int		i;
// 	char	*str0 = "H";
// 	char	*str1 = "a b";
// 	char	*str2 = "a      b      ";
// 	char	str3[] = {'a', ' ', ' ', ' ', 'b', ' ', '"', ' ', ' ', 'a', ' ', ' ', '"', '\0'};
// 	char	str4[] = {'a', ' ', ' ', ' ', 'b', ' ', '"', ' ', '"', '\0'};
// 	char	str5[] = {'a', ' ', ' ', ' ', 'b', ' ', 39, ' ', 39, '\0'};
// 	char	str6[] = {'a', ' ', ' ', ' ', 'b', ' ', 39, ' ', ' ', 'a', ' ', ' ', 39, '\0'};
// 	char	str7[] = {'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', '\0'};
// 	char	str8[] = {'a', ' ', ' ', ' ', 'b', ' ', 39,'"', ' ', ' ', 'a', ' ', ' ', '"', 39, '\0'};
// 	char	str9[] = {'"', 39, '"', '\0'};
// 	char	str10[] = {'"', 'a', '"', 'a','\0'};
// 	char	str11[] = {'"', 39, 'a', 39, '"', 'a','\0'};
// 	// char	str12[] = {'"', 39, 'a', 39, '"', 'a','\0'};
// 	char	str12[] = {39, '"', 'a', '"', 39, 'a','\0'};
// 	char	str13[] = {'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' ,'\0'};
// 	char	str14[] = {'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' , ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' ,'\0'};
// 	char	str15[] = {'e', 'c', 'h', 'o', ' ', '"', '-', '"', '"', 'n', 'n', 39, 'n', 'n', 39, '"', ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' , ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' ,'\0'};
// 	char	str16[] = {'e', 'c', 'h', 'o', ' ', '"', '"','\0'};

// 	printf("string = %s\nnb de mot = %d\n", str0, ft_countwords(str0));
// 	t_split *strs = ft_split_minish(str0);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str1, ft_countwords(str1));
// 	strs = ft_split_minish(str1);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str2, ft_countwords(str2));
// 	strs = ft_split_minish(str2);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str3, ft_countwords(str3));
// 	strs = ft_split_minish(str3);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str4, ft_countwords(str4));
// 	strs = ft_split_minish(str4);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str5, ft_countwords(str5));
// 	strs = ft_split_minish(str5);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str6, ft_countwords(str6));
// 	strs = ft_split_minish(str6);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str7, ft_countwords(str7));
// 	strs = ft_split_minish(str7);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str8, ft_countwords(str8));
// 	strs = ft_split_minish(str8);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str9, ft_countwords(str9));
// 	strs = ft_split_minish(str9);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str10, ft_countwords(str10));
// 	strs = ft_split_minish(str10);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str11, ft_countwords(str11));
// 	strs = ft_split_minish(str11);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str12, ft_countwords(str12));
// 	strs = ft_split_minish(str12);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str13, ft_countwords(str13)); //count letter ne passe pas
// 	strs = ft_split_minish(str13);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str14, ft_countwords(str14)); //count letter ne passe pas
// 	strs = ft_split_minish(str14);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str15, ft_countwords(str15)); //count letter ne passe pas
// 	strs = ft_split_minish(str15);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str16, ft_countwords(str16)); //count letter ne passe pas
// 	strs = ft_split_minish(str16);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// }
