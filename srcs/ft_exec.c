/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 14:20:52 by galambey          #+#    #+#             */
/*   Updated: 2023/12/15 16:46:02 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//SAME
int	ft_search_pipe(t_msh *msh)
{
	t_split *head;

	head = msh->av;
	while (msh->av && msh->av->token != OPERATOR && msh->av->token != PAR_OPEN
		&& msh->av->token != PAR_CLOSE)
	{
		if (msh->av->token == PIPE)
		{
			msh->av = head;
			return (1);
		}
		msh->av = msh->av->next;
	}
	msh->av = head;
	return (0);
}

//****************************************************************************************

// int	ft_create_sub_msh(t_msh *sub_msh, t_msh *msh, t_split **head, int rule, int j)
// {
// 	pid_t	pid;
// 	int redef;
	
// 	sub_msh->line = ft_strtrim_msh(&msh->line);
// 	sub_msh->env = msh->env;
// 	pid = fork();
// 	redef = 0;
// 	if (pid == 0)
// 	{
		
// 		while (msh->av && msh->av->token != OPERATOR)
// 		{
// 			if (msh->av->token == INFILE || msh->av->token == HERE_DOC)
// 			{
// 				redef_stdin(msh, rule, j);//TO DO adapter au pipe
// 				if (redef == 0)
// 					redef = 1;
// 				else if (redef == 2)
// 					redef = 3;
// 			}
// 			else if (msh->av->token == OUTFILE_TRUNC || msh->av->token == OUTFILE_NO_TRUNC)
// 			{
// 				redef_stout(msh, rule, j);//TO DO adapter au pipe
// 				if (redef == 0)
// 					redef = 2;
// 				else if (redef == 1)
// 					redef = 3;
// 			}
// 			else
// 				msh->av = msh->av->next;
// 		}
// 		if (rule != CMD_ALONE && redef < 3)
// 		{
// 			if (redef == 0 || redef == 2)
// 				redef_stdin(msh, rule, 0);//TO DO adapter au pipe
// 			if (redef == 0 || redef == 1)
// 				redef_stout(msh, rule, 0);//TO DO adapter au pipe
// 		}
// 		ft_minishell(sub_msh, 1);
// 	}
// 	else
// 	{
// 		while (msh->av && msh->av->token != OPERATOR)
// 			msh->av = ft_lstdel_and_relink_split(msh->av, NULL, head);  
// 	}
// 	return (0);
// }

// int ft_exec_par(t_msh *msh, t_split **head, int rule, int j)
// {
// 	t_msh	sub_msh;

// 	while (msh->av->token != PAR_CLOSE)
// 		msh->av = ft_lstdel_and_relink_split(msh->av, NULL, head);
// 	while (msh->av && msh->av->token == PAR_CLOSE)
// 		msh->av = ft_lstdel_and_relink_split(msh->av, NULL, head);
// 	*head = msh->av;
// 	ft_create_sub_msh(&sub_msh, msh, head, rule, j);
// 	while (wait(&status) > 0)
// 	;
// 	if (WIFEXITED(status))
// 		status = WEXITSTATUS(status);
// 	// printf("status %d\n", status);
// 	msh->av = *head;
// 	return (0);
// }
	
// int	ft_exec_operator(t_msh *msh, t_split **head)
// {
// 	if (ft_search_pipe(msh) == 1)
// 		pipex_multi(msh);
// 	else
// 		ft_cmd_alone(msh);
// 	*head = msh->av;
// 	while (msh->av)
// 	{
// 		if (msh->av->token == PAR_OPEN)
// 		{
// 			ft_exec_par(msh, head, CMD_ALONE, 0);
// 			continue;
// 		}
// 		if (msh->av->token == OPERATOR && ft_strcmp(msh->av->data, "&&") == 0)
// 		{
// 			if (status == 0)
// 			{
// 				msh->av = msh->av->next;
// 				if (ft_search_pipe(msh) == 1)
// 					pipex_multi(msh);
// 				else
// 					ft_cmd_alone(msh);
// 				// if (msh->av)
// 				*head = msh->av;
// 				continue;
// 			}
// 			else
// 			{
// 				while (msh->av)
// 					msh->av = ft_lstdel_and_relink_split(msh->av, NULL, head);
// 				continue;
// 			}
// 		}
// 		if (msh->av->token == OPERATOR && ft_strcmp(msh->av->data, "||") == 0)
// 		{
// 			if (status != 0)
// 			{
// 				msh->av = msh->av->next;
// 				if (ft_search_pipe(msh) == 1)
// 					pipex_multi(msh);
// 				else
// 					ft_cmd_alone(msh);
// 				// if (msh->av)
// 				*head = msh->av;
// 				continue;
// 			}
// 			else
// 			{
// 				while (msh->av)
// 				{
// 					msh->av = ft_lstdel_and_relink_split(msh->av, NULL, head);
// 				}
// 			}
// 		}
// 		else
// 			msh->av = msh->av->next;
// 	}
// 	return (0);
// }

// int	ft_exec(t_msh *msh, int sub)
// {
// 	t_split *head;
	
// 	(void) sub;
// 	ft_heredoc(msh);
// 	head = msh->av;
// 	if (msh->av->token == PAR_OPEN)
// 		ft_exec_par(msh, &head, CMD_ALONE, 0);
// 	if (msh->av)
// 		ft_exec_operator(msh, &head);
// 	else
// 		return (0);
// 	msh->av = head;
// 	ft_unlink_heredoc(msh->p.here_doc);
// 	ft_magic_malloc(FLUSH, 0, NULL, NO_ENV);
// 	return (0);
// }
//****************************************************************************



void	ft_create_sub_msh(t_msh *sub_msh, t_msh *msh, /* t_split **head,  */int sub)
// int	ft_create_sub_msh(t_msh *sub_msh, t_msh *msh, /* t_split **head,  */int sub)
{
	pid_t	pid;
	// int		fd;
	
	sub_msh->line = ft_strtrim_msh(msh, &msh->line, sub); // SI MALLOC KO, ON QUITTE LE PROCESS ACTUEL A L INTERIEUR
	sub_msh->env = msh->env;
	sub_msh->export_env = msh->export_env;
	pid = fork();
	if (pid == -1)
		(status = 255, ft_exit_bis(msh, sub, -1, -1)); // SI FORK KO, ON QUITTE LE PROCESS ACTUEL 
	if (pid == 0)
	{
		while (msh->av && msh->av->token != OPERATOR)
		{
			if (msh->av->token == INFILE || msh->av->token == HERE_DOC)
			{
				if (redef_stdin(msh, CMD_ALONE, 0, 1) == -1) // SI ERREUR OPEN OU DUP OU DUP2 QUIT LE PROCESS ACTUEL + SI MALLOC KO ON QUITTE A L INTERIEUR
					ft_exit(-1, -1, -1);
			}
			else if (msh->av->token == OUTFILE_TRUNC || msh->av->token == OUTFILE_NO_TRUNC)
			{	
				if (redef_stdout(msh, CMD_ALONE, 0, 1) == -1) // SI ERREUR OPEN OU DUP OU DUP2 QUIT LE PROCESS ACTUEL + SI MALLOC KO ON QUITTE A L INTERIEUR
					ft_exit(-1, -1, -1); 
			}
			else
				msh->av = msh->av->next;
		}
		ft_minishell(sub_msh, 1);
	}
	// else A PRIORI ON NE PASSE JAMAIS DEDANS N EAFFACER QUE SI ON EST CERTAINSSSSS QUE C EST TOUJOURS FAIT AVANT DANS FT_EXEC_PAR
	// {
	// 	while (msh->av && msh->av->token != OPERATOR)
	// 	{
	// 		dprintf(2, "test\n");
	// 		msh->av = ft_lstdel_and_relink_split(msh->av, NULL, head);  
	// 	}
	// }
	// return (0);
}

void ft_exec_par(t_msh *msh, t_split **head, int rule, int sub)
// int ft_exec_par(t_msh *msh, t_split **head, int rule, int sub)
{
	t_msh	sub_msh;
	int		par;
	t_env *head_hd;
	t_env *prev_hd;

(void) rule;
	par = 0;
	dprintf(2, "msh->av->token %d\n", msh->av->token);
	sub_msh.p.here_doc = ft_copy_heredoc(msh, msh->p.here_doc, sub); // SI MALLOC KO ON QUITTE DANS 
	while (msh->av->token != PAR_CLOSE || par != 0)
	{
		if (msh->av->token == PAR_OPEN)
			par++;
		else if (msh->av->token == PAR_CLOSE)
		{
			par--;
			if (par == 0)
				break ;
		}
		if (msh->av->token == HERE_DOC)
		{
			head_hd = msh->p.here_doc;
			prev_hd = NULL;
			while (msh->p.here_doc && (ft_strcmp(msh->p.here_doc->name, msh->av->data) != 0 || (ft_strcmp(msh->p.here_doc->name, msh->av->data) == 0 && msh->p.here_doc->read == 1)))
			{
				prev_hd = msh->p.here_doc;
				msh->p.here_doc = msh->p.here_doc->next;
			}
			if (msh->p.here_doc)
				msh->p.here_doc->read = 1;
			msh->p.here_doc = head_hd;
		}
		// dprintf(2, "msh->av->data %s msh->av->token %d par = %d\n", msh->av->data, msh->av->token, par);
		msh->av = ft_lstdel_and_relink_split(msh->av, NULL, head);
	}
	while (msh->av && msh->av->token == PAR_CLOSE) // et pourquoi pas if a la place
		msh->av = ft_lstdel_and_relink_split(msh->av, NULL, head);
	*head = msh->av;
	ft_create_sub_msh(&sub_msh, msh, sub); // OK TOUT EST PROTEGE A L INTERIEUR
	// ft_create_sub_msh(&sub_msh, msh, head, rule);
	while (wait(&status) > 0)
	;
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	// printf("status %d\n", status);
	msh->av = *head;
	// return (0);
}

void	ft_choice_exec(t_msh *msh, t_split **head, int sub)
{
	(void) head;
	if (msh->av->token == PAR_OPEN)
		ft_exec_par(msh, head, CMD_ALONE, sub); // OK PROTEGE A L INTERIEUR
	else if (ft_search_pipe(msh) == 1)
		pipex_multi(msh, sub); // OK PROTEGE A L INTERIEUR
	else
		ft_cmd_alone(msh, sub); // OK PROTEGE A L INTERIEUR
}
	
int	ft_exec_operator(t_msh *msh, t_split **head, int sub)
{
	ft_choice_exec(msh, head, sub); // OK PROTEGE A L INTERIEUR
	*head = msh->av;
	while (msh->av)
	{
		if (msh->av->token == OPERATOR && ft_strcmp(msh->av->data, "&&") == 0) //modifier env si cmd export avt operator
		{
			if (status == 0)
			{
				msh->av = msh->av->next;
				ft_choice_exec(msh, head, sub); // OK PROTEGE A L INTERIEUR
				*head = msh->av;
				continue;
			}
			else
			{
				while (msh->av)
					msh->av = ft_lstdel_and_relink_split(msh->av, NULL, head);
				continue;
			}
		}
		if (msh->av->token == OPERATOR && ft_strcmp(msh->av->data, "||") == 0)
		{
			if (status != 0) //modifier env si cmd export avt operator
			{
				msh->previous_status = status;
				status = 0;
				msh->av = msh->av->next;
				ft_choice_exec(msh, head, sub); // OK PROTEGE A L INTERIEUR
				*head = msh->av;
				continue;
			}
			else
			{
				while (msh->av)
					msh->av = ft_lstdel_and_relink_split(msh->av, NULL, head);
			}
		}
		else
			msh->av = msh->av->next;
	}
	return (0);
}



int	ft_exec(t_msh *msh, int sub)
{
	t_split *head;
	
	(void) sub;
	printf("FT_EXEC\n");
	if (sub == 0 && ft_heredoc(msh) == 130) // SI ERREUR MALLOC OU OPEN ON QUITTE A L INTERIEUR
		return (1); // OK CTRL + C GERE
	head = msh->av;
	ft_exec_operator(msh, &head, sub); // OK PROTEGE A L INTERIEUR
	if (sub == 0)
		ft_unlink_heredoc(msh->p.here_doc);
	ft_magic_malloc(FLUSH, 0, NULL, NO_ENV);
	return (0);
}
//****************************************************************************
// int	ft_exec(t_msh *msh)
// {
// 	t_split *head;
// 	// t_split *prev;
// 	// int pipe;
	
// 	printf("FT_EXEC\n");
// 	ft_heredoc(msh);
// 	head = msh->av;
// 	// pipe = 0;
// 	if (ft_search_pipe(msh) == 1)
// 		pipex_multi(msh);
// 	else
// 		ft_cmd_alone(msh);
// 	if (msh->av)
// 		printf("msh->av->data %s\n", msh->av->data);
// 	head = msh->av;
// 	while (msh->av)
// 	{
// 		if (msh->av->token == OPERATOR && ft_strcmp(msh->av->data, "&&") == 0)
// 		{
// 			if (status == 0)
// 			{
// 				msh->av = msh->av->next;
// 				if (ft_search_pipe(msh) == 1)
// 					pipex_multi(msh);
// 				else
// 					ft_cmd_alone(msh);
// 				if (msh->av)
// 					printf("msh->av->data %s\n", msh->av->data);
// 				head = msh->av;
// 				continue;
// 			}
// 			else
// 			{
// 				while (msh->av)
// 					msh->av = ft_lstdel_and_relink_split(msh->av, NULL, &head);
// 				continue;
// 			}
// 		}
// 		if (msh->av->token == OPERATOR && ft_strcmp(msh->av->data, "||") == 0)
// 		{
// 			if (status != 0)
// 			{
// 				msh->av = msh->av->next;
// 				if (ft_search_pipe(msh) == 1)
// 					pipex_multi(msh);
// 				else
// 					ft_cmd_alone(msh);
// 				if (msh->av)
// 					printf("msh->av->data %s\n", msh->av->data);
// 				head = msh->av;
// 				continue;
// 			}
// 			else
// 			{
// 				while (msh->av)
// 					msh->av = ft_lstdel_and_relink_split(msh->av, NULL, &head);
// 			}
// 		}
// 		else
// 		{
// 			// prev = msh->av;
// 			msh->av = msh->av->next;
// 		}
// 	}
// 	msh->av = head;
// 	printf("head %p\n", head);
// 	// if (head)
// 	// 	ft_cmd_alone(msh);
// 	ft_unlink_heredoc(msh->p.here_doc);
// 	ft_magic_malloc(FLUSH, 0, NULL, NO_ENV);
// 	return (0);
// }