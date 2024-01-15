/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_token.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 16:54:09 by galambey          #+#    #+#             */
/*   Updated: 2024/01/15 11:02:11 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_token_chev_right(t_msh *msh)
{
	msh->av->token = CHEVRON;
	if (msh->av->next && msh->av->data[1] && msh->av->data[1] == '>')
		msh->av->next->token = OUTFILE_NO_TRUNC;
	else if (msh->av->next)
		msh->av->next->token = OUTFILE_TRUNC;
}

static void	ft_token_chev_left(t_msh *msh)
{
	msh->av->token = CHEVRON;
	if (msh->av->next && msh->av->data[1] && msh->av->data[1] == '<')
		msh->av->next->token = HDOC;
	else if (msh->av->next)
		msh->av->next->token = INFILE;
}

static void	ft_set_token(t_msh *msh)
{
	if (!msh->av->quote && msh->av->data[0] == '>')
		ft_token_chev_right(msh);
	else if (!msh->av->quote && msh->av->data[0] == '<')
		ft_token_chev_left(msh);
	else if (!msh->av->quote && msh->av->data[0] == '|' && !msh->av->data[1])
		msh->av->token = PIPE;
	else if (!msh->av->quote && (msh->av->data[0] == '&'
			|| msh->av->data[0] == '|'))
		msh->av->token = OP;
	else if (!msh->av->quote && msh->av->data[0] == '(')
		msh->av->token = PAR_OPEN;
	else if (!msh->av->quote && msh->av->data[0] == ')')
		msh->av->token = PAR_CLOSE;
	else if (msh->av->token == TO_DEFINE)
		msh->av->token = CMD;
}

void	ft_token(t_msh *msh)
{
	t_split	*head;

	head = msh->av;
	while (msh->av)
	{
		if (!msh->av->data[0])
		{
			msh->av = msh->av->next;
			continue ;
		}
		ft_set_token(msh);
		msh->av = msh->av->next;
	}
	msh->av = head;
}
