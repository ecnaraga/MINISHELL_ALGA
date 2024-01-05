/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 11:31:49 by galambey          #+#    #+#             */
/*   Updated: 2024/01/05 12:14:05 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
TO DO : BIEN CHECKER LES FREE ET UNLINK
*/

int	ft_check_unique_name(char *tmp, t_env **hdoc)
{
	t_env *head;

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

/*
Create a random name for the here_doc and at it at the end of the linked list
	(after the limiter added)
*/
void	create_here_doc(t_msh *msh, t_split *av, t_env **hdoc, t_env **head)
{
	t_env	*new;
	char *tmp;
	
	/*Add the limiter in the linked list*/
	tmp = mcgic(mlcp(ft_strdup(av->data), 1), ADD, NO_ENV, msh);
	if (!tmp)
		ft_exit_bis(msh, 0, -1, -1); // IF MALLOC KO ON QUITTE 
	new = ft_lst_new_heredoc(msh, tmp, 0); // IF MALLOC KO ON QUITTE A L INTERIEUR
	/* Create and add the name of the here_doc in the linked list*/
	while (1)
	{
		new->content = mcgic(mlcp(ft_random_filename("/tmp/", 8), 1), ADD, NO_ENV, msh);
		if (!new->content)
			ft_exit_bis(msh, 0, -1, -1); // IF MALLOC KO ON QUITTE 
		if (ft_check_unique_name(new->content, hdoc) == 0)
			break;
		else
			mcgic(mlcp(new->content, 0), FREE, NO_ENV, msh);
	}
	ft_lstadd_back_env(hdoc, new);
	*head = *hdoc;
	while ((*hdoc)->next)
		*hdoc = (*hdoc)->next;
}

void ft_expand_hdoc(t_msh *msh, char *line, int fd, t_expand *e)
{
	e->len = valide_expand(line + e->i + 1);
	if (e->len == 0)
	{
		e->tmp = mcgic(mlcp(ft_strdup(line + e->i + 1), 0), ADD, NO_ENV, msh); // OK PROTEGE
		if (msh->status == 255)
			ft_exit_bis(msh, 0, fd, -1);
	}
	else
	{
		e->tmp = mcgic(mlcp(ft_substr(line, e->i + 1, (size_t)e->len), 0), ADD, NO_ENV, msh); // OK PROTEGE
		if (msh->status == 255)
			ft_exit_bis(msh, 0, fd, -1);
	}
	e->tmp = get_value(msh, msh->env, e->tmp, HDOC); // OK PROTEGE
	if (msh->status == 255)
		ft_exit_bis(msh, 0, fd, -1);
	if (ft_strcmp(e->tmp, " ") != 0)
		ft_putstr_fd(e->tmp, fd);
	e->i += (e->len + 1);
}

void	ft_write_hdoc(t_msh *msh, char *line, int fd)
{
	t_expand	e;

	e.i = 0;
	while (line[e.i])
	{
		if (line[e.i] != '$')
			ft_putchar_fd(line[e.i++], fd);
		else if (!line[e.i + 1] || (ft_isalpha(line[e.i + 1]) == 0 && line[e.i + 1] != '$'))
			ft_putchar_fd(line[e.i++], fd);
		else if (line[e.i + 1] == '$')
		{
			ft_putchar_fd(line[e.i], fd);
			e.i += 2 ;
		}
		else
		{
			ft_expand_hdoc(msh, line, fd, &e); // PROTEGER ON QUITTE DASN FT_EXPAND_HDOC SI MALLOC KO
			if (e.len == 0)
				break;
		}
	}
	(write(fd, "\n", 1), mcgic(mlcp(line, 0), FREE, NO_ENV, msh));
}

int	ft_sigint_hdoc(t_msh *msh, int fd)
{
	msh->status = 130;
	if (open("/dev/stdout", O_RDONLY) == -1)
		(msh->status = 1, ft_exit_bis(msh, 0, fd, -1)); // SI ERREUR OPEN , status == 1 + ON QUITTE
	mcgic(NULL, FLUSH, NO_ENV, msh);
	return (close(fd), 130); // SI CTRL + C OK GERE
}

int	ft_handle_ctrld_hdoc(t_msh *msh, int fd, char *lim)
{
	char *str;
	char *tmp;
	
	str = mcgic(mlcp(ft_strjoin("bash: warning: here-document delimited by end-of-file (wanted `", lim), 1), ADD, NO_ENV, msh);
	if (msh->status == 255)
		ft_exit_bis(msh, 0, fd, -1); // SI ERREUR MALLOC > ON QUITTE
	tmp = mcgic(mlcp(ft_strjoin(str, "')\n"), 1), ADD, NO_ENV, msh);
	if (msh->status == 255)
		ft_exit_bis(msh, 0, fd, -1); // SI ERREUR MALLOC GNL > ON QUITTE
	write(2, tmp, ft_strlen(tmp));
	mcgic(mlcp(str, 0), FREE, NO_ENV, msh);
	mcgic(mlcp(tmp, 0), FREE, NO_ENV, msh);
	return (close(fd), 1);
}

/*
Function called in a loop in the parent function
Copy into the file create for an heredoc, the stdin line entered by the user. 
*/
static int	ft_recover_prompt(t_msh *msh, int fd, char *lim)
{
	char	*line;

	ft_signal_handler_msh_ter();
	line = NULL;
	line = readline("> ");
	if (sign == 1)
		return (ft_sigint_hdoc(msh, fd));
	if (!line) // SI SI CTRLD DANS READLINE
		return (ft_handle_ctrld_hdoc(msh, fd, lim));
	line = mcgic(mlcp(line, 1), ADD, NO_ENV, msh);
	if (msh->status == 255)
		(close(fd), ft_exit_bis(msh, 0, -1, -1)); // SI MALLOC KO ON QUITTE
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
int	ft_prompt(t_msh *msh, t_split *av, t_env **hdoc)
{
	int		fd;
	char	*lim;
	t_env *head;
	int err ;

	create_here_doc(msh, av, hdoc, &head); // IF MALLOC KO ON QUITTE A L'INTERIEUR 
	fd = open((*hdoc)->content, O_CREAT | O_APPEND | O_WRONLY, 0744);
	if (fd == -1)
		(perror((*hdoc)->content), msh->status = 1, ft_exit_bis(msh, 0, -1, -1)); // IF ERREUR OPEN -> status = 1 + ON QUITTE
	lim = mcgic(mlcp(ft_strdup(av->data), 1), ADD, NO_ENV, msh);
	if (!lim)
		ft_exit_bis(msh, 0, fd, -1); // IF MALLOC KO ON QUITTE
	while (1)
	{
		err = ft_recover_prompt(msh, fd, lim); // IF MALLOC KO OU ERREUR OPEN ON QUITTE A L INTERIEUR
		if (err == 1) // LIM RENCONTRE
			break ;
		if (err == 130) // SI CTRL + C OK GERE
			return (130);
	}
	ft_signal_handler_msh();
	*hdoc = head;
 	(close(fd), mcgic(mlcp(lim, 0), FREE, NO_ENV, msh));
	return (0);
}

/*
Create a copy of the list of heredoc
If there is no list of heredoc, return sub_heredoc = NULL
If error MLC, quit the actual process
*/
t_env	*ft_copy_heredoc(t_msh *msh, t_env *hdoc, int sub)
{
	t_env *head;
	t_env *sub_hdoc;
	t_env *new;
	char *tmp;

	head = hdoc;
	sub_hdoc = NULL;
	while (hdoc)
	{
		tmp = mcgic(mlcp(ft_strdup(hdoc->name), 1), ADD, NO_ENV, msh);
		if (!tmp)
			ft_exit_bis(msh, sub, -1, -1); // SI MALLOC KO ON QUITTE
		new = ft_lst_new_heredoc(msh, tmp, sub); // SI MALLOC KO ON QUITTE DANS F_LST_NEW_HEREDOC 
		new->content = mcgic(mlcp(ft_strdup(hdoc->content), 1), ADD, NO_ENV, msh);
		if (!new->content)
			ft_exit_bis(msh, sub, -1, -1); // SI MALLOC KO ON QUITTE
		new->read = hdoc->read;
		ft_lstadd_back_env(&sub_hdoc, new);
		hdoc = hdoc->next;
	}
	hdoc = head;
	return (sub_hdoc);
}

/*
In a loop, create all the here_doc and storage in a linked list the limiter
	followed by the name of the here_doc created
*/
int	ft_heredoc(t_msh *msh)
{
	t_split *head;
	
	head = msh->av;
	msh->p.hdoc = NULL;
	while (msh->av)
	{
		if (msh->av->token == HDOC)
		{
			if (ft_prompt(msh, msh->av, &msh->p.hdoc) == 130) // IF MALLOC KO OU ERREUR OPEN ON QUITTE A L INTERIEUR
				return (130); // CTRL + C OK GERE
		}
		msh->av = msh->av->next;
	}
	msh->av = head;
	return (0);
}

void	ft_unlink_heredoc(t_env *hdoc)
{
	int i;

	i = 0;
	while (hdoc)
	{
		unlink(hdoc->content);
		hdoc = hdoc->next;
		i++;
	}
}
