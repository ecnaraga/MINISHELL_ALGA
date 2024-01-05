/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_str_bis.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 11:43:13 by athiebau          #+#    #+#             */
/*   Updated: 2024/01/05 18:08:22 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_operator(char c)
{
	if (c == '(' || c == ')' || c == '>' || c == '<' || c == '&' || c == '|')
		return (1);
	else
		return (0);
}

/*
Verifie que les " et ' sont bien apparies
	- quote entre quote 🗸
	- pouvoir ajouter plusieurs quote entre 2 mm quote : "'bra'vo" 🗸
*/
static void	ft_quote_order_bis(char *str, char c, int *i, int *nb_q)
{
	*i += 1;
	*nb_q += 1;
	while (str[*i] && str[*i] != c)
		*i += 1;
	if (str[*i] && str[*i] == c)
	{
		*nb_q += 1;
	}
	if (str[*i] /* && str[*i + 1] */)
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
		ft_putstr_fd("minishell: les quote ne sont pas appariees\n", 2);
		return (2);
	}
	return (0);
}

int	ft_parse_line(t_msh *msh)
{
	if (ft_quote_order(msh->line) == 2)
		return (1);
	msh->line = add_spaces(msh, msh->line); // SI MALLOC KO ON QUITTE DANS ADD_SPACES
	msh->line = ft_parse_bis_bis(msh, msh->line);
	// if (!msh->line)
	// 	return (1);
	printf("Sortie ft_parse_line : _%s_\n", msh->line);
	return (0);
}
