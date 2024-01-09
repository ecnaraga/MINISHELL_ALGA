/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 13:15:29 by galambey          #+#    #+#             */
/*   Updated: 2024/01/09 13:15:03 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// /*
// echo non envoye => a modifier si on veut l 'envoyer
// echo pourra etre dans des quotes ou non, cela restera la cmd echo
// les infile et outfile doivent etre gere avant appel a cette fonction

// TO DO IMPLEMENTER EXPAND dans echo + IMPLEMENTER * et /*
// * : liste les fichiers et dossiers du dossier courant => ex : a.out expand.c ft_echo.c ft_free_split_minish.c
// ./* : liste les fichiers et dossiers du dossier courant avec le chemin depuis le dossier courant => ex : ./a.out ./expand.c ./ft_echo.c ./ft_free_split_minish.c
// ../* : liste les fichiers et dossiers du dossier parent avec le chemin depuis le dossier parent => ex : ../C_13_piscine_arbre_binaire ../cat ../code erreurs.odt
// /* : ex : /bin /boot /core.240260 /dev /etc /goinfre /home
// CAS NON EXHAUSTIF => A COMPLETER ET TESTER AVEC STRACE + VOIR COMMENT RECUP LES ARGUMENTS
// */

/*
Retourne 0 si option -n, sinon retourne 0
	- -n
	- "-n"
	- -nnnnnnnnnnnnnnnn
	- "-nnnnn"
	- '-nn'
	- -"nnnnnnnnnnn"
	- -"nn"n"nnnnnnnn"
	- "-"nn'n'nnnnnnnn
	Ces cas sont normalement gere par ft_split_minish qui a retire les ""
	ou ' ' "non imprimables" et a indique ARG dans le type de la struct
Par contre : "-""nn'n'nnnnnnnn" n est pas considiere comme l option -n mais
	dans ce cas le parsing affiche les single quote donc normalement deja gere
Elle peut avoir 1 ou plusieurs n mais 1 seul tiret
*/
int	ft_option_echo(char *data)
{
	int i;

	i = 0;
	if (data[0] != '-')
		return (1);
	while (data[++i])
	{
		if (data[i] != 'n')
			return (1);
	}
	return (0);
}

/*
L'option -n peut etre entre quote (single OU double) ou non , ex :
	- -n
	- "-n"
	- -nnnnnnnnnnnnnnnn
	- "-nnnnn"
	- '-nn'
	- -"nnnnnnnnnnn"
	- -"nn"n"nnnnnnnn"
	- "-"nn'n'nnnnnnnn
	Ces cas sont normalement gere par ft_split_minish qui a retire les ""
	ou ' ' "non imprimables" et a indique ARG dans le type de la struct
Par contre : "-""nn'n'nnnnnnnn" n est pas considiere comme l option -n mais
	dans ce cas le parsing affiche les single quote donc normalement deja gere
Elle peut avoir 1 ou plusieurs n mais 1 seul tiret
*/
void	builtin_echo(t_msh *msh/* , t_split *av, int nb_arg */) //Il faudra envoyer la structure du node avec dans le node le t_split *av + l'env x pouvoir utiliser l expand
{
	int i;
	int new_line;

	i = 1;
	new_line = 0;
	while (msh->p.cmd_t[i] && ft_option_echo(msh->p.cmd_t[i]) == 0) // implementer expand dans boucle
	{
		new_line = 1;
		i++;
	}
	// while (i < nb_arg) // implementer expand dans boucle
	while (msh->p.cmd_t[i]) // implementer expand dans boucle
	{
		// if (av[i].type != ARG && ft_is_char(av[i].data, '$')) //ARG x l instant car que ce type de defini dans le split x noter qu'il n'y avait pas de quote
		// 	ft_expand(av, av[i].data); // devra remplacer $suivi de la chaine de char (ex $USER) par la valeur de la variable d environnement a chercher dans env
		ft_putstr_fd(msh->p.cmd_t[i], 1);
		i++;
		// if (i < nb_arg)
		if (msh->p.cmd_t[i])
			write(1, " ", 1);
	}
	if (new_line != 1)
		write(1, "\n", 1);
}

// a effacer
// static int	ft_countwords(const char *s)
// {
// 	int	wd;
// 	int	i;
// 	int	d_q;
// 	int	s_q;

// 	wd = 0;
// 	i = 0;
// 	d_q = 0;
// 	s_q = 0;
// 	while (s[i])
// 	{
// 		ft_inc_quote(s[i], &d_q, &s_q);
// 		while (s[i] && d_q % 2 == 0 && s_q % 2 == 0 && ft_is_sep(s[i]) == 0)
// 			i++;
// 		if (s[i] && (d_q % 2 == 1 || s_q % 2 == 1 || ft_is_sep(s[i]) == 1))
// 		{
// 			ft_inc_quote(s[i++], &d_q, &s_q);
// 			wd++;
// 		}
// 		while (s[i] && (d_q % 2 == 1 || s_q % 2 == 1 || ft_is_sep(s[i]) == 1))
// 			ft_inc_quote(s[i++], &d_q, &s_q);
// 	}
// 	return (wd);
// }

// int main(void)
// {
// 	t_split	*strs;
// 	char str0[] = "echo a b";
// 	char str1[] = "echo -n a b";
// 	char	str2[] = "a      b      ";
// 	char	str3[] = {'e', 'c', 'h', 'o', ' ', 'a', ' ', ' ', ' ', 'b', ' ', '"', ' ', ' ', 'a', ' ', ' ', '"', '\0'};
// 	char	str4[] = {'e', 'c', 'h', 'o', ' ', 'a', ' ', ' ', ' ', 'b', ' ', '"', ' ', '"', '\0'};
// 	char	str5[] = {'e', 'c', 'h', 'o', ' ', 'a', ' ', ' ', ' ', 'b', ' ', 39, ' ', 39, '\0'};
// 	char	str6[] = {'e', 'c', 'h', 'o', ' ', 'a', ' ', ' ', ' ', 'b', ' ', 39, ' ', ' ', 'a', ' ', ' ', 39, '\0'};
// 	char	str7[] = {'e', 'c', 'h', 'o', ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', '\0'};
// 	char	str8[] = {'e', 'c', 'h', 'o', ' ', 'a', ' ', ' ', ' ', 'b', ' ', 39,'"', ' ', ' ', 'a', ' ', ' ', '"', 39, '\0'};
// 	char	str9[] = {'e', 'c', 'h', 'o', ' ', '"', 39, '"', '\0'};
// 	char	str10[] = {'e', 'c', 'h', 'o', ' ', '"', 'a', '"', 'a','\0'};
// 	char	str11[] = {'e', 'c', 'h', 'o', ' ', '"', 39, 'a', 39, '"', 'a','\0'};
// 	char	str12[] = {'e', 'c', 'h', 'o', ' ', 39, '"', 'a', '"', 39, 'a','\0'};
// 	char	str13[] = {'e', 'c', 'h', 'o', ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' ,'\0'};
// 	char	str14[] = {'e', 'c', 'h', 'o', ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' , ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' ,'\0'};
// 	char	str15[] = {'e', 'c', 'h', 'o', ' ', '-', 'n', 'n', 'n',' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' , ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' ,'\0'};
// 	char	str16[] = {'e', 'c', 'h', 'o', ' ', '-', 'n', ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' , ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' ,'\0'};
// 	char	str17[] = {'e', 'c', 'h', 'o', ' ', '"', '-', 'n', '"', ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' , ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' ,'\0'};
// 	char	str18[] = {'e', 'c', 'h', 'o', ' ', '"', '-', 'n', 'n', 'n', 'n', '"', ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' , ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' ,'\0'};
// 	char	str19[] = {'e', 'c', 'h', 'o', ' ', 39, '-', 'n', 39, ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' , ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' ,'\0'};
// 	char	str20[] = {'e', 'c', 'h', 'o', ' ', 39, '-', 'n', 'n', 'n', 'n', 'n', 39, ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' , ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' ,'\0'};
// 	char	str21[] = {'e', 'c', 'h', 'o', ' ', '-', '"', 'n', 'n', 'n', 'n', '"', ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' , ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' ,'\0'};
// 	char	str22[] = {'e', 'c', 'h', 'o', ' ', '-', 39, 'n', 'n', 'n', 'n', 39, ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' , ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' ,'\0'};
// 	char	str23[] = {'e', 'c', 'h', 'o', ' ', '-', '"', 'n', '"', 'n', 'n', '"', 'n', '"', ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' , ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' ,'\0'};
// 	char	str24[] = {'e', 'c', 'h', 'o', ' ', '-', '"', 'n', '"', 'n', 'n', '"', 'n', '"', 'n', ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' , ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' ,'\0'};
// 	char	str25[] = {'e', 'c', 'h', 'o', ' ', '-', 39, 'n', 39, 'n', 'n', 39, 'n', 39, 'n', ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' , ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' ,'\0'};
// 	char	str26[] = {'e', 'c', 'h', 'o', ' ', '"', '-', '"', 'n', 'n', 39, 'n', 'n', 39, ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' , ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' ,'\0'};
// 	char	str27[] = {'e', 'c', 'h', 'o', ' ', '"', '-', '"', '"', 'n', 'n', 39, 'n', 'n', 39, '"', ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' , ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' ,'\0'};
// 	char	str28[] = {'e', 'c', 'h', 'o', ' ', '"', '"', '\0'};
// 	char	str29[] = {'e', 'c', 'h', 'o', ' ', '"', '-', '"', '"', 'n', 'n', 'n', 'n', '"', ' ', '"', '-', '"', '"', 'n', 'n', 'n', 'n', '"', ' ', '"', '-', '"', '"', 'n', 'n', 'n', 'n', '"', ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' , ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' ,'\0'};
// 	char	str30[] = {'e', 'c', 'h', 'o', ' ', '"', '-', '"', '"', 'n', 'n', 'b', 'n', 'n', '"', ' ', '"', '-', '"', '"', 'n', 'n', 'n', 'n', '"', ' ', '"', '-', '"', '"', 'n', 'n', 'n', 'n', '"', ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' , ' ', 'a', ' ', ' ', ' ', 'b', ' ','"', 39, ' ', ' ', 'a', ' ', ' ', 39, '"', 39, 'a', 'b', 39 , '"', 'a', 'b', '"' ,'\0'};

// 	printf("string : %s\n", str0);
// 	strs = ft_split_minish(str0);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str0) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str1);
// 	strs = ft_split_minish(str1);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str1) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");
	
// 	printf("string : %s\n", str2);
// 	strs = ft_split_minish(str2);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str2) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str3);
// 	strs = ft_split_minish(str3);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str3) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");
	
// 	printf("string : %s\n", str4);
// 	strs = ft_split_minish(str4);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str4) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str5);
// 	strs = ft_split_minish(str5);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str5) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str6);
// 	strs = ft_split_minish(str6);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str6) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str7);
// 	strs = ft_split_minish(str7);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str7) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str8);
// 	strs = ft_split_minish(str8);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str8) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str9);
// 	strs = ft_split_minish(str9);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str9) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str10);
// 	strs = ft_split_minish(str10);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str10) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str11);
// 	strs = ft_split_minish(str11);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str11) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str12);
// 	strs = ft_split_minish(str12);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str12) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str13);
// 	strs = ft_split_minish(str13);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str13) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str14);
// 	strs = ft_split_minish(str14);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str14) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str15);
// 	strs = ft_split_minish(str15);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str15) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");
	
// 	printf("string : %s\n", str16);
// 	strs = ft_split_minish(str16);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str16) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");
	
// 	printf("string : %s\n", str17);
// 	strs = ft_split_minish(str17);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str17) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str18);
// 	strs = ft_split_minish(str18);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str18) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");
	
// 	printf("string : %s\n", str19);
// 	strs = ft_split_minish(str19);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str19) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");
	
// 	printf("string : %s\n", str20);
// 	strs = ft_split_minish(str20);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str20) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str21);
// 	strs = ft_split_minish(str21);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str21) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str22);
// 	strs = ft_split_minish(str22);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str22) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str23);
// 	strs = ft_split_minish(str23);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str23) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str24);
// 	strs = ft_split_minish(str24);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str24) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str25);
// 	strs = ft_split_minish(str25);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str25) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str26);
// 	strs = ft_split_minish(str26);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str26) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str27);
// 	strs = ft_split_minish(str27);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str27) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");

// 	printf("string : %s\n", str28);
// 	strs = ft_split_minish(str28);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str28) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");
	
// 	printf("string : %s\n", str29);
// 	strs = ft_split_minish(str29);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str29) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");
	
// 	printf("string : %s\n", str30);
// 	strs = ft_split_minish(str30);
// 	if (strs && ft_strcmp(strs[0].data, "echo") == 0)
// 		ft_echo(strs + 1, ft_countwords(str30) - 1);
// 	ft_free_split_minish(strs);
// 	printf("\n");
// }
