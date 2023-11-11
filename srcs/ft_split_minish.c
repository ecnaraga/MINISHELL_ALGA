/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_minish.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garance <garance@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/25 11:01:59 by garance           #+#    #+#             */
/*   Updated: 2023/11/11 10:43:06 by garance          ###   ########.fr       */
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
		while (s[i] && d_q % 2 == 0 && s_q % 2 == 0 && ft_is_isspace(s[i]) == 0)
		{
			ft_inc_quote(s[i], &d_q, &s_q);
			i++;
		}
		if (s[i] && (d_q % 2 == 1 || s_q % 2 == 1 || (d_q % 2 == 0 && s_q % 2 == 0 && ft_is_isspace(s[i]) == 1)))
		{
			ft_inc_quote(s[i], &d_q, &s_q);
			wd++;
			i++;
		}
		while (s[i] && (d_q % 2 == 1 || s_q % 2 == 1 || (d_q % 2 == 0 && s_q % 2 == 0 && ft_is_isspace(s[i]) == 1)))
		{
			ft_inc_quote(s[i], &d_q, &s_q);
			i++;
		}
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
static int	ft_count_letter(const char *s, t_quote *q, int *i, int *k, int *dollar)
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
	while (s[*i] && ft_test(s[*i], s[*i + 1], s[*i - 1], q->d, q->s) == 0)
	{
		ft_inc_quote(s[*i], &q->d, &q->s);
		if (s[*i] == '$' && (*i == 0 || s[*i - 1] != '$'))
			*dollar += 1;
		if (ft_test_bis(s[*i], q->d, q->s) == 0)
			lt++;
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
	int		d;

	i = 0;
	j = -1;
	while (s[i] && ++j < wd)
	{
		strs[j].dollar = 0;
		lt = ft_count_letter(s, &q, &i, &k, &strs[j].dollar);
		strs[j].data = (char *)malloc(sizeof(char) * (lt + 1)); // MALLOC DANS BOUCLE
		if (strs[j].data == NULL)
			return (ft_free_strs(strs, j));
		strs[j].type = NULL;
		if (strs[j].dollar > 0)
		{
			strs[j].type = (t_dollar *)malloc(sizeof(t_dollar) * strs[j].dollar); // MALLOC DANS BOUCLE
			if (strs[j].type == NULL)
				return (ft_free_strs(strs, j)); //a modifier
			d = -1;
			while (++d < strs[j].dollar)
				strs[j].type[d].expnd = TO_DEFINE;
		}
		if (lt > 0)
			ft_strlcpy_minish(&strs[j], s + i - k - 1, lt + 1, i - k - 1);
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
	strs[wd].type = NULL;
	return (strs);
}

int	main(void)
{
	int		i;
	int		d;
	char	*str0 = "H";
	char	*str1 = "a b";
	char	*str2 = "a      b      ";
	char	*str3 = "a   b \"  a  \"";
	char	*str4 = "a   b \" \"";
	char	*str5 = "a   b \' \'";
	char	*str6 = "a   b \'  a  \'";
	char	*str7 = "a   b \"\'  a  \'\"";
	char	*str8 = "a   b \'\"  a  \"\'";
	char	*str9 = "\"\'\"";
	char	*str10 = "\"a\"a";
	char	*str11 = "\"\'a\'\"a";
	char	*str12 = "\'\"a\"\'a";
	char	*str13 = "a   b \"\'  a  \'\"\'ab\'\"ab\"";
	char	*str14 = "a   b \"\'  a  \'\"\'ab\'\"ab\" a   b \"\'  a  \'\"\'ab\'\"ab\"";
	char	*str15 = "echo \"-\"\"nn\'nn\'\" a   b \"\'  a  \'\"\'ab\'\"ab\" a   b \"\'  a  \'\"\'ab\'\"ab\"";
	char	*str16 = "echo \"\"";
	char	*str17 = "echo \"-n\" Hola";
	char	*str18 = "echo \" \" | cat -e";
	char	*str19 = "echo \"\" hola";
	char	*str20 = "echo\" \" | cat -e";
	char	*str21 = "\"    \"echo\"    \" | cat -e";
	char	*str22 = "echo \"\"hola";
	char	*str23 = "export HOLA=\" -n bonjour \"";
	char	*str24 = "export \"\" HOLA=bonjour";
	char	*str25 = "echo \"\"\"$HOLA\"\"\" | cat -e";
	char	*str26 = "export HOLA=\" hola et $HOLA\"$HOLA";
	char	*str27 = "export HOLA=\" bonjour hey \"";
	char	*str28 = "export HOLA=\' hola et $HOLA\'";
	char	*str29 = "env | \"wc\" -l";
	char	*str30 = "export \"\" || ls";
	char	*str31 = "export \"\" && ls";
	char	*str32 = "export \"\" && unset \"\"";

	//TESTS TYPE
	char	*str33 = "echo $USER"; // >> Doit etre expand 
	char	*str34 = "echo \"$USER\""; // >> Doit etre expand
	char	*str35 = "echo \'$USER\'"; // >> Ne doit pas etre expand
	char	*str36 = "echo \'\"$USER\"\'"; // >> Ne doit pas etre expand
	char	*str37 = "echo \"\'$USER\'\""; // >> Doit etre expand
	char	*str38 = "echo \"\'$  USER\'\""; // >> Doit etre expand
	char	*str39 = "echo \"\'$$$  $USER\'\""; // >> Doit etre expand
	char	*str40 = "echo \"\'$$$  $USER\'$USER\"\'$USER\'$USER\"$US\"ER\"$USER\"";
	char	*str41 = "echo $\"USER\""; //le dollar ne devra pas s afficher
	char	*str42 = "echo \"$\"\"USER\""; //le dollar doit s afficher
	char	*str43 = "echo $ \"USER\""; //le dollar doit s afficher
	char	*str44 = "echo \"$\"\"\""; //le dollar doit s afficher
	char	*str45 = "echo $\"\""; //le dollar ne devra pas s afficher
	char	*str46 = "echo $\'USER\'"; //le dollar ne devra pas s afficher
	char	*str47 = "echo \'$\'\'USER\'"; //le dollar doit s afficher
	char	*str48 = "echo $ \'USER\'"; //le dollar doit s afficher
	char	*str49 = "echo \'$\'\'\'"; //le dollar doit s afficher
	char	*str50 = "echo $\'\'"; //le dollar ne devra pas s afficher
	char	*str51 = "echo $\"\'USER\'\""; //le dollar ne devra pas s afficher
	// ""''echo hola""'''' que""'' tal""'' => SPLIT MAUVAIS echo doit avoir type 0 et les autres mots type 5
	// echo $"" => SPLIT $ en type 5 au lieu de 0

	printf("string = %s\nnb de mot = %d\n", str0, ft_countwords(str0));
	t_split *strs = ft_split_minish(str0);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str1, ft_countwords(str1));
	strs = ft_split_minish(str1);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str2, ft_countwords(str2));
	strs = ft_split_minish(str2);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str3, ft_countwords(str3));
	strs = ft_split_minish(str3);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str4, ft_countwords(str4));
	strs = ft_split_minish(str4);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str5, ft_countwords(str5));
	strs = ft_split_minish(str5);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str6, ft_countwords(str6));
	strs = ft_split_minish(str6);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str7, ft_countwords(str7));
	strs = ft_split_minish(str7);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str8, ft_countwords(str8));
	strs = ft_split_minish(str8);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str9, ft_countwords(str9));
	strs = ft_split_minish(str9);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str10, ft_countwords(str10));
	strs = ft_split_minish(str10);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str11, ft_countwords(str11));
	strs = ft_split_minish(str11);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str12, ft_countwords(str12));
	strs = ft_split_minish(str12);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str13, ft_countwords(str13)); //count letter ne passe pas
	strs = ft_split_minish(str13);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str14, ft_countwords(str14)); //count letter ne passe pas
	strs = ft_split_minish(str14);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str15, ft_countwords(str15)); //count letter ne passe pas
	strs = ft_split_minish(str15);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str16, ft_countwords(str16)); //count letter ne passe pas
	strs = ft_split_minish(str16);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str17, ft_countwords(str17)); //count letter ne passe pas
	strs = ft_split_minish(str17);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str18, ft_countwords(str18)); //count letter ne passe pas
	strs = ft_split_minish(str18);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str19, ft_countwords(str19)); //count letter ne passe pas
	strs = ft_split_minish(str19);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str20, ft_countwords(str20)); //count letter ne passe pas
	strs = ft_split_minish(str20);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str21, ft_countwords(str21)); //count letter ne passe pas
	strs = ft_split_minish(str21);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str22, ft_countwords(str22)); //count letter ne passe pas
	strs = ft_split_minish(str22);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str23, ft_countwords(str23)); //count letter ne passe pas
	strs = ft_split_minish(str23);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str24, ft_countwords(str24)); //count letter ne passe pas
	strs = ft_split_minish(str24);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str25, ft_countwords(str25)); //count letter ne passe pas
	strs = ft_split_minish(str25);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str26, ft_countwords(str26)); //count letter ne passe pas
	strs = ft_split_minish(str26);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str27, ft_countwords(str27)); //count letter ne passe pas
	strs = ft_split_minish(str27);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str28, ft_countwords(str28)); //count letter ne passe pas
	strs = ft_split_minish(str28);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str29, ft_countwords(str29)); //count letter ne passe pas
	strs = ft_split_minish(str29);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str30, ft_countwords(str30)); //count letter ne passe pas
	strs = ft_split_minish(str30);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str31, ft_countwords(str31)); //count letter ne passe pas
	strs = ft_split_minish(str31);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str32, ft_countwords(str32)); //count letter ne passe pas
	strs = ft_split_minish(str32);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str33, ft_countwords(str33)); //count letter ne passe pas
	strs = ft_split_minish(str33);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str34, ft_countwords(str34)); //count letter ne passe pas
	strs = ft_split_minish(str34);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str35, ft_countwords(str35)); //count letter ne passe pas
	strs = ft_split_minish(str35);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str36, ft_countwords(str36)); //count letter ne passe pas
	strs = ft_split_minish(str36);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str37, ft_countwords(str37)); //count letter ne passe pas
	strs = ft_split_minish(str37);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str38, ft_countwords(str38)); //count letter ne passe pas
	strs = ft_split_minish(str38);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str39, ft_countwords(str39)); //count letter ne passe pas
	strs = ft_split_minish(str39);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str40, ft_countwords(str40)); //count letter ne passe pas
	strs = ft_split_minish(str40);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str41, ft_countwords(str41)); //count letter ne passe pas
	strs = ft_split_minish(str41);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str42, ft_countwords(str42)); //count letter ne passe pas
	strs = ft_split_minish(str42);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str43, ft_countwords(str43)); //count letter ne passe pas
	strs = ft_split_minish(str43);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str44, ft_countwords(str44)); //count letter ne passe pas
	strs = ft_split_minish(str44);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str45, ft_countwords(str45)); //count letter ne passe pas
	strs = ft_split_minish(str45);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str46, ft_countwords(str46)); //count letter ne passe pas
	strs = ft_split_minish(str46);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str47, ft_countwords(str47)); //count letter ne passe pas
	strs = ft_split_minish(str47);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str48, ft_countwords(str48)); //count letter ne passe pas
	strs = ft_split_minish(str48);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str49, ft_countwords(str49)); //count letter ne passe pas
	strs = ft_split_minish(str49);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str50, ft_countwords(str50)); //count letter ne passe pas
	strs = ft_split_minish(str50);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
//
	printf("\nstring = %s\nnb de mot = %d\n", str51, ft_countwords(str51)); //count letter ne passe pas
	strs = ft_split_minish(str51);
	i = -1;
	while (strs[++i].data)
	{
		printf("strs[%d].data = |%s|\n", i, strs[i].data);
		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
		free(strs[i].data);
		if (strs[i].type)
		{
			d = -1;
			while (++d < strs[i].dollar)
				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
			free(strs[i].type);
		}
	}
	printf("strs[%d].data = |%s|\n", i, strs[i].data);
	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
	free(strs);
}
