/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 11:31:49 by galambey          #+#    #+#             */
/*   Updated: 2024/01/11 14:07:34 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_expand_hdoc(t_msh *msh, char *line, int fd, t_expand *e)
{
	e->len = valide_expand(line + e->i + 1);
	if (e->len == 0)
	{
		e->tmp = mcgic(mlcp(ft_strdup(line + e->i + 1), 0), ADD, NO_ENV, msh);
		if (msh->status == 255)
			ft_exit_bis(msh, 0, fd, -1);
	}
	else
	{
		e->tmp = mcgic(mlcp(ft_substr(line, e->i + 1, (size_t)e->len), 0), ADD,
				NO_ENV, msh);
		if (msh->status == 255)
			ft_exit_bis(msh, 0, fd, -1);
	}
	e->tmp = get_value(msh, msh->env, e, HDOC);
	if (msh->status == 255)
		ft_exit_bis(msh, 0, fd, -1);
	if (ft_strcmp(e->tmp, " ") != 0)
		ft_putstr_fd(e->tmp, fd);
	e->i += (e->len + 1);
}

/*
Create a copy of the list of heredoc
If there is no list of heredoc, return sub_heredoc = NULL
If error MLC, quit the actual process
*/
t_env	*ft_copy_heredoc(t_msh *msh, t_env *hdoc, int sub)
{
	t_env	*head;
	t_env	*sub_hdoc;
	t_env	*new;
	char	*tmp;

	head = hdoc;
	sub_hdoc = NULL;
	while (hdoc)
	{
		tmp = mcgic(mlcp(ft_strdup(hdoc->name), 1), ADD, NO_ENV, msh);
		if (!tmp)
			ft_exit_bis(msh, sub, -1, -1);
		new = ft_lst_new_heredoc(msh, tmp, sub);
		new->content = mcgic(mlcp(ft_strdup(hdoc->content), 1), ADD, NO_ENV,
				msh);
		if (!new->content)
			ft_exit_bis(msh, sub, -1, -1);
		new->read = hdoc->read;
		ft_lstadd_back_env(&sub_hdoc, new);
		hdoc = hdoc->next;
	}
	hdoc = head;
	return (sub_hdoc);
}
