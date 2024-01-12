/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_par_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 12:45:33 by galambey          #+#    #+#             */
/*   Updated: 2024/01/10 13:36:00 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_count_char(char *str)
{
	int		i;
	int		count;
	char	c;

	i = -1;
	count = 0;
	c = str[0];
	while (str[++i] == c)
		count++;
	if (!str[i] || (str[i] != c && ft_isspace(str[i]) != 0)
		|| (ck_char(">&|", c) != 0 && count > 2) || (count > 3 && c == '<'))
		return (1);
	return (0);
}

/*
Skip all the char between double or single quotes
*/
void	ft_pass_quote(t_msh *msh, t_par *p, int *i, char c)
{
	while (msh->line[++(*i)] && msh->line[*i] != c)
		;
	(*i)++;
	if (p->prec == ISS && (p->prec_iss == OTHER || p->prec_iss == CHEVRON))
		p->multi_cmd = 2;
	p->prec_iss = OTHER;
	p->prec = OTHER;
}

int	ft_other_char(t_msh *msh, t_par *p, int *i)
{
	if (p->prec_iss == PAR_CLOSE && msh->line[*i] != '>'
		&& msh->line[*i] != '<')
		return (msh->status = 2,
			e_syntx(ft_error_message(msh->line + *i, msh)));
	p->prec_iss = OTHER;
	if (p->prec != OTHER)
		p->multi_cmd += 1;
	p->prec = OTHER;
	(*i)++;
	return (0);
}
