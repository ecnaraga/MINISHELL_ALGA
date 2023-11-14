/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_token.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 16:54:09 by galambey          #+#    #+#             */
/*   Updated: 2023/11/14 15:22:02 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h"

// INCLURE l'expand

void	ft_token(t_msh *msh)
{
	int	i;

	i = -1;
	while (msh->av[++i].data )
	{
		if (!msh->av[i].data[0] || msh->av[i].token != TO_DEFINE)
			continue ;
		if (msh->av[i].data[0] == '>' && msh->av[i + 1].data) //Attention au cas ">(" et "> ("
		{
			msh->av[i].token = CHEVRON;
			if (msh->av[i].data[1] && msh->av[i].data[1] == '>')
				msh->av[i + 1].token = OUTFILE_NO_TRUNC;
			// else if (msh->av[i].data[1] && msh->av[i].data[1] == '(')
			else
				msh->av[i + 1].token = OUTFILE_TRUNC;
		}
		else if (msh->av[i].data[0] == '<' && msh->av[i + 1].data) //Attention au cas "<(" et  "< ("
		{
			msh->av[i].token = CHEVRON;
			if (msh->av[i].data[1] && msh->av[i].data[1] == '<')
				msh->av[i + 1].token = HERE_DOC;
			// else if (msh->av[i].data[1] && msh->av[i].data[1] == '(')
			else
				msh->av[i + 1].token = INFILE;
		}
		else if (ft_strcmp(msh->av[i].data, "&&") == 0 || ft_strcmp(msh->av[i].data, "||") == 0 || ft_strcmp(msh->av[i].data, "|") == 0)
			msh->av[i].token = OPERATOR;
		else if (msh->av[i].data[0] == '(')
			msh->av[i].token = PAR_OPEN;
		else if (msh->av[i].data[0] == ')')
			msh->av[i].token = PAR_CLOSE;
		else
			msh->av[i].token = CMD;
	}
}