/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_bis.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 12:45:33 by galambey          #+#    #+#             */
/*   Updated: 2023/12/20 15:56:08 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h"

//TO DO : UNE FOIS ECHO ET EXEC IMPLEMENTER PENSER A VERIFIER EXIT STATUS X
//TOUS LES CAS D ERREUR

//ATTENTION A GESTION ERREUR SI PARENTHESES OUVERTE...

/*
Initialize the parameters at 0
*/
static void	ft_init_var(t_par *p, int *i)
{
	*i = 0;
	p->par_o = 0;
	p->par_c = 0;
	p->prec_iss = TO_DEFINE;
	p->chev = 0;
	p->prec = TO_DEFINE;
	// p->multi_par = 0;
	p->multi_cmd = 0;
}

static int	ft_parse_parenthesis(t_msh *msh, t_par *p, t_storage *f, int *i)
{
	if (msh->line[*i] == '"' || msh->line[*i] == '\'')
		ft_pass_quote(msh, p, i, msh->line[*i]);
	else if (ft_isspace(msh->line[*i]) == 0)
	{
		p->prec = ISS;
		(*i)++;
	}
	else if (msh->line[*i] == '(' || msh->line[*i] == ')')
	{
		(*f) = ft_storage(msh->line[*i]);
		if ((*f)(msh, p, i) != 0)
			return (1);
	}
	else if (ck_char("&|><", msh->line[*i]) != 0)
	{
		(*f) = ft_storage(msh->line[*i]);
		if ((*f)(msh, p, i) == 0)
			return (2);
	}
	else if (ft_other_char(msh, p, i) != 0)
		return (1);
	return (0);
}

int ft_parse_bis(t_msh *msh)
{
	t_par	p;
	int	i;
	int res;
	t_storage function;
	
	ft_init_var(&p, &i);
	while (msh->line[i])
	{
		res = ft_parse_parenthesis(msh, &p, &function, &i);
		if (res == 1)
			return (1);
		if (res == 2)
			return (0);
	}
	if (p.par_c != p.par_o && p.prec_iss != OPERATOR) // POUR GERER SI PARENTHESE OUVERTE
		return (msh->status = 2, err_syntax("syntax error parenthesis expected closed\n"));
	return (0);
}

/*TESTS : 

1- DOIT FONCTIONNER :
( bravo )
(bravo)
( b ravo )
(b ravo)
( bravo ) && ( bravo )
(bravo) && (bravo)
( echo bravo ) && ( bravo )
(echo bravo) && (bravo)
( echo bravo )&&( bravo )
(echo bravo)&&(bravo)
( ( bravo ) )
( ( bravo ))
(( bravo ) )
( (bravo) )
((bravo) )
(( bravo) )
( ( bravo ) ) && ( ( bravo ) )
( ( bravo ) && ( bravo ) )
(( bravo ) && ( bravo ))
((bravo ) && ( bravo))
((bravo) && (bravo))
((bravo)&&(bravo))
( (bravo)&&(bravo) )
( ( echo bravo ) )
((echo bravo) )
( (echo bravo))
( ( echo bravo ))
(( echo bravo ) )
(( bravo ) && bravo )
((bravo) && bravo)
( (bravo) && bravo)
( ( bravo ) && bravo )
( bravo && (bravo))
( bravo && (bravo) )
( bravo && ( bravo ) )
( bravo && ( bravo ))
( ( ls && ls ) )
(( ls && ls ) )
( ( ls && ls ))
(( echo && ( bravo )) && la )
(( echo && ( bravo ) ) && la )
( ( echo && ( bravo ) ) && la )
( ( echo && ( bravo )) && la )
	
2- DOIT PASSER TEST PARSING MAIS EN EXEC : N IMPRIME RIEN + EXITSTATUS = 1 // AGERER DANS L EXEC
(( bravo ))
((bravo))
(( ls && ls ))
((ls && ls))
echo Bravo && ((ls && ls)) && echo hehe
echo Bravo && ((ls && ls)) || echo hehe
( ( (( )) ) )
(())
(( ))
>>>> parenthese collee gerees comme parenthese espacees car on doit gerer parentheses pour priorites

3- NE DOIT PAS PASSER DANS EXEC : bash: syntax error near unexpected token + EXIT STATUS 2
1. 
( bravo ) ( bravo )
(bravo) (bravo)
(bravo)(bravo)
( bravo )( bravo )
	=> minishell: syntax error near unexpected token `('
1. 
( echo bravo ) ( bravo )
(echo bravo) (bravo)
(echo bravo)(bravo)
( echo bravo )( bravo )=> minishell: syntax error near unexpected token `('
3. 
()
( )
(      )
	=> syntax error near unexpected token `)'
3. 
( ( ) ) 
	=> bash: syntax error near unexpected token `)'
3.
( ( ( ( ) ) ) ) 
	=> bash: syntax error near unexpected token `)'
4. 
)
		)
bravo )
=> syntax error near unexpected token `)'

2. 
( echo ( bravo ) )
( echo( bravo ))
( echo(bravo))
(echo (bravo))
( echoo ( bravo ) echo )
( echoo ( bravo ) echo)
(( echo ) bravo )
( ( echo ) bravo )
	=> minishell: syntax error near unexpected token `bravo'
1. 
(( bravo ) ( bravo ))
( (bravo)  (bravo) )
((bravo)  (bravo))
((bravo)(bravo))
((bravo)(bravo) )
( (bravo)(bravo) )
	=> minishell: syntax error near unexpected token `('
1. 
> hola (ls && pwd)
	=> bash: syntax error near unexpected token `('
1. 
> pwd (ls)

4- DOIT ALLER DANS EXEC ET ERREUR DU TYPE : minishell: ((: bravo  bravo: syntax error in expression (error token is "bravo")  + EXIT STATUS 2 // AGERER DANS L EXEC
(( echo bravo ))
((echo bravo))
	=> minishell: ((: echo bravo : syntax error in expression (error token is "bravo ")
( (( ( ) )) )
	=> minishell: ((: ( ) : syntax error: operand expected (error token is ") ")
(((())))
	=> minishell: ((: (()): syntax error: operand expected (error token is "))")
*/

/*
KO : 
export HOLA=bon(jour
export HOLA=bon()jour

bash-5.2$ echo ( bravo )
bash: syntax error near unexpected token `bravo'
bash-5.2$ echo echo ( bravo )
bash: syntax error near unexpected token `('
 
*/


// int ft_parse_bis(t_msh *minish)
// {
// 	int par;
// 	int par_o;
// 	int	multi_par_o;
// 	int	multi_par_condition_close;
// 	int par_c;
// 	int	multi_par_c;
// 	int par_and_oper;
// 	int prec;
// 	int prec_iss;
// 	int prec_type_par;
// 	int cmd;
// 	int other_char;
// 	char *line;
// 	char *tmp;
// 	int	i;
	
// 	i = 0;
// 	par = 0;
// 	par_o = 0;
// 	multi_par_o = 0;
// 	par_c = 0;
// 	multi_par_c = 0;
// 	par_and_oper = 0;
// 	prec = 0;
// 	prec_iss = 0;
// 	cmd = 0;
// 	other_char = 0;
// 	multi_par_condition_close = 0;
// 	prec_type_par = 0;
	
// 	while (minish->line[i])
// 	{
// 		if (minish->line[i] == '"')
// 		{
// 			other_char = 1;
// 			while (minish->line[++i] && minish->line[i] != '"');
// 		}
// 		else if (minish->line[i] == '\'')
// 		{
// 			other_char = 1;
// 			while (minish->line[++i] && minish->line[i] != '\'');
// 		}
// 		else if (ft_isspace(minish->line[i]) == 0)
// 		{
// 			// if (multi_par_o > 0 && prec_iss == OTHER && cmd == 0)
// 			// 	cmd = 1;
// 			printf("ISSSPACE BEF: minish->line[%d] = <%c>, prec = %d, prec_iss = %d, par_and_oper = %d, multi_par_o = %d, par_o = %d, multi_par_condition_close = %d, par_c = %d, par = %d\n", 
// 			i, minish->line[i], prec, prec_iss, par_and_oper, multi_par_o, par_o, multi_par_condition_close, par_c, par);
// 			prec = ISS;
// 			printf("ISSSPACE AF: minish->line[%d] = <%c>, prec = %d, prec_iss = %d, par_and_oper = %d, multi_par_o = %d, par_o = %d, multi_par_condition_close = %d, par_c = %d, par = %d\n\n", 
// 			i, minish->line[i], prec, prec_iss, par_and_oper, multi_par_o, par_o, multi_par_condition_close, par_c, par);
// 			i++;
// 		}
// 		else if (minish->line[i] == '(')
// 		{
// 			printf("( BEF: minish->line[%d] = <%c>, prec = %d, prec_iss = %d, par_and_oper = %d, multi_par_o = %d, par_o = %d, multi_par_condition_close = %d, par_c = %d, par = %d\n", 
// 			i, minish->line[i], prec, prec_iss, par_and_oper, multi_par_o, par_o, multi_par_condition_close, par_c, par);
// 			if (prec_iss == OPERATOR)
// 				par_and_oper += 1;
// 			if (prec == PAR_OPEN)
// 			{
// 				multi_par_o += 1;
// 				prec_type_par = 2;
// 			}
// 			else
// 				prec_type_par = 1;
// 			if (prec_iss == PAR_CLOSE /*|| prec_iss == OTHER*/)
// 				return (err_syntax("syntax error near unexpected token `('\n", 2, 0));
// 			if (prec_iss == OTHER)
// 			{
// 				while (ft_isspace(minish->line[++i]) == 0);
// 				line = ft_error_message(minish->line + i); //MALLOC
// 				//IF ERROR MALLOC
// 				return (err_syntax(line, 2, 1));
// 			}
// 			prec = PAR_OPEN;
// 			prec_iss = PAR_OPEN;
// 			par_o += 1;
// 			par += 1;
// 			printf("( AF: minish->line[%d] = <%c>, prec = %d, prec_iss = %d, par_and_oper = %d, multi_par_o = %d, par_o = %d, multi_par_condition_close = %d, par_c = %d, par = %d\n\n", 
// 			i, minish->line[i], prec, prec_iss, par_and_oper, multi_par_o, par_o, multi_par_condition_close, par_c, par);
// 			i++;
// 		}
// 		else if (minish->line[i] == ')')
// 		{
// 			printf(") BEF: minish->line[%d] = <%c>, prec = %d, prec_iss = %d, par_and_oper = %d, multi_par_o = %d, par_o = %d, multi_par_condition_close = %d, par_c = %d, par = %d\n", 
// 			i, minish->line[i], prec, prec_iss, par_and_oper, multi_par_o, par_o, multi_par_condition_close, par_c, par);
// 			if (prec_iss == PAR_OPEN)
// 				return (err_syntax("syntax error near unexpected token `)'\n", 2, 0));
// 			if (par_o <= par_c)
// 				return (err_syntax("syntax error near unexpected token `)'\n", 2, 0));
// 			if (prec == PAR_CLOSE && multi_par_o > 0 && prec_type_par == 2)
// 				return (err_syntax("", 2, 0));
// 			if (multi_par_o > 0 && 2 * multi_par_o == par)
// 				multi_par_condition_close += 1;
// 			// if (multi_par_condition_close == 1 && prec == PAR_CLOSE)//manque quelque chose
// 			// {	
// 			// 	multi_par_o -= 1;//
// 			// 	multi_par_condition_close = 0;
// 			// }
// 			prec = PAR_CLOSE;
// 			prec_iss = PAR_CLOSE;
// 			par_c += 1;
// 			par -= 1;
			
			
// 			// if (prec == PAR_CLOSE)
// 			// 	multi_par_c += 2;
// 			printf(") AF: minish->line[%d] = <%c>, prec = %d, prec_iss = %d, par_and_oper = %d, multi_par_o = %d, par_o = %d, multi_par_condition_close = %d, par_c = %d, par = %d\n\n", 
// 			i, minish->line[i], prec, prec_iss, par_and_oper, multi_par_o, par_o, multi_par_condition_close, par_c, par);
// 			i++;
// 		}
// 		else if (minish->line[i] == '&' || minish->line[i] == '|')
// 		{
// 			printf("OPERATOR BEF: minish->line[%d] = <%c>, prec = %d, prec_iss = %d, par_and_oper = %d, multi_par_o = %d, par_o = %d, multi_par_condition_close = %d, par_c = %d, par = %d\n", 
// 			i, minish->line[i], prec, prec_iss, par_and_oper, multi_par_o, par_o, multi_par_condition_close, par_c, par);
// 			if (prec_iss == PAR_CLOSE)
// 				par_and_oper += 1;
// 			// if (multi_par_condition_close == 1)
// 			// {
// 			// 	multi_par_o -= 1;
// 			// 	multi_par_condition_close = 0;
// 			// }
// 			prec = OPERATOR;
// 			prec_iss = OPERATOR;
// 			printf("OPERATOR AF: minish->line[%d] = <%c>, prec = %d, prec_iss = %d, par_and_oper = %d, multi_par_o = %d, par_o = %d, multi_par_condition_close = %d, par_c = %d, par = %d\n\n", 
// 			i, minish->line[i], prec, prec_iss, par_and_oper, multi_par_o, par_o, multi_par_condition_close, par_c, par);
// 			i++;
// 		}
// 		else
// 		{
// 			printf("OTHER : minish->line[%d] = <%c>, prec = %d, prec_iss = %d, par_and_oper = %d, multi_par_o = %d, par_o = %d, multi_par_condition_close = %d, par_c = %d, par = %d\n", 
// 			i, minish->line[i], prec, prec_iss, par_and_oper, multi_par_o, par_o, multi_par_condition_close, par_c, par);
// 			if (prec_iss == PAR_CLOSE)
// 			{
// 				line = ft_error_message(minish->line + i); //MALLOC
// 				//IF ERROR MALLOC
// 				return (err_syntax(line, 2, 1)); // a free dans la fonction error_syntax
// 			}
// 			if (cmd == 1)
// 				cmd = 2;
// 			prec = OTHER;
// 			prec_iss = OTHER;
// 			other_char += 1;
// 			printf("OTHER AF: minish->line[%d] = <%c>, prec = %d, prec_iss = %d, par_and_oper = %d, multi_par_o = %d, par_o = %d, multi_par_condition_close = %d, par_c = %d, par = %d\n\n", 
// 			i, minish->line[i], prec, prec_iss, par_and_oper, multi_par_o, par_o, multi_par_condition_close, par_c, par);
// 			i++;
// 		}
// 	}
// 	if (multi_par_o != 0 && multi_par_c == multi_par_o) //UTILE???
// 	{
// 		printf("multi_par_o %d\n", multi_par_o);
// 		return (err_syntax("", 2, 0));
// 	}
// 	// while (minish->line[i])
// 	// {
// 	// 	if (minish->line[i] == '(')
// 	// 	{
// 	// 		par_o += 1;
// 	// 		i++;
// 	// 	}
// 	// 	else if (minish->line[i] == ')')
// 	// 	{
// 	// 		if (par_o <= par_c)
// 	// 			 return (err_syntax("syntax error near unexpected token `)'\n", 2));
// 	// 		par_c += 1;
// 	// 		i++;
// 	// 	}
// 	// 	else if (minish->line[i] == '"')
// 	// 		while (minish->line[++i] && minish->line[i] != '"');
// 	// 	else if (minish->line[i] == '\'')
// 	// 		while (minish->line[++i] && minish->line[i] != '\'');
// 	// 	else
// 	// 		i++;
// 	// }
// 	if ((par_c + par_o) % 2 == 1) // POUR GERER SI PARENTHESE OUVERTE
// 	{
// 		line = readline("> ");
// 		tmp = minish->line;
// 		minish->line = ft_strjoin(minish->line, " "); // MALLOC
// 		// IF ERROR MALLOC
// 		free(tmp);
// 		tmp = minish->line;
// 		minish->line = ft_strjoin(minish->line, line); // MALLOC
// 		// IF ERROR MALLOC
// 		free(tmp);
// 		free(line);
// 		add_history(line); //voir comment supprimer derniere lg de l historique et remplacer par la nouvelle ici
// 		ft_parse_line(minish); // MALLOC
// 		// IF ERROR MALLOC
// 		ft_parse_bis(minish); // MALLOC
// 		// IF ERROR MALLOC
// 	}
// 	return (0);
// }


// int ft_parse_bis(t_msh *minish)
// {
// 	int	i;
// 	// int j;
// 	int string_b;
// 	int string_a;
// 	int par;
// 	// int d_q;
// 	// int s_q;
// 	char *line;
// 	char *tmp;
// 	int par_o;
// 	int par_c;
// 	char prec;

// 	i = 0;
// 	par = 0;
// 	string_a = 0;
// 	string_b = 0;
// 	par_o = 0;
// 	par_c = 0;
// 	prec = TO_DEFINE;
// 	while (minish->line[i])
// 	{
// 		while (minish->line[i] && ft_isspace(minish->line[i]) == 0)
// 		{
// 			if (string_b == 2)
// 				string_b = 3;
// 			i++;
// 		}
// 		if (minish->line[i] && minish->line[i] == '"')
// 		{
// 			string_a = 1;
// 			while (minish->line[++i] && minish->line[i] != '"');
// 		}
// 		else if (minish->line[i] && minish->line[i] == 39)
// 		{
// 			string_a = 1;
// 			while (minish->line[++i] && minish->line[i] != 39);
// 		}
// 		else if ((string_a == 1 || par == 0 || par_o > 1) && minish->line[i] && (minish->line[i] == '|' || minish->line[i] == '&'))
// 		{
// 			string_a = 0;
// 			prec = OPERATOR;
// 			printf("minish->line[%d] = %c string_b = %d par_c = %d\n", i, minish->line[i], string_b, par_c);
// 			if ((string_b == 3 || string_b == 4) && par_c == 0)
// 				string_b = 5;
// 			// if (string_b == 3 && par_c + 1 == par_o)
// 			string_b = 0;
// 			i++;
// 		}
// 		else if (minish->line[i] && minish->line[i] == '(')
// 		{
// 			printf("minish->line[%d] = %c prec = %d\n", i, minish->line[i], prec);
// 			if (string_a == 1 || prec == PAR_CLOSE)
// 				return (err_syntax("minishell: syntax error near unexpected token `('\n", 2));
// 			if (par > 0 && ((par_c + 1 != par_o) || par_c == 0) && (string_b == 0 || string_b == 3))
// 				string_b = 1;
// 			par += 1;
// 			par_o += 1;
// 			prec = PAR_OPEN;
// 			i++;
// 		}
// 		else if (minish->line[i] && minish->line[i] == ')')
// 		{
// 			printf("minish->line[%d] = %c prec = %d\n", i, minish->line[i], prec);
// 			if (/*par % 2 == 0 &&*/ par == 0) //attention ne gere pas les doubles parenthese donc si 2 par ouverte rentre dedans
// 				return (err_syntax("minishell: syntax error near unexpected token `)'\n", 2));
// 			if (string_a == 0)
// 				return (err_syntax("minishell: syntax error near unexpected token `)'\n", 2));
// 			if (string_b == 1 || string_b == 2 || ((string_b == 3 || string_b == 5) && par_c + 1 == par_o))
// 			{
// 				printf("string_a = %d string_b = %d par_c = %d par_o = %d\n", string_a, string_b, par_c, par_o);
// 				return (err_syntax("erreur a ne pas imprimer\n", 1)); //revoir exitstatus
// 			}
// 			if (string_b == 4)
// 				return (err_syntax("erreur exit status 1\n", 1)); //revoir exitstatus
// 			// else if (fonction pour traiter le parsing post fermeture pparenthese)
// 			par -= 1;
// 			par_c += 1;
// 			prec = PAR_CLOSE;
// 			i++;
// 		}
// 		else
// 		{
// 			string_a = 1;
// 			prec = OTHER;
// 			if (string_b == 1) // double parenthese ((
// 				string_b = 2; // double parenthese + 1 char (( a
// 			if (string_b == 3) // double parenthese + 1 char + 1 isspace (( a_
// 			{
				
// 				string_b = 4; // double parenthese + 1 char + 1 isspace + 1 char (( a a
// 			}
// 			i++;
// 		}
// 		// if (i > 0)
// 		// printf("minish->line[%d] = |%c| ; par = %d ; string_a = %d\n", i - 1, minish->line[i - 1], par, string_a);
// 	}
// 	if (par % 2 == 1) // POUR GERER SI PARENTHESE OUVERTE
// 	{
// 		line = readline("> ");
// 		tmp = minish->line;
// 		minish->line = ft_strjoin(minish->line, " "); // MALLOC
// 		// IF ERROR MALLOC
// 		free(tmp);
// 		tmp = minish->line;
// 		minish->line = ft_strjoin(minish->line, line); // MALLOC
// 		// IF ERROR MALLOC
// 		free(tmp);
// 		free(line);
// 		add_history(line); //voir comment supprimer derniere lg de l historique et remplacer par la nouvelle ici
// 		ft_parse_line(minish); // MALLOC
// 		// IF ERROR MALLOC
// 		ft_parse_bis(minish); // MALLOC
// 		// IF ERROR MALLOC
// 	}
// 	return (0);
// }
