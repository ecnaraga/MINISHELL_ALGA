/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_error_token.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 13:39:24 by galambey          #+#    #+#             */
/*   Updated: 2024/01/15 11:01:56 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_check_chev(t_msh *msh)
{
	if (msh->av->token == CHEVRON && (ft_strlen(msh->av->data) > 2
			|| ft_same_char(msh->av->data) == 1))
		return (msh->status = 2, e_syntx(e_chev(msh->av->data, 1, msh)));
	if (msh->av->next->data && msh->av->token == CHEVRON
		&& msh->av->next->token == CHEVRON)
		return (msh->status = 2, e_syntx(e_chev(msh->av->next->data, 0, msh)));
	if (msh->av->next->data && msh->av->token == CHEVRON
		&& (msh->av->next->token == OP || msh->av->next->token == PIPE))
		return (msh->status = 2, e_syntx(
				ft_err_op(msh->av->next->data, 0, msh)));
	return (0);
}

static int	ft_check_op(t_msh *msh)
{
	if (msh->av->token == OP && (ft_strlen(msh->av->data) > 2
			|| ft_same_char(msh->av->data) == 1))
		return (msh->status = 2, e_syntx(ft_err_op(msh->av->data, 1, msh)));
	if (msh->av->next->data
		&& (msh->av->token == OP || msh->av->token == PIPE)
		&& (msh->av->next->token == OP || msh->av->next->token == PIPE))
		return (msh->status = 2, e_syntx(
				ft_err_op(msh->av->next->data, 0, msh)));
	return (0);
}

static int	ft_handle_chev(t_msh *msh, t_split *prev, t_split **head)
{
	t_split	*tmp;

	if (msh->ac == 1)
		return (msh->status = 2, e_syntx(e_chev(msh->av->data, 1, msh)));
	else if (ft_check_chev(msh) == 1)
		return (1);
	else if (prev == NULL)
	{
		tmp = msh->av->next;
		ft_lstdelone_split(msh, msh->av, del_two);
		msh->av = tmp;
		*head = msh->av;
	}
	else
	{
		prev->next = msh->av->next;
		ft_lstdelone_split(msh, msh->av, del_two);
		msh->av = prev->next;
	}
	return (0);
}

static int	ft_last_token(t_msh *msh, t_split *head)
{
	if (msh->av->token == CHEVRON)
		return (msh->status = 2, e_syntx(e_chev(msh->av->data, 1, msh)));
	if (msh->av->token == OP || msh->av->token == PIPE)
		return (msh->status = 2, e_syntx(ft_err_op(msh->av->data, 0, msh)));
	msh->av = head;
	return (0);
}

int	ft_parse_error_token(t_msh *msh)
{
	t_split	*head;
	t_split	*tmp;

	head = msh->av;
	tmp = NULL;
	if (msh->av->token == OP || msh->av->token == PIPE)
		return (msh->status = 2, e_syntx(ft_err_op(msh->av->data, 0, msh)));
	while (msh->av->next)
	{
		if (msh->av->token == CHEVRON)
		{
			if (ft_handle_chev(msh, tmp, &head) == 1)
				return (1);
		}
		else
		{
			if (ft_check_op(msh) == 1)
				return (1);
			tmp = msh->av;
			msh->av = msh->av->next;
		}
	}
	return (ft_last_token(msh, head));
}
