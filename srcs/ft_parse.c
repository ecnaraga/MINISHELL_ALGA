/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 15:44:01 by athiebau          #+#    #+#             */
/*   Updated: 2023/11/08 16:41:05 by galambey         ###   ########.fr       */
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

/*----------------------------------------------------------------------------------------------------*/

/* 
Verifie que les chevrons sont bien assortis a leurs arguments : 
	- colles a l'argument suivant 🗸
	- colles a l'argument precedent 🗸
	- si rien apres renvoie erreur 🗸
	- espaces ignores 🗸
*/
static int	ft_parse_chevron(char *str)
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

/*
Verifie que les " et ' sont bien apparies
	- quote entre quote 🗸
	- pouvoir ajouter plusieurs quote entre 2 mm quote : "'bra'vo" 🗸
*/
static int	ft_quote_order(char *str)
{
	char	*quote;
	int		count;
	int		i;
	int sq;
	int dq;
	int flag;

	(quote = malloc(sizeof(char) * ft_strlen(str)), ft_bzero(quote, sizeof(char)));
	count = -1;
	i = -1;
	dq = 0;
	sq = 0;
	flag = 0;
	while (str[++i])
	{
		if ((str[i] == '"'  && flag == 1 ) || (str[i] == '\'' && flag == 2))
			flag = 0;
		if ((str[i] == '"' || str[i] == '\'') && flag == 0)
		{
			if(str[i] == '"') 
				flag = 1;
			else if (str[i] == '\'')
				flag = 2;
			i++;
		}
		if ((str[i] == '"' || str[i] == '\'') && flag == 0)
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

static int	is_separator(char c)
{
	if (c == '\t' || c == '\f' || c == '\n' || c == '\r' || c == '\v' || c == ' ')
		return (1);
	else
		return (0);
}

static int	is_operator(char c)
{
	if (c == '|' || c == '<' || c == '>' || c == '&' || c == '(' || c == ')')
		return (1);
	else
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
	size_t	i = 0;
	size_t	j = 0;
	int	count = 0;
	//int	flag = 0;

	while(str[i])
	{
		/*if ((str[i] == '"' || str[i] == '\'') && flag == 0)
		{
			if(str[i] == '"') 
				flag = 1;
			else if (str[i] == '\'')
				flag = 2;
			i++;
		}
		if ((str[i] == '"'  && flag == 1 ) || (str[i] == '\'' && flag == 2))
			flag = 0;*/
		if (is_operator(str[i]) == 1) 
		{
            		if (i > 0 && is_separator(str[i - 1]) == 0 && is_operator(str[i - 1]) == 0)
				count++;
            		if (i < ft_strlen(str) && (str[i + 1] == str[i]))
				i += 1;
            		 if (i + 1 < ft_strlen(str) && is_separator(str[i + 1]) == 0)
				count++;
        	} 
             	i++;
	}
	// printf("str size : %zu\n", i);
	// printf("fstr size : %lu\n", i + count);
	fstr = malloc(sizeof(char) * (i + count + 1));
	i = 0;
	
	// fstr = malloc(sizeof(char) * (28));
	//flag = 0;
	while (str[i]) 
	{
		/*if ((str[i] == '"' || str[i] == '\'') && flag == 0)
		{
			if(str[i] == '"') 
				flag = 1;
			else if (str[i] == '\'')
				flag = 2;
			i++;
		}
		if ((str[i] == '"'  && flag == 1 ) || (str[i] == '\'' && flag == 2))
			flag = 0;*/
		if (is_operator(str[i]) == 1)
		{
            if (i > 0 && is_separator(str[i - 1]) == 0 && is_operator(str[i - 1]) == 0)
	   		{
				if (is_operator(str[i - 1]) == 0)
					fstr[j++] = ' ';
				fstr[j] = str[i];
			}
	    	else if (i < ft_strlen(str) && ((str[i + 1] == str[i]) || str[i] == str[i - 1]))
			{	
				fstr[j] = str[i];
			}
            if (i < ft_strlen(str) && (is_separator(str[i + 1]) == 0))
	    	{
				if (is_operator(str[i + 1]) == 0 || (is_operator(str[i + 1]) == 1 && str[i] != str[i + 1]))
				{	
					
					fstr[j] = str[i];
					if (i + 1 < ft_strlen(str))
                				fstr[++j] = ' '; 
				}
					
            }
			else if (i < ft_strlen(str) && is_separator(str[i + 1]) == 1)
			{	
				fstr[j] = str[i];
			}
        }
		else
		{
            fstr[j] = str[i];
		}
		i++;
		j++;
	}
    	fstr[j] = '\0';
	return (fstr);
}

int	ft_parse_line(t_msh *minish)
{
	if(ft_parse_chevron(minish->line) == 2)
		return (2);
	if (ft_quote_order(minish->line) == 2)
		return (2);
	minish->line = add_spaces(minish->line); // malloc
	printf("str : _%s_\n", minish->line);
	return (0);
}

// int main(int ac, char **av)
// {
// 	char *str;
// 	char *fstr;
// 	(void)ac;
// 	(void)av;
// 	while (1)
// 	{
// 		str = readline("Oui$ "); //malloc
// 		if (!str)
// 			return (1);
// 		//ft_parse_chevron(str);
// 		//ft_quote_order(av[1]);
// 		printf("str avant : \"%s\"\n", str);
// 		fstr = add_spaces(str); //malloc
// 		free(str);
// 		printf("fstr apres : \"%s\"\n", fstr);
// 		printf("--------------------------------\n");
// 		free(fstr);
// 	}	
// }
