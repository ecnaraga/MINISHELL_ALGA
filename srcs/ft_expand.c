/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 10:50:24 by galambey          #+#    #+#             */
/*   Updated: 2024/01/15 12:01:08 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*ft_expand_exitcode(t_msh *msh, char *cmd, t_expand *e)
{
	e->tmp = mcgic(mlcp(ft_itoa(msh->previous_status), 1), ADD, NO_ENV, msh);
	if (msh->status == 255)
		return (NULL);
	e->tmp2 = cmd;
	cmd = mcgic(mlcp(ft_strjoin(cmd, e->tmp), 1), ADD, NO_ENV, msh);
	if (msh->status == 255)
		return (NULL);
	if (e->tmp2)
		mcgic(mlcp(e->tmp2, 0), FREE, NO_ENV, msh);
	mcgic(mlcp(e->tmp, 0), FREE, NO_ENV, msh);
	(e->i) += 2;
	(e->j)++;
	return (cmd);
}

static char	*ft_expand_env(t_msh *msh, char *cmd, int rule, t_expand *e)
{
	e->len = valide_expand(e->tmp);
	if (e->len == 0)
	{
		cmd = ft_do_expand(msh, e, cmd, rule);
		if (msh->status == 255)
			return (NULL);
		e->i += msh->av->type[e->j].len_variable;
	}
	else if (e->len == -1)
	{
		msh->av->type[e->j].expnd = 2;
		return (cmd);
	}
	else
	{
		cmd = ft_expand_valid_var(msh, cmd, rule, e);
		if (msh->status == 255)
			return (NULL);
		e->i += (e->len + 1);
	}
	e->j++;
	return (cmd);
}

static char	*ft_expand_var(t_msh *msh, char *cmd, int rule, t_expand *e)
{
	if (msh->av->data[e->i + 1] == '?')
		cmd = ft_expand_exitcode(msh, cmd, e);
	else
	{
		e->tmp = mcgic(mlcp(ft_substr(msh->av->data, e->i + 1,
						msh->av->type[e->j].len_variable - 1), 1), ADD, NO_ENV,
				msh);
		if (msh->status == 255)
			return (NULL);
		cmd = ft_expand_env(msh, cmd, rule, e);
	}
	return (cmd);
}

static char	*ft_handle_dollar(t_msh *msh, char *cmd, int rule, t_expand *e)
{
	if (msh->av->type[e->j].expnd == MULTI_DOLLAR)
		cmd = ft_add_char(msh, cmd, e, &e->j);
	else if ((msh->av->type[e->j].expnd == 1
			&& msh->av->type[e->j].len_variable == 1)
		|| msh->av->type[e->j].expnd == 2)
		cmd = ft_add_char(msh, cmd, e, &e->j);
	else if (msh->av->type[e->j].expnd == 1
		&& msh->av->type[e->j].len_variable == 0)
	{
		e->i += 1;
		e->j += 1;
	}
	else if (msh->av->type[e->j].expnd == 1
		&& msh->av->type[e->j].len_variable > 1)
	{
		cmd = ft_expand_var(msh, cmd, rule, e);
		if (rule == CMD)
			msh->ambiguous = 2;
	}
	return (cmd);
}

/*
Pour savoir s'il comment traiter le dollar, se referer aux instructions dans le
	tableau de structure strs.type (plus de precisions dans ft_strlcpy_msh)
1. Si (expnd = EXPAND = 1 et len variable = 1) ou si (expnd = MULTIDOLLAR = 3)
	=> afficher 1 $
2. Si (expnd = NO_EXPAND = 2) => afficher les char tel quel et ne pas expand
3. Si (expnd = EXPAND = 1 et len_variable = 0) => ne pas afficher le dollar
4. Si (expnd = EXPAND = 1 et len_variable > 1) => expand : remplacer la
	variable d environnement par sa valeur dans l 'env
	exemple : si expnd = 1 && len_variable = 5 => remplacer $ + 4 char
		(ex : $USER) par la variable d environnement dans env

REGLES A SUIVRE :
- Si plusieurs dollars d affilee : afficher 1 dollar
- Si 1 seul dollar suivi de quote : ne pas afficher le dollar
- Si un seul dollar entre des quotes single ou double : afficher 1 dollar
- Si dollar pas entre quote : expand
- Si dollar entre double_quote : expand
- Si dollar entre de single_quote : ne pas expand
*/
char	*ft_expand(t_msh *msh, char *cmd, int rule)
{
	t_expand	e;

	e.i = 0;
	e.j = 0;
	cmd = NULL;
	msh->first_d = 0;
	while (msh->av->data[e.i])
	{
		if (msh->av->data[e.i] == '$')
			cmd = ft_handle_dollar(msh, cmd, rule, &e);
		else
			cmd = ft_add_char(msh, cmd, &e, NULL);
		if (msh->status == 255)
			return (NULL);
	}
	return (cmd);
}
