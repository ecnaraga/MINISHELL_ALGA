/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garance <garance@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 16:13:33 by galambey          #+#    #+#             */
/*   Updated: 2023/11/12 08:22:07 by garance          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
Incremente les double quotes(d_q) si pas entre single quotes (s_q)
Incremente les single quotes(s_q) si pas entre double quotes (d_q)
Ps : 39 = single quote
*/
void	ft_inc_quote(char c, int *d_q, int *s_q)
{
	if (*s_q % 2 == 0 && c == '"')
		*d_q += 1;
	else if (*d_q % 2 == 0 && c == 39)
		*s_q += 1;
}

/*
Test si le char envoye est un isspace
*/
int	ft_is_isspace(char c)
{
	if (c == ' ' || (c >= '\t' && c <= '\r'))
		return (0);
	else
		return (1);
}

/*
Test si le char envoye est un isspace
*/
int	ft_isspace(char c)
{
	if (c == ' ' || (c >= '\t' && c <= '\r'))
		return (0);
	else
		return (1);
}
