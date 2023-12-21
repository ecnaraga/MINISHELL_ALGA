/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 10:16:25 by galambey          #+#    #+#             */
/*   Updated: 2023/12/21 12:14:54 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_env	*ft_lst_new_heredoc(t_msh *msh, char *lim, int sub)
{
	t_env	*temp;

	temp = NULL;
	temp = mlcgic(mlcp(NULL, sizeof(t_env)), MALLOC, NO_ENV, msh);
	// temp = ft_magic_malloc(MALLOC, sizeof(t_env), NULL, NO_ENV);
	if (temp == NULL)
		ft_exit_bis(msh, sub, -1, -1); // SI MALLOC KO ON QUITTE
	temp->name = lim;
	temp->content = NULL;
	temp->read = 0;
	temp->next = NULL;
	return (temp);
}
