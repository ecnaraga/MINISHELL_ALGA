/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 15:44:01 by athiebau          #+#    #+#             */
/*   Updated: 2023/11/20 16:23:58 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minishell.h"

// size_t	ft_strlen(const char *str)
// {
// 	int	i;

// 	i = 0;
// 	while (str[i])
// 		i++;
// 	return (i);
// }

// void	ft_bzero(void *s, size_t n)
// {
// 	size_t	i;
// 	char	*str;

// 	i = 0;
// 	str = s;
// 	while (i < n)
// 	{
// 		str[i] = '\0';
// 		i++;
// 	}
// }

static int	is_operator(char c)
{
	if (c == '(' || c == ')' || c == '>' || c == '<' || c == '&' || c == '|')
		return (1);
	else
		return (0);
}

/*----------------------------------------------------------------------------*/

/*
Verifie que les " et ' sont bien apparies
	- quote entre quote 🗸
	- pouvoir ajouter plusieurs quote entre 2 mm quote : "'bra'vo" 🗸
*/
static void	ft_quote_order_bis(char *str, char c, int *i, int *nb_q)
{
	*i += 1;
	*nb_q += 1;
	while (str[*i] != c && str[*i])
		*i += 1;
	if (str[*i] == c)
		*nb_q += 1;
	if (str[*(i + 1)])
		*i += 1;
}

static int	ft_quote_order(char *str)
{
	int	dq;
	int	sq;
	int	i;

	dq = 0;
	sq = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			if (str[i] == '"')
				ft_quote_order_bis(str, str[i], &i, &dq);
			if (str[i] == '\'')
				ft_quote_order_bis(str, str[i], &i, &sq);
		}
		else
			i++;
	}
	if (dq % 2 != 0 || sq % 2 != 0)
	{
		printf("minishell: les quote ne sont pas appariees\n");
		return (2);
	}
	return (0);
}

/*
Ajouter des espaces avant et apres des operateurs
	- espaces apres 🗸
	- espaces avant 🗸
	- double operateurs 🗸
	- calcul du malloc 🗸
	- guillemets X
*/
char	*add_spaces(char *str)
{
	char	*fstr;
	size_t	i;
	size_t	j;
	int		count;
	int		flag;

	i = 0;
	j = 0;
	count = 0;
	flag = 0;
	while (str[i])
	{
		if ((str[i] == '"' || str[i] == '\'') && flag == 0)
		{
			if (str[i] == '"')
				flag = 1;
			else if (str[i] == '\'')
				flag = 2;
			i++;
		}
		if ((str[i] == '"' && flag == 1) || (str[i] == '\'' && flag == 2))
			flag = 0;
		if (is_operator(str[i]) == 1 && flag == 0)
		{
			if ((i > 0 && ft_isspace(str[i - 1]) == 1 && is_operator(str[i
						- 1]) == 0))
				count++;
			if (i < ft_strlen(str) && (str[i + 1] == str[i]))
				i += 1;
			if ((i + 1 < ft_strlen(str) && ft_isspace(str[i + 1]) == 1)
				&& !((str[i] == '>' && str[i + 1] == '<') || (str[i] == '<'
						&& str[i + 1] == '>')) && !((str[i] == '|' && str[i
						+ 1] == '&') || (str[i] == '&' && str[i + 1] == '|')))
				count++;
		}
		i++;
	}
	// printf("size str : %zu\n", i);
	// printf("size fstr : %zu\n", i + count);
	fstr = malloc(sizeof(char) * (i + count + 1));
	i = 0;
	flag = 0;
	while (str[i])
	{
		if ((str[i] == '"' || str[i] == '\'') && flag == 0)
		{
			if (str[i] == '"')
				flag = 1;
			else if (str[i] == '\'')
				flag = 2;
			fstr[j++] = str[i++];
		}
		if ((str[i] == '"' && flag == 1) || (str[i] == '\'' && flag == 2))
			flag = 0;
		if (is_operator(str[i]) == 1 && flag == 0)
		{
			if (i > 0 && ft_isspace(str[i - 1]) == 1 && is_operator(str[i
					- 1]) == 0)
			{
				if (is_operator(str[i - 1]) == 0)
					fstr[j++] = ' ';
				fstr[j] = str[i];
			}
			else if (i < ft_strlen(str) && (str[i] == str[i + 1]))
				fstr[j] = str[i];
			else if (i < ft_strlen(str) && i > 0 && (str[i] == str[i - 1]))
				fstr[j] = str[i];
			if (i < ft_strlen(str) && (ft_isspace(str[i + 1]) == 1)
				&& !((str[i] == '>' && str[i + 1] == '<') || (str[i] == '<'
						&& str[i + 1] == '>')) && !((str[i] == '>' && str[i
						+ 1] == '(') || (str[i] == '<' && str[i + 1] == '(')))
			{
				if (is_operator(str[i + 1]) == 0 || (is_operator(str[i
							+ 1]) == 1 && str[i] != str[i + 1]))
				{
					fstr[j] = str[i];
					if (i + 1 < ft_strlen(str) && !((str[i] == '|' && str[i
								+ 1] == '&') || (str[i] == '&' && str[i
								+ 1] == '|')))
						fstr[++j] = ' ';
				}
			}
			else if (i < ft_strlen(str))
				fstr[j] = str[i];
		}
		else
			fstr[j] = str[i];
		i++;
		j++;
	}
	fstr[j] = '\0';
	return (fstr);
}

int	ft_parse_line(t_msh *minish)
{
	char *tmp;
	/*if(ft_parse_chevron(minish->line) == 2)
		return (2);
	if(operator_pairing(minish->line) == 2)
		return (2);*/
	if (ft_quote_order(minish->line) == 2)
		return (2);
	tmp = minish->line;
	minish->line = add_spaces(minish->line); // malloc
	free(tmp);
	printf("Sortie ft_parse_line : _%s_\n", minish->line);
	return (0);
}

// int	main(int ac, char **av)
// {
// 	char	*str;
// 	char	*fstr;

// 	(void)ac;
// 	(void)av;
// 	while (1)
// 	{
// 		str = readline("Oui$ "); // malloc
// 		if (!str)
// 			return (1);
// 		printf("str avant : _%s_\n", str);
// 		/*if (operator_pairing(str) != 2)
// 		{
// 			if (ft_parse_chevron(str) != 2)
// 			{*/
// 				if (ft_quote_order(str) != 2)
// 				{
// 					fstr = add_spaces(str); // malloc
// 					free(str);
// 					printf("fstr apres : _%s_\n", fstr);
// 					free(fstr);
// 				}
// 			//}
// 		//}
// 		printf("--------------------------------\n");
// 	}
// }
