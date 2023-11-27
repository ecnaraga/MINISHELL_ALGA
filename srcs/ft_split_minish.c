/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_minish.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 11:03:33 by garance           #+#    #+#             */
/*   Updated: 2023/11/27 10:09:22 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_init_var(int *n1, int *n2, int *n3, int *n4)
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

	ft_init_var(&wd, &i, &d_q, &s_q);
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
Compte le nb de char a copies et le renvoie + compte le nb de potentielles
	variables d'environnement 
lt = nb de char a copies
k = nb de char qui seront copiees + nb de quote(double et single) qui ne seront
	pas copies car non suivies/ou precedees selon si fermant/ou ouvrant d'un
	issspace
dollar = nb de potentielles variables d environnement (qui commencent par un $)
	ps : une suite de dollar est comptee comme un seul dollar
*/
static t_letter	ft_count_letter(const char *s, t_quote *q, int *i, int *dollar)
{
	t_letter	l;

	ft_init_var(&l.lt, &l.k, &q->d, &q->s);
	while (s[*i] && q->d % 2 == 0 && q->s % 2 == 0
		&& (s[*i] == '"' || s[*i] == 39 || ft_isspace(s[*i]) == 0))
		ft_inc_quote(s[(*i)++], &q->d, &q->s);
	if (i == 0 && s[*i] && ft_test(s[*i], &s[*i + 1], NULL, q) == 0)
	{
		ft_inc_quote(s[*i], &q->d, &q->s);
		if (s[*i] == '$' && (*i == 0 || s[*i - 1] != '$'))
			*dollar += 1;
		if (ft_test_bis(s[(*i)++], q->d, q->s) == 0)
			l.lt++;
		l.k += 1;
	}
	while (s[*i] && ft_test(s[*i], &s[*i + 1], &s[*i - 1], q) == 0)
	{
		ft_inc_quote(s[*i], &q->d, &q->s);
		if (s[*i] == '$' && (*i == 0 || s[*i - 1] != '$'))
			*dollar += 1;
		if (ft_test_bis(s[(*i)++], q->d, q->s) == 0)
			l.lt++;
		l.k += 1;
	}
	return (l);
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
static t_split	*ft_split_strs(const char *s, t_split *strs, int wd)
{
	int			j;
	t_letter	l;
	t_quote		q;
	int			i;

	i = 0;
	j = -1;
	while (s[i] && ++j < wd)
	{
		strs[j].dollar = 0;
		l = ft_count_letter(s, &q, &i, &strs[j].dollar);
		strs[j].data = ft_magic_malloc(MALLOC, sizeof(char) * (l.lt + 1), NULL, NO_ENV);
		if (strs[j].data == NULL)
			return (NULL);
		strs[j].token = TO_DEFINE;
		if (ft_alloc_type(strs, j) == 1)
			return (NULL);
		if (l.lt > 0)
			ft_strlcpy_msh(&strs[j], s + i - l.k - 1, l.lt + 1, i - l.k - 1);
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
t_split	*ft_split_msh(char const *s)
{
	int		wd;
	t_split	*strs;

	if (!s)
		return (NULL);
	wd = ft_countwords(s);
	strs = ft_magic_malloc(MALLOC, sizeof(t_split) * (wd + 1), NULL, NO_ENV);
	if (strs == NULL)
		return (NULL);
	if (ft_split_strs(s, strs, wd) == NULL)
		return (NULL);
	strs[wd].data = NULL;
	strs[wd].type = NULL;
	return (strs);
}

// int	main(void)
// {
// 	int		i;
// 	int		d;
// 	char	*str0 = "H";
// 	char	*str1 = "a b";
// 	char	*str2 = "a      b      ";
// 	char	*str3 = "a   b \"  a  \"";
// 	char	*str4 = "a   b \" \"";
// 	char	*str5 = "a   b \' \'";
// 	char	*str6 = "a   b \'  a  \'";
// 	char	*str7 = "a   b \"\'  a  \'\"";
// 	char	*str8 = "a   b \'\"  a  \"\'";
// 	char	*str9 = "\"\'\"";
// 	char	*str10 = "\"a\"a";
// 	char	*str11 = "\"\'a\'\"a";
// 	char	*str12 = "\'\"a\"\'a";
// 	char	*str13 = "a   b \"\'  a  \'\"\'ab\'\"ab\"";
// 	char	*str14 = "a   b \"\'  a  \'\"\'ab\'\"ab\" a   b \"\'  a  \'\"\'ab\'\"ab\"";
// 	char	*str15 = "echo \"-\"\"nn\'nn\'\" a   b \"\'  a  \'\"\'ab\'\"ab\" a   b \"\'  a  \'\"\'ab\'\"ab\"";
// 	char	*str16 = "echo \"\"";
// 	char	*str17 = "echo \"-n\" Hola";
// 	char	*str18 = "echo \" \" | cat -e";
// 	char	*str19 = "echo \"\" hola";
// 	char	*str20 = "echo\" \" | cat -e";
// 	char	*str21 = "\"    \"echo\"    \" | cat -e";
// 	char	*str22 = "echo \"\"hola";
// 	char	*str23 = "export HOLA=\" -n bonjour \"";
// 	char	*str24 = "export \"\" HOLA=bonjour";
// 	char	*str25 = "echo \"\"\"$HOLA\"\"\" | cat -e";
// 	char	*str26 = "export HOLA=\" hola et $HOLA\"$HOLA";
// 	char	*str27 = "export HOLA=\" bonjour hey \"";
// 	char	*str28 = "export HOLA=\' hola et $HOLA\'";
// 	char	*str29 = "env | \"wc\" -l";
// 	char	*str30 = "export \"\" || ls";
// 	char	*str31 = "export \"\" && ls";
// 	char	*str32 = "export \"\" && unset \"\"";

// 	//TESTS TYPE
// 	char	*str33 = "echo $USER"; // >> Doit etre expand 
// 	char	*str34 = "echo \"$USER\""; // >> Doit etre expand
// 	char	*str35 = "echo \'$USER\'"; // >> Ne doit pas etre expand
// 	char	*str36 = "echo \'\"$USER\"\'"; // >> Ne doit pas etre expand
// 	char	*str37 = "echo \"\'$USER\'\""; // >> Doit etre expand
// 	char	*str38 = "echo \"\'$  USER\'\""; // >> Doit etre expand
// 	char	*str39 = "echo \"\'$$$  $USER\'\""; // >> Doit etre expand
// 	char	*str40 = "echo \"\'$$$  $USER\'$USER\"\'$USER\'$USER\"$US\"ER\"$USER\"";
// 	char	*str41 = "echo $\"USER\""; //le dollar ne devra pas s afficher
// 	char	*str42 = "echo \"$\"\"USER\""; //le dollar doit s afficher
// 	char	*str43 = "echo $ \"USER\""; //le dollar doit s afficher
// 	char	*str44 = "echo \"$\"\"\""; //le dollar doit s afficher
// 	char	*str45 = "echo $\"\""; //le dollar ne devra pas s afficher
// 	char	*str46 = "echo $\'USER\'"; //le dollar ne devra pas s afficher
// 	char	*str47 = "echo \'$\'\'USER\'"; //le dollar doit s afficher
// 	char	*str48 = "echo $ \'USER\'"; //le dollar doit s afficher
// 	char	*str49 = "echo \'$\'\'\'"; //le dollar doit s afficher
// 	char	*str50 = "echo $\'\'"; //le dollar ne devra pas s afficher
// 	char	*str51 = "echo $\"\'USER\'\""; //le dollar ne devra pas s afficher
// 	char	*str52 = "$USER$HELLOYOU"; //les dollars devront s afficher
// 	char	*str53 = "$USER$LESS$$$$USER"; //les dollars devront s afficher
// 	// ""''echo hola""'''' que""'' tal""'' => SPLIT MAUVAIS echo doit avoir type 0 et les autres mots type 5
// 	// echo $"" => SPLIT $ en type 5 au lieu de 0

// 	printf("string = %s\nnb de mot = %d\n", str0, ft_countwords(str0));
// 	t_split *strs = ft_split_msh(str0);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str1, ft_countwords(str1));
// 	strs = ft_split_msh(str1);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str2, ft_countwords(str2));
// 	strs = ft_split_msh(str2);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str3, ft_countwords(str3));
// 	strs = ft_split_msh(str3);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str4, ft_countwords(str4));
// 	strs = ft_split_msh(str4);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str5, ft_countwords(str5));
// 	strs = ft_split_msh(str5);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str6, ft_countwords(str6));
// 	strs = ft_split_msh(str6);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str7, ft_countwords(str7));
// 	strs = ft_split_msh(str7);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str8, ft_countwords(str8));
// 	strs = ft_split_msh(str8);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str9, ft_countwords(str9));
// 	strs = ft_split_msh(str9);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str10, ft_countwords(str10));
// 	strs = ft_split_msh(str10);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str11, ft_countwords(str11));
// 	strs = ft_split_msh(str11);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str12, ft_countwords(str12));
// 	strs = ft_split_msh(str12);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str13, ft_countwords(str13)); //count letter ne passe pas
// 	strs = ft_split_msh(str13);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str14, ft_countwords(str14)); //count letter ne passe pas
// 	strs = ft_split_msh(str14);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str15, ft_countwords(str15)); //count letter ne passe pas
// 	strs = ft_split_msh(str15);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str16, ft_countwords(str16)); //count letter ne passe pas
// 	strs = ft_split_msh(str16);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str17, ft_countwords(str17)); //count letter ne passe pas
// 	strs = ft_split_msh(str17);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str18, ft_countwords(str18)); //count letter ne passe pas
// 	strs = ft_split_msh(str18);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str19, ft_countwords(str19)); //count letter ne passe pas
// 	strs = ft_split_msh(str19);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str20, ft_countwords(str20)); //count letter ne passe pas
// 	strs = ft_split_msh(str20);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str21, ft_countwords(str21)); //count letter ne passe pas
// 	strs = ft_split_msh(str21);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str22, ft_countwords(str22)); //count letter ne passe pas
// 	strs = ft_split_msh(str22);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str23, ft_countwords(str23)); //count letter ne passe pas
// 	strs = ft_split_msh(str23);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str24, ft_countwords(str24)); //count letter ne passe pas
// 	strs = ft_split_msh(str24);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str25, ft_countwords(str25)); //count letter ne passe pas
// 	strs = ft_split_msh(str25);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str26, ft_countwords(str26)); //count letter ne passe pas
// 	strs = ft_split_msh(str26);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str27, ft_countwords(str27)); //count letter ne passe pas
// 	strs = ft_split_msh(str27);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str28, ft_countwords(str28)); //count letter ne passe pas
// 	strs = ft_split_msh(str28);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str29, ft_countwords(str29)); //count letter ne passe pas
// 	strs = ft_split_msh(str29);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str30, ft_countwords(str30)); //count letter ne passe pas
// 	strs = ft_split_msh(str30);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str31, ft_countwords(str31)); //count letter ne passe pas
// 	strs = ft_split_msh(str31);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str32, ft_countwords(str32)); //count letter ne passe pas
// 	strs = ft_split_msh(str32);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str33, ft_countwords(str33)); //count letter ne passe pas
// 	strs = ft_split_msh(str33);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str34, ft_countwords(str34)); //count letter ne passe pas
// 	strs = ft_split_msh(str34);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str35, ft_countwords(str35)); //count letter ne passe pas
// 	strs = ft_split_msh(str35);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str36, ft_countwords(str36)); //count letter ne passe pas
// 	strs = ft_split_msh(str36);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str37, ft_countwords(str37)); //count letter ne passe pas
// 	strs = ft_split_msh(str37);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str38, ft_countwords(str38)); //count letter ne passe pas
// 	strs = ft_split_msh(str38);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str39, ft_countwords(str39)); //count letter ne passe pas
// 	strs = ft_split_msh(str39);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str40, ft_countwords(str40)); //count letter ne passe pas
// 	strs = ft_split_msh(str40);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str41, ft_countwords(str41)); //count letter ne passe pas
// 	strs = ft_split_msh(str41);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str42, ft_countwords(str42)); //count letter ne passe pas
// 	strs = ft_split_msh(str42);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str43, ft_countwords(str43)); //count letter ne passe pas
// 	strs = ft_split_msh(str43);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str44, ft_countwords(str44)); //count letter ne passe pas
// 	strs = ft_split_msh(str44);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str45, ft_countwords(str45)); //count letter ne passe pas
// 	strs = ft_split_msh(str45);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str46, ft_countwords(str46)); //count letter ne passe pas
// 	strs = ft_split_msh(str46);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str47, ft_countwords(str47)); //count letter ne passe pas
// 	strs = ft_split_msh(str47);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str48, ft_countwords(str48)); //count letter ne passe pas
// 	strs = ft_split_msh(str48);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str49, ft_countwords(str49)); //count letter ne passe pas
// 	strs = ft_split_msh(str49);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str50, ft_countwords(str50)); //count letter ne passe pas
// 	strs = ft_split_msh(str50);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str51, ft_countwords(str51)); //count letter ne passe pas
// 	strs = ft_split_msh(str51);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str52, ft_countwords(str52)); //count letter ne passe pas
// 	strs = ft_split_msh(str52);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// //
// 	printf("\nstring = %s\nnb de mot = %d\n", str53, ft_countwords(str53)); //count letter ne passe pas
// 	strs = ft_split_msh(str53);
// 	i = -1;
// 	while (strs[++i].data)
// 	{
// 		printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 		// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 		free(strs[i].data);
// 		if (strs[i].type)
// 		{
// 			d = -1;
// 			while (++d < strs[i].dollar)
// 				printf("strs[%d].type[%d].expnd = %d strs[%d].type[%d].len_variable = %d\n", i, d, strs[i].type[d].expnd, i, d, strs[i].type[d].len_variable);
// 			free(strs[i].type);
// 		}
// 	}
// 	printf("strs[%d].data = |%s|\n", i, strs[i].data);
// 	// printf("strs[%d].type = |%d|\n", i, strs[i].type);
// 	free(strs);
// }
