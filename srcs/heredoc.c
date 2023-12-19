/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 11:31:49 by galambey          #+#    #+#             */
/*   Updated: 2023/12/19 16:47:11 by galambey         ###   ########.fr       */
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
void	create_here_doc(t_msh *msh, t_split *av, t_env **hdoc)
{
	t_env	*new;
	char *tmp;
	
	/*Add the limiter in the linked list*/
	tmp = ft_magic_malloc(ADD, 0, ft_strdup(av->data), NO_ENV);
	if (!tmp)
		ft_exit_bis(msh, 0, -1, -1); // IF MALLOC KO ON QUITTE 
	new = ft_lst_new_heredoc(msh, tmp, 0); // IF MALLOC KO ON QUITTE A L INTERIEUR
	/* Create and add the name of the here_doc in the linked list*/
	while (1)
	{
		new->content = ft_magic_malloc(ADD, 0, ft_random_filename("/tmp/", 8), NO_ENV);
		if (!new->content)
			ft_exit_bis(msh, 0, -1, -1); // IF MALLOC KO ON QUITTE 
		if (ft_check_unique_name(new->content, hdoc) == 0)
			break;
		else
			ft_magic_malloc(FREE, 0, new->content, NO_ENV);
	}
	ft_lstadd_back_env(hdoc, new);
}

/*
Function called in a loop in the parent function
Copy into the file create for an heredoc, the stdin line entered by the user. 
*/
static int	ft_recover_prompt(t_msh *msh, int fd_temp, char *lim)
{
	char	*line;
	char *str;
	char *tmp;

	ft_signal_handler_msh_ter();
	write(1, "> ", 2);
	line = get_next_line_magic(STDIN_FILENO);
	if (status == 130)
	{
		if (open("/dev/stdout", O_RDONLY) == -1)
			(close(fd_temp), status = 1, ft_exit_bis(msh, 0, -1, -1)); // SI ERREUR OPEN , status == 1 + ON QUITTE
		return (close(fd_temp), 130); // SI CTRL + C OK GERE
	}
	if (status == 255)
		(close(fd_temp), ft_exit_bis(msh, 0, -1, -1)); // SI ERREUR MALLOC GNL > ON QUITTE
	if (!line)
	{
		str = ft_magic_malloc(ADD, 0, ft_strjoin_bfr_char("bash: warning: here-document delimited by end-of-file (wanted `", lim, '\n'), NO_ENV);
		if (status == 255)
			(close(fd_temp), ft_exit_bis(msh, 0, -1, -1)); // SI ERREUR MALLOC GNL > ON QUITTE
		tmp = ft_magic_malloc(ADD, 0, ft_strjoin(str, "')\n"), NO_ENV);
		if (status == 255)
			(close(fd_temp), ft_exit_bis(msh, 0, -1, -1)); // SI ERREUR MALLOC GNL > ON QUITTE
		write(2, tmp, ft_strlen(tmp));
		ft_magic_malloc(FREE, 0, str, NO_ENV); 
		ft_magic_malloc(FREE, 0, tmp, NO_ENV); 
		return (close(fd_temp), 1); // SI ERREUR MALLOC GNL > ON QUITTE
	}
	// {
	// 	(close(fd_temp), write(2, "get_next_line: error\n", 21), ft_magic_malloc(FREE, 0, lim, NO_ENV));
	// 	(unlink(hdoc->content)/*, ft_magic_malloc(FREE, 0, p->name_hdoc, NO_ENV)*/); //Penser a free
	// 	exit(EXIT_FAILURE);
	// }
	line = ft_magic_malloc(ADD, 0, line, NO_ENV);
	if (!line)
		(close(fd_temp), ft_exit_bis(msh, 0, -1, -1)); // SI MALLOC KO ON QUITTE
	if (ft_strcmp(lim, line) == 0)
		return (ft_magic_malloc(FREE, 0, line, NO_ENV), 1);
	(write(fd_temp, line, ft_strlen(line)), ft_magic_malloc(FREE, 0, line, NO_ENV));
	return (0);
}

/*
Function called in a loop in the parent function
For each limiter encountered, create a heredoc and recover the stdin to copy it
	into the file created
*/
int	ft_prompt(t_msh *msh, t_split *av, t_env **hdoc)
{
	int		fd_temp;
	char	*lim;
	t_env *head;

	create_here_doc(msh, av, hdoc); // IF MALLOC KO ON QUITTE A L'INTERIEUR 
	head = *hdoc;
	while ((*hdoc)->next/*  && ft_strcmp((*hdoc)->name, msh->av->data) != 0 */)
		*hdoc = (*hdoc)->next;
	fd_temp = open((*hdoc)->content, O_CREAT | O_APPEND | O_WRONLY, 0744);
	if (fd_temp == -1)
		(perror((*hdoc)->content), status = 1, ft_exit_bis(msh, 0, -1, -1)); // IF ERREUR OPEN -> status = 1 + ON QUITTE
	lim = ft_magic_malloc(ADD, 0, ft_strjoin(av->data, "\n"), NO_ENV);
	if (!lim)
		(close(fd_temp), ft_exit_bis(msh, 0, -1, -1)); // IF MALLOC KO ON QUITTE
	// {
	// 	(close(fd_temp), write(2, "ft_strjoin: error\n", 18));
	// 	unlink((*hdoc)->content)/* ,free(p->name_hdoc)*/;
	// 	exit(EXIT_FAILURE);  //PENSER A FREE 
	// }
	while (1)
	{
		int err ;

		err = ft_recover_prompt(msh, fd_temp, lim); // IF MALLOC KO OU ERREUR OPEN ON QUITTE A L INTERIEUR
		if (err == 1) // LIM RENCONTRE
			break ;
		if (err == 130) // SI CTRL + C OK GERE
			return (130);
	}
	ft_signal_handler_msh();
	*hdoc = head;
 	(close(fd_temp), ft_magic_malloc(FREE, 0, lim, NO_ENV));
	return (0);
}

/*
Create a copy of the list of heredoc
If there is no list of heredoc, return sub_heredoc = NULL
If error malloc, quit the actual process
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
		tmp = ft_magic_malloc(ADD, 0, ft_strdup(hdoc->name), NO_ENV);
		if (!tmp)
			ft_exit_bis(msh, sub, -1, -1); // SI MALLOC KO ON QUITTE
		new = ft_lst_new_heredoc(msh, tmp, sub); // SI MALLOC KO ON QUITTE DANS F_LST_NEW_HEREDOC 
		new->content = ft_magic_malloc(ADD, 0, ft_strdup(hdoc->content), NO_ENV);
		if (!new->content)
			ft_exit_bis(msh, sub, -1, -1); // SI MALLOC KO ON QUITTE
		// if (hdoc->read == 1)
		// 	new->read = 1;
		// else
		// 	new->read = 0;
		dprintf(2, "hdoc->read = %d\n", hdoc->read);
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
