/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_par_storage.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 12:45:33 by galambey          #+#    #+#             */
/*   Updated: 2024/01/15 11:01:59 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_par_open(t_msh *msh, t_par *p, int *i)
{
	if (p->prec_iss == PAR_CLOSE || ((p->prec_iss == OTHER
				|| p->prec_iss == CHEVRON)
			&& (p->chev == 1 || p->multi_cmd > 1)))
		return (msh->status = 2,
			e_syntx("minishell: syntax error near unexpected token `('\n"));
	if (p->prec_iss == OTHER || p->prec_iss == CHEVRON)
	{
		while (ft_isspace(msh->line[++(*i)]) == 0)
			;
		return (msh->status = 2, e_syntx(ft_error_message(msh->line + *i,
					msh)));
	}
	p->prec_iss = PAR_OPEN;
	p->prec = PAR_OPEN;
	p->par_o += 1;
	(*i)++;
	return (0);
}

static int	ft_par_close(t_msh *msh, t_par *p, int *i)
{
	(void)msh;
	if (p->par_o <= p->par_c || (p->prec_iss == PAR_OPEN
			|| p->prec_iss == CHEVRON))
		return (msh->status = 2,
			e_syntx("minishell: syntax error near unexpected token `)'\n"));
	p->prec_iss = PAR_CLOSE;
	p->prec = PAR_CLOSE;
	p->par_c += 1;
	(*i)++;
	return (0);
}

/*
La premiere erreur de syntax trouvee est "prioritaire", donc :
Retour = 0 => 1ere erreur de syntaxe trouvee sur les operateurs qui est gere
	par la suite dans ft_parse_error_token (apres tokenisation)
*/
static int	ft_operator(t_msh *msh, t_par *p, int *i)
{
	if (p->prec_iss == TO_DEFINE || ((p->prec_iss == OP
				|| p->prec_iss == CHEVRON) && p->prec == ISS)
		|| ft_count_char(msh->line + *i) == 1)
		return (0);
	p->prec_iss = OP;
	p->prec = OP;
	p->chev = 0;
	p->multi_cmd = 0;
	(*i)++;
	return (1);
}

/*
La premiere erreur de syntax trouvee est "prioritaire", donc :
Retour = 0 => 1ere erreur de syntaxe trouvee sur les operateurs qui est gere
	par la suite dans ft_parse_error_token (apres tokenisation)
*/
static int	ft_chevron(t_msh *msh, t_par *p, int *i)
{
	p->chev = 1;
	if ((p->prec_iss == CHEVRON && p->prec == ISS)
		|| ft_count_char(msh->line + *i) == 1)
		return (0);
	p->prec_iss = CHEVRON;
	p->prec = CHEVRON;
	(*i)++;
	return (1);
}

t_storage	ft_storage(int c)
{
	t_storage	*function;

	function = (t_storage[128]){
	['('] = ft_par_open,
	[')'] = ft_par_close,
	['&'] = ft_operator,
	['|'] = ft_operator,
	['>'] = ft_chevron,
	['<'] = ft_chevron};
	return (function[c]);
}
