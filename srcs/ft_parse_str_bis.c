/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_str_bis.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 11:43:13 by athiebau          #+#    #+#             */
/*   Updated: 2023/11/23 11:47:49 by athiebau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

int	ft_quote_order(char *str)
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
