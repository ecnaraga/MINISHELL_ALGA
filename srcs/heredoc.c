/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 11:31:49 by galambey          #+#    #+#             */
/*   Updated: 2024/01/10 18:17:28 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
Create a random name for the here_doc and at it at the end of the linked list
	(after the limiter added)
*/
static void	create_here_doc(t_msh *msh, t_split *av, t_env **hdoc, t_env **head)
{
	t_env	*new;
	char	*tmp;

	tmp = mcgic(mlcp(ft_strdup(av->data), 1), ADD, NO_ENV, msh);
	if (!tmp)
		ft_exit_bis(msh, 0, -1, -1);
	new = ft_lst_new_heredoc(msh, tmp, 0);
	while (1)
	{
		new->content = mcgic(mlcp(ft_random_filename("/tmp/", 8), 1), ADD,
				NO_ENV, msh);
		if (!new->content)
			ft_exit_bis(msh, 0, -1, -1);
		if (ft_check_unique_name(new->content, hdoc) == 0)
			break ;
		else
			mcgic(mlcp(new->content, 0), FREE, NO_ENV, msh);
	}
	ft_lstadd_back_env(hdoc, new);
	*head = *hdoc;
	while ((*hdoc)->next)
		*hdoc = (*hdoc)->next;
}

static void	ft_write_hdoc(t_msh *msh, char *line, int fd)
{
	t_expand	e;

	e.i = 0;
	while (line[e.i])
	{
		if (line[e.i] != '$')
			ft_putchar_fd(line[e.i++], fd);
		else if (!line[e.i + 1] || (ft_isalpha(line[e.i + 1]) == 0 && line[e.i
					+ 1] != '$'))
			ft_putchar_fd(line[e.i++], fd);
		else if (line[e.i + 1] == '$')
		{
			ft_putchar_fd(line[e.i], fd);
			e.i += 2;
		}
		else
		{
			ft_expand_hdoc(msh, line, fd, &e);
			if (e.len == 0)
				break ;
		}
	}
	(write(fd, "\n", 1), mcgic(mlcp(line, 0), FREE, NO_ENV, msh));
}

/*
Function called in a loop in the parent function
Copy into the file create for an heredoc, the stdin line entered by the user. 
*/
static int	ft_recover_prompt(t_msh *msh, int fd, char *lim)
{
	char	*line;

	if (ft_signal_handler_msh_hdoc(msh) == 255)
		ft_exit_bis(msh, 0, fd, -1);
	line = NULL;
	line = readline("> ");
	if (g_sign == 1)
		return (ft_sigint_hdoc(msh, fd));
	if (!line)
		return (ft_handle_ctrld_hdoc(msh, fd, lim));
	line = mcgic(mlcp(line, 1), ADD, NO_ENV, msh);
	if (msh->status == 255)
		ft_exit_bis(msh, 0, fd, -1);
	if (ft_strcmp(msh->av->data, line) == 0)
		return (mcgic(mlcp(line, 0), FREE, NO_ENV, msh), 1);
	ft_write_hdoc(msh, line, fd);
	return (0);
}

/*
Function called in a loop in the parent function
For each limiter encountered, create a heredoc and recover the stdin to copy it
	into the file created
*/
static int	ft_prompt(t_msh *msh, t_split *av, t_env **hdoc)
{
	int		fd;
	char	*lim;
	t_env	*head;
	int		err;

	create_here_doc(msh, av, hdoc, &head);
	fd = open((*hdoc)->content, O_CREAT | O_APPEND | O_WRONLY, 0744);
	if (fd == -1)
		(perror((*hdoc)->content), msh->status = 1, ft_exit_bis(msh, 0, -1,
				-1));
	lim = mcgic(mlcp(ft_strdup(av->data), 1), ADD, NO_ENV, msh);
	if (!lim)
		ft_exit_bis(msh, 0, fd, -1);
	while (1)
	{
		err = ft_recover_prompt(msh, fd, lim);
		if (err == 1)
			break ;
		if (err == 130)
			return (130);
	}
	if (ft_signal_handler_msh(msh) == 255)
		ft_exit_bis(msh, 0, fd, -1);
	*hdoc = head;
	return (close(fd), mcgic(mlcp(lim, 0), FREE, NO_ENV, msh), 0);
}

/*
In a loop, create all the here_doc and storage in a linked list the limiter
	followed by the name of the here_doc created
*/
int	ft_heredoc(t_msh *msh)
{
	t_split	*head;

	head = msh->av;
	msh->p.hdoc = NULL;
	while (msh->av)
	{
		if (msh->av->token == HDOC)
		{
			if (ft_prompt(msh, msh->av, &msh->p.hdoc) == 130)
				return (130);
		}
		msh->av = msh->av->next;
	}
	msh->av = head;
	return (0);
}
