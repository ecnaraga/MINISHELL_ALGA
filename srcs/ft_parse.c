/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 15:44:01 by athiebau          #+#    #+#             */
/*   Updated: 2023/11/20 12:01:18 by athiebau         ###   ########.fr       */
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

static int	is_separator(char c)
{
	if (c == '\t' || c == '\f' || c == '\n' || c == '\r' || c == '\v'
		|| c == ' ')
		return (1);
	else
		return (0);
}

static int	is_operator(char c)
{
	if (c == '(' || c == ')' || c == '>' || c == '<' || c == '&' || c == '|')
		return (1);
	else
		return (0);
}

/*----------------------------------------------------------------------------*/

// static int	operator_pairing(char *str)
// {
// 	int	i;
// 	int 	flag;

// 	i = -1;
// 	flag = 0;
// 	while (str[++i])
// 	{
// 		if ((str[i] == '"' || str[i] == '\'') && flag == 0)
// 		{
// 			if (str[i] == '"')
// 				flag = 1;
// 			else if (str[i] == '\'')
// 				flag = 2;
// 			i++;
// 		}
// 		if ((str[i] == '"' && flag == 1) || (str[i] == '\'' && flag == 2))
// 			flag = 0;
// 		if (((str[i] == '&' || str[i] == '|') && (is_separator(str[i + 1]) == 1)
// 					&& str[i + 2] == str[i]) && flag == 0)
// 		{
// 			printf("minishell: syntax error near unexpected token `%c'\n",
// 				str[i]);
// 			return (2);
// 		}
// 	}
// 	return (0);
// }

/*
Verifie que les chevrons sont bien assortis a leurs arguments :
	- colles a l'argument suivant 🗸
	- colles a l'argument precedent 🗸
	- si rien apres renvoie erreur 🗸
	- espaces ignores 🗸
*/
// static int	ft_parse_chevron(char *str)
// {
// 	int	i;
// 	int	flag;
// 	char *line;

// 	i = -1;
// 	flag = 0;
// 	while (str[++i])
// 	{
// 		if ((str[i] == '"' || str[i] == '\'') && flag == 0)
// 		{
// 			if (str[i] == '"')
// 				flag = 1;
// 			else if (str[i] == '\'')
// 				flag = 2;
// 			i++;
// 		}
// 		if ((str[i] == '"' && flag == 1) || (str[i] == '\'' && flag == 2))
// 			flag = 0;
// 		if (str[i] == '>')
// 		{
// 			if (str[i + 1] == '>')
// 				i++;
// 			i++;
// 			while (str[i] == ' ')
// 				i++;
// 			if (!((str[i - 1] == ' ' || str[i - 1] == '>') && (str[i] >= 33
// 						&& str[i] <= 126) && str[i] != '>'))
// 			{
// 				printf("coucou garance\n");
// 				/*printf("minishell: ");
// 				printf("syntax error near unexpected token `newline'\n");*/
// 				while (is_separator(str[i++]) == 1);
// 				line = ft_error_message(str + (i -1)); //MALLOC
// 				//IF ERROR MALLOC
// 				return (ft_error_syntax(line, 2, 1));
// 				//return (2);
// 			}
// 		}
// 	}
// 	return (0);
// }

/*
Verifie que les " et ' sont bien apparies
	- quote entre quote 🗸
	- pouvoir ajouter plusieurs quote entre 2 mm quote : "'bra'vo" 🗸
*/

static int	ft_quote_order(char *str)
{
	int	dq = 0;
	int	sq = 0;
	int	i = -1;
	
	while (str[++i])
	{
		if (str[i] == '"')
		{
			i++;
			//printf("dans \" : str[%d] |%c|\n", i, str[i]);
			dq++;
			while(str[i] != '"' && str[i])
				i++;
			if (str[i] == '"')
				dq++;
			if (str[i + 1])
				i++;
			
		}
		//printf("apres \" : str[%d] |%c|\n", i, str[i]);
		//printf("dq : %d\n", dq);
		//printf("sq : %d\n", sq);
		if (str[i] == '\'')
		{
			i++;
			//printf("dans \' : str[%d] |%c|\n", i, str[i]);
			sq++;
			while(str[i] != '\'' && str[i])
				i++;
			if (str[i] == '\'')
				sq++;
			if (str[i + 1])
				i++;
		}
		//printf("apres \' : str[%d] |%c|\n", i, str[i]);
		//printf("dq : %d\n", dq);
		//printf("sq : %d\n", sq);
	}
	//printf("dq final : %d\n", dq);
	//printf("sq final : %d\n", sq);
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

	i = 0;
	j = 0;
	count = 0;
	int	flag = 0;
	while (str[i])
	{
		if ((str[i] == '"' || str[i] == '\'') && flag == 0)
		{
			if(str[i] == '"')
				flag = 1;
			else if (str[i] == '\'')
				flag = 2;
			i++;
		}
		if ((str[i] == '"'  && flag == 1 ) || (str[i] == '\'' && flag == 2))
			flag = 0;
		if (is_operator(str[i]) == 1 && flag == 0)
		{
			if ((i > 0 && is_separator(str[i - 1]) == 0 && is_operator(str[i
					- 1]) == 0))
				count++;
			if (i < ft_strlen(str) && (str[i + 1] == str[i]))
				i += 1;
			if ((i + 1 < ft_strlen(str) && is_separator(str[i + 1]) == 0) && !((str[i] == '>' && str[i + 1] == '<') || (str[i] == '<' && str[i + 1] == '>')) && !((str[i] == '|' && str[i + 1] == '&') || (str[i] == '&' && str[i + 1] == '|')))
				count++;
		}
		i++;
	}
	//printf("size str : %zu\n", i);
	//printf("size fstr : %zu\n", i + count);
	fstr = malloc(sizeof(char) * (i + count + 1));
	i = 0;
	flag = 0;
	while (str[i])
	{
		if ((str[i] == '"' || str[i] == '\'') && flag == 0)
		{
			if(str[i] == '"')
				flag = 1;
			else if (str[i] == '\'')
				flag = 2;
			fstr[j++] = str[i++];
		}
		if ((str[i] == '"'  && flag == 1 ) || (str[i] == '\'' && flag == 2))
			flag = 0;
		if (is_operator(str[i]) == 1 && flag == 0)
		{
			if (i > 0 && is_separator(str[i - 1]) == 0 && is_operator(str[i
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
			if (i < ft_strlen(str) && (is_separator(str[i + 1]) == 0) && !((str[i] == '>' && str[i + 1] == '<') || (str[i] == '<' && str[i + 1] == '>')) && !((str[i] == '>' && str[i + 1] == '(') || (str[i] == '<' && str[i + 1] == '(')))
			{
				
				if (is_operator(str[i + 1]) == 0 || (is_operator(str[i + 1]) == 1 && str[i] != str[i + 1]))
				{
					fstr[j] = str[i];
					if (i + 1 < ft_strlen(str) && !((str[i] == '|' && str[i + 1] == '&') || (str[i] == '&' && str[i + 1] == '|')))
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
//(echo "((((>>||bravo")("lfhrkjgbrg"

int	ft_parse_line(t_msh *minish)
{
	/*if(ft_parse_chevron(minish->line) == 2)
		return (2);
	if(operator_pairing(minish->line) == 2)
		return (2);*/
	if (ft_quote_order(minish->line) == 2)
		return (2);
	minish->line = add_spaces(minish->line); // malloc
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
