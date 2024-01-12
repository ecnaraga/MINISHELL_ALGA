/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils_bis.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 18:11:53 by galambey          #+#    #+#             */
/*   Updated: 2024/01/10 18:13:26 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_env	*ft_lst_new_heredoc(t_msh *msh, char *lim, int sub)
{
	t_env	*temp;

	temp = NULL;
	temp = mcgic(mlcp(NULL, sizeof(t_env)), MLC, NO_ENV, msh);
	if (temp == NULL)
		ft_exit_bis(msh, sub, -1, -1);
	temp->name = lim;
	temp->content = NULL;
	temp->read = 0;
	temp->next = NULL;
	return (temp);
}

int	ft_sigint_hdoc(t_msh *msh, int fd)
{
	msh->status = 130;
	if (open("/dev/stdout", O_RDONLY) == -1)
	{
		msh->status = 1;
		ft_exit_bis(msh, 0, fd, -1);
	}
	mcgic(NULL, FLUSH, NO_ENV, msh);
	return (close(fd), 130);
}

int	ft_handle_ctrld_hdoc(t_msh *msh, int fd, char *lim)
{
	char	*str;
	char	*tmp;

	tmp = ft_strjoin(
			"bash: warning: here-document delimited by end-of-file (wanted `",
			lim);
	str = mcgic(mlcp(tmp, 1), ADD, NO_ENV, msh);
	if (msh->status == 255)
		ft_exit_bis(msh, 0, fd, -1);
	tmp = mcgic(mlcp(ft_strjoin(str, "')\n"), 1), ADD, NO_ENV, msh);
	if (msh->status == 255)
		ft_exit_bis(msh, 0, fd, -1);
	write(2, tmp, ft_strlen(tmp));
	mcgic(mlcp(str, 0), FREE, NO_ENV, msh);
	mcgic(mlcp(tmp, 0), FREE, NO_ENV, msh);
	return (close(fd), 1);
}

int	ft_check_unique_name(char *tmp, t_env **hdoc)
{
	t_env	*head;

	head = *hdoc;
	while (*hdoc)
	{
		if (ft_strcmp((*hdoc)->content, tmp) == 0)
			return (1);
		*hdoc = (*hdoc)->next;
	}
	*hdoc = head;
	return (0);
}

void	ft_unlink_heredoc(t_env *hdoc)
{
	int	i;

	i = 0;
	while (hdoc)
	{
		unlink(hdoc->content);
		hdoc = hdoc->next;
		i++;
	}
}
