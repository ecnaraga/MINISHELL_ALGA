/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 15:44:01 by athiebau          #+#    #+#             */
/*   Updated: 2023/11/02 14:39:18 by galambey         ###   ########.fr       */
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

// void	ft_inc_quote(char c, int *d_q, int *s_q)
// {
// 	if (*s_q % 2 == 0 && c == '"')
// 		*d_q += 1;
// 	else if (*d_q % 2 == 0 && c == 39)
// 		*s_q += 1;
// }

/*----------------------------------------------------------------------------------------------------*/

/*
Verifie que les " et ' sont bien apparies
	- quote entre quote 🗸
	- pouvoir ajouter plusieurs quote entre 2 mm quote : "'bra'vo" X
*/
int	ft_quote_order(char *str)
{
	char	*quote;
	int		count;
	int		i;
	int sq;
	int dq;

	(quote = malloc(sizeof(char) * ft_strlen(str)), ft_bzero(quote, sizeof(char)));
	count = -1;
	i = -1;
	dq = 0;
	sq = 0;
	while (str[++i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			if (count >= 0 && quote[count] == str[i])
				quote[count--] = '\0';
			else
				quote[++count] = str[i];
		}
	}
	if (count != -1)
	{
		printf("minishell: les guillemets ne sont pas correctement appariés.\n");
		//printf("minishell: syntax error); AJOUTER UN TRUC AVEC NB DES OP
		exit (2);
	}	
	free(quote);
	return (0);
}

/* 
Verifie que les chevrons sont bien assortis a leurs arguments : 
	- colles a l'argument suivant 🗸
	- colles a l'argument precedent 🗸
	- si rien apres renvoie erreur 🗸
	- espaces ignores 🗸
*/
int	ft_parse_chevron(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '>')
		{
			if (str[i + 1] == '>')
				i++;
			i++;
			while (str[i] == ' ')
				i++;
			if (!((str[i - 1] == ' ' || str[i - 1] == '>')
					&& (str[i] >= 33 && str[i] <= 126) && str[i] != '>'))
			{
				printf("minishell: syntax error near unexpected token `newline'\n");
				return (2);
			}
		}
	}
	return (0);
}

static int	is_separator(char c)
{
	if (c == '\t' || c == '\f' || c == '\n' || c == '\r' || c == '\v' || c == ' ' || c == '\0')
		return (1);
	else
		return (0);
}

static int	is_operator(char c)
{
	if (c == '|' || c == '(' || c == ')' || c == '<' || c == '>' || c == '&')
		return (1);
	else
		return (0);
}

/*
Ajouter des espaces avant et apres des operateurs
	- espaces apres 🗸
	- espaces avant 🗸
*/
char	*add_spaces(char *str)
{
	int	flag;
	int	i;
	int	count;
	char	*fstr;
	int	j;

	i = 0;
	flag = 0;
	count = 0;
	while(str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			flag = 1;
			i++;
		}
		if ((str[i] == '"' || str[i] == '\'') && flag == 1)
			flag = 0;
		if (is_operator(str[i]) == 1 && i != 0 && is_separator(str[i - 1]) == 0 && (str[i - 1] != str[i]))
			count++;
		if (is_operator(str[i]) == 1 && ((size_t)i < ft_strlen(str)) && is_separator(str[i + 1]) == 0 && str[i + 1] != str[i])
			count++;
		i++;
	}
	fstr = malloc(sizeof(char) * (i + count));
	i = 0;
	j = 0;
	while(str[i])
	{
		if (is_separator(str[i]) == 0 && is_operator(str[i + 1]) == 1)
		{
			if (is_operator(str[i]) == 0)
			{
				fstr[j] = str[i];
				j++;
				fstr[j] = ' ';
			}
			else
				fstr[j] = str[i];
		}
		else if (is_operator(str[i]) == 1 && is_separator(str[i + 1]) == 0 && is_operator(str[i - 1]) == 1)
		{
			fstr[j] = str[i];
			j++;
			fstr[j] = ' ';
		}
		else if (is_operator(str[i + 1]) == 0 || is_separator(str[i]) == 1)
			fstr[j] = str[i];
		i++;
		j++;
	}
	return (fstr);
}

int	ft_parse_line(t_msh *minish)
{
	if(ft_parse_chevron(minish->line) == 2)
		return (2);
	if (ft_quote_order(minish->line) == 2)
		return (2);
	minish->line = add_spaces(minish->line); // malloc
	printf("str : \"%s\"\n", minish->line);
	return (0);
}
/*int main(int ac, char **av)
{
	(void)ac;
	if (!av[1])
		return (2);
	char *str;
	ft_parse_chevron(av[1]);
	ft_quote_order(av[1]);
	str = add_spaces(av[1]); //malloc
	printf("str : \"%s\"\n", str);
	free(str);	
}*/
