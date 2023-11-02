/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_bis.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 12:45:33 by galambey          #+#    #+#             */
/*   Updated: 2023/11/02 14:41:31 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h"

/*QUESTIONS x ALIX : 
( ( bravo ) ) => Dans ce cas est ce que tu mets un espace entre les doubles parentheses?
*/

int ft_error_syntax(char *str, int exitstatus) // a modifier
{
	ft_putstr_fd(str, 2);
	return (exitstatus);
}

//penser a gerer les parentheses dans des quotes
int ft_parse_bis(t_msh *minish)
{
	int	i;
	// int j;
	// int string_b;
	int string_a;
	int par;
	// int d_q;
	// int s_q;
	char *line;
	char *tmp;

	i = 0;
	par = 0;
	string_a = 0;
	// string_b = 0;
	// d_q = 0;
	// s_q = 0;
	while (minish->line[i])
	{
		while (minish->line[i] && ft_is_isspace(minish->line[i]) == 0)
			i++;
		if (minish->line[i] && minish->line[i] == '"')
		{
			string_a = 1;
			while (minish->line[++i] && minish->line[i] != '"');
		}
		else if (minish->line[i] && minish->line[i] == 39)
		{
			string_a = 1;
			while (minish->line[++i] && minish->line[i] != 39);
		}
		else if ((string_a == 1 || par == 0) && minish->line[i] && (minish->line[i] == '|' || minish->line[i] == '&'))
		{
			string_a = 0;
			i++;
		}
		else if (minish->line[i] && minish->line[i] == '(')
		{
			if (string_a == 1)
				return (ft_error_syntax("minishell: syntax error near unexpected token `('\n", 2));
			par += 1;
			i++;
		}
		else if (minish->line[i] && minish->line[i] == ')')
		{
			if (/*par % 2 == 0 &&*/ par == 0) //attention ne gere pas les doubles parenthese donc si 2 par ouverte rentre dedans
				return (ft_error_syntax("minishell: syntax error near unexpected token `)'\n", 2));
			else if (string_a == 0)
				return (ft_error_syntax("minishell: syntax error near unexpected token `)'\n", 2));
			// else if (fonction pour traiter le parsing post fermeture pparenthese)
			par -= 1;
			i++;
		}
		else
		{
			string_a = 1;
			i++;
		}
		// if (i > 0)
		// 	printf("minish->line[%d] = |%c| ; par = %d ; string_a = %d\n", i - 1, minish->line[i - 1], par, string_a);
	}
	if (par % 2 == 1) // POUR GERER SI PARENTHESE OUVERTE
	{
		line = readline("> ");
		tmp = minish->line;
		minish->line = ft_strjoin(minish->line, " "); // MALLOC
		// IF ERROR MALLOC
		free(tmp);
		tmp = minish->line;
		minish->line = ft_strjoin(minish->line, line); // MALLOC
		// IF ERROR MALLOC
		free(tmp);
		free(line);
		add_history(line); //voir comment supprimer derniere lg de l historique et remplacer par la nouvelle ici
		ft_parse_line(minish); // MALLOC
		// IF ERROR MALLOC
		ft_parse_bis(minish); // MALLOC
		// IF ERROR MALLOC
	}
	return (0);
}
/*
TESTS OK : 
( bravo )
( bravo ) && ( bravo )
( bravo ) ( bravo ) => minishell: syntax error near unexpected token `('
( echo bravo ) ( bravo ) => minishell: syntax error near unexpected token `('
( echo bravo ) && ( bravo )
( bravo ) && ( echo bravo )
( bravo
" ( bravo )"
" ( bravo "
" ((( bravo "

TESTS A GERER : 
( ( bravo ) ) doit retourner 1 en exit status et ne rien faire
( ( echo bravo ) )
*/

