/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_bis.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 12:45:33 by galambey          #+#    #+#             */
/*   Updated: 2023/11/02 17:54:05 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h"

/*QUESTIONS x ALIX : 
( ( bravo ) ) => Dans ce cas est ce que tu mets un espace entre les doubles parentheses?
*/

//TO DO : UNE FOIS ECHO ET EXEC IMPLEMENTER PENSER A VERIFIER EXIT STATUS X TOUS LES CAS D ERREUR

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
	int string_b;
	int string_a;
	int par;
	// int d_q;
	// int s_q;
	char *line;
	char *tmp;
	int par_o;
	int par_c;
	char prec;

	i = 0;
	par = 0;
	string_a = 0;
	string_b = 0;
	par_o = 0;
	par_c = 0;
	prec = TO_DEFINE;
	while (minish->line[i])
	{
		while (minish->line[i] && ft_is_isspace(minish->line[i]) == 0)
		{
			if (string_b == 2)
				string_b = 3;
			i++;
		}
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
		else if ((string_a == 1 || par == 0 || par_o > 1) && minish->line[i] && (minish->line[i] == '|' || minish->line[i] == '&'))
		{
			string_a = 0;
			prec = OPERATOR;
			if ((string_b == 3 || string_b == 4) && par_c == 0)
				string_b = 5;
			i++;
		}
		else if (minish->line[i] && minish->line[i] == '(')
		{
			printf("minish->line[%d] = %c prec = %d\n", i, minish->line[i], prec);
			if (string_a == 1 || prec == PAR_CLOSE)
				return (ft_error_syntax("minishell: syntax error near unexpected token `('\n", 2));
			if (par > 0 && (string_b == 0 || string_b == 3))
				string_b = 1;
			par += 1;
			par_o += 1;
			prec = PAR_OPEN;
			i++;
		}
		else if (minish->line[i] && minish->line[i] == ')')
		{
			printf("minish->line[%d] = %c prec = %d\n", i, minish->line[i], prec);
			if (/*par % 2 == 0 &&*/ par == 0) //attention ne gere pas les doubles parenthese donc si 2 par ouverte rentre dedans
				return (ft_error_syntax("minishell: syntax error near unexpected token `)'\n", 2));
			if (string_a == 0)
				return (ft_error_syntax("minishell: syntax error near unexpected token `)'\n", 2));
			if (string_b == 1 || string_b == 2 || ((string_b == 3 || string_b == 5) && par_c + 1 == par_o))
				return (ft_error_syntax("erreur a ne pas imprimer\n", 1)); //revoir exitstatus
			if (string_b == 4)
				return (ft_error_syntax("erreur exit status 1\n", 1)); //revoir exitstatus
			// else if (fonction pour traiter le parsing post fermeture pparenthese)
			par -= 1;
			par_c += 1;
			prec = PAR_CLOSE;
			i++;
		}
		else
		{
			string_a = 1;
			prec = OTHER;
			if (string_b == 1) // double parenthese ((
				string_b = 2; // double parenthese + 1 char (( a
			if (string_b == 3) // double parenthese + 1 char + 1 isspace (( a_
			{
				
				string_b = 4; // double parenthese + 1 char + 1 isspace + 1 char (( a a
			}
			i++;
		}
		// if (i > 0)
		// printf("minish->line[%d] = |%c| ; par = %d ; string_a = %d\n", i - 1, minish->line[i - 1], par, string_a);
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
TESTS OK MAIS VERIFIER EXITSTATUS: 
( bravo )
( b ravo )
( bravo ) && ( bravo )
( bravo ) ( bravo ) => minishell: syntax error near unexpected token `('
( echo bravo ) ( bravo ) => minishell: syntax error near unexpected token `('
( echo bravo ) && ( bravo )
( bravo ) && ( echo bravo )
( bravo
" ( bravo )"
" ( bravo "
" ( ( ( bravo "

TESTS A GERER :
doit retourner 1 en exit status et ne rien faire:  
(( bravo ))
((bravo && bravo))

doit fonctionner : 
((bravo) && (bravo))


doit retourner 1 en exit status et
( ( echo bravo ) ) => bash: ((: echo  bravo: syntax error in expression (error token is "bravo")
((bravo  bravo)) => bash: ((: bravo  bravo: syntax error in expression (error token is "bravo")

doit retourner 2 en exit status et
((bravo)  (bravo)) => bash: syntax error near unexpected token `('
*/

