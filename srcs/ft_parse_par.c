/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_par.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 12:45:33 by galambey          #+#    #+#             */
/*   Updated: 2024/01/15 11:02:03 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

int	ft_parse_par(t_msh *msh)
{
	t_par		p;
	int			i;
	int			res;
	t_storage	function;

	ft_init_var(&p, &i);
	while (msh->line[i])
	{
		res = ft_parse_parenthesis(msh, &p, &function, &i);
		if (res == 1)
			return (1);
		if (res == 2)
			return (0);
	}
	if (p.par_c != p.par_o && p.prec_iss != OP)
		return (msh->status = 2,
			e_syntx("syntax error parenthesis expected closed\n"));
	return (0);
}
