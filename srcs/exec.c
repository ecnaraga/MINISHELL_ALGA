/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 14:20:52 by galambey          #+#    #+#             */
/*   Updated: 2023/12/22 13:11:55 by galambey         ###   ########.fr       */
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





void	ft_create_sub_msh(t_msh *sub_msh, t_msh *msh, int sub)
// int	ft_create_sub_msh(t_msh *sub_msh, t_msh *msh, /* t_split **head,  */int sub)
{
	pid_t	pid;
	
	sub_msh->line = ft_strtrim_msh(msh, &msh->line, sub); // SI MALLOC KO, ON QUITTE LE PROCESS ACTUEL A L INTERIEUR
	sub_msh->env = msh->env;
	sub_msh->status = msh->status;
	sub_msh->export_env = msh->export_env;
	pid = fork();
	if (pid == -1)
		(msh->status = 255, ft_exit_bis(msh, sub, -1, -1)); // SI FORK KO, ON QUITTE LE PROCESS ACTUEL 
	if (pid == 0)
	{
		// msh->p.hdoc = sub_msh->p.hdoc;
		// printf("FT_CREATE sub_msh->p.hdoc->read %d\n", sub_msh->p.hdoc->read);
		// printf("FT_CREATE msh->p.hdoc->read %d\n", msh->p.hdoc->read);
		while (msh->av && msh->av->token != OPERATOR)
		{
			// dprintf(2, "msh->av->data %s\n", msh->av->data);
			if (msh->av->token == HDOC)
			{
				// dprintf(2, "HDOC\n");
				// msh->p.hdoc = sub_msh->p.hdoc;
				if (redef_stdin(msh, PAR_OPEN, 0, 1) == -1) // SI ERREUR OPEN OU DUP OU DUP2 QUIT LE PROCESS ACTUEL + SI MALLOC KO ON QUITTE A L INTERIEUR
				// if (redef_stdin(msh, PAR_OPEN, 0, 1) == -1) // SI ERREUR OPEN OU DUP OU DUP2 QUIT LE PROCESS ACTUEL + SI MALLOC KO ON QUITTE A L INTERIEUR
					ft_exit(-1, -1, -1, msh);
				msh->av = msh->av->next;
				continue ;
			}
			if (msh->av->token == INFILE)
			{
				if (redef_stdin(msh, PAR_OPEN, 0, 1) == -1) // SI ERREUR OPEN OU DUP OU DUP2 QUIT LE PROCESS ACTUEL + SI MALLOC KO ON QUITTE A L INTERIEUR
					ft_exit(-1, -1, -1, msh);
			}
			else if (msh->av->token == OUTFILE_TRUNC || msh->av->token == OUTFILE_NO_TRUNC)
			{	
				if (redef_stdout(msh, PAR_OPEN, 0, 1) == -1) // SI ERREUR OPEN OU DUP OU DUP2 QUIT LE PROCESS ACTUEL + SI MALLOC KO ON QUITTE A L INTERIEUR
					ft_exit(-1, -1, -1, msh); 
			}
			else
				msh->av = msh->av->next;
		}
		ft_minishell(sub_msh, 1);
	}
	// else// A PRIORI ON NE PASSE JAMAIS DEDANS N EAFFACER QUE SI ON EST CERTAINSSSSS QUE C EST TOUJOURS FAIT AVANT DANS FT_EXEC_PAR
	// {
	// 	while (msh->av && msh->av->token != OPERATOR)
	// 	{
	// 		dprintf(2, "test\n");
	// 		msh->av = ft_lstdel_and_relink_split(msh, msh->av, NULL, head);  
	// 	}
	// }
	// return (0);
}

void ft_exec_par(t_msh *msh, t_split **head, int sub)
// int ft_exec_par(t_msh *msh, t_split **head, int rule, int sub)
{
	t_msh	sub_msh;
	int		par;
	t_env *head_hd;
	t_env *prev_hd;

	par = 0;
	sub_msh.p.hdoc = ft_copy_heredoc(msh, msh->p.hdoc, sub); // SI MALLOC KO ON QUITTE DANS 
	*head = msh->av;
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
		if (msh->av->token == HDOC)
		{
			head_hd = msh->p.hdoc;
			prev_hd = NULL;
			while (msh->p.hdoc && (ft_strcmp(msh->p.hdoc->name, msh->av->data) != 0 || (ft_strcmp(msh->p.hdoc->name, msh->av->data) == 0 && msh->p.hdoc->read == 1)))
			{
				prev_hd = msh->p.hdoc;
				msh->p.hdoc = msh->p.hdoc->next;
			}
			if (msh->p.hdoc)
				msh->p.hdoc->read = 1;
			msh->p.hdoc = head_hd;
		}
		msh->av = ft_lstdel_and_relink_split(msh, msh->av, NULL, head);
	}
	while (msh->av && msh->av->token == PAR_CLOSE) // et pourquoi pas if a la place
		msh->av = ft_lstdel_and_relink_split(msh, msh->av, NULL, head);
	ft_create_sub_msh(&sub_msh, msh, sub); // OK TOUT EST PROTEGE A L INTERIEUR
	while (msh->av->token != OPERATOR)
	{
		if (msh->av->token == HDOC)
		{
			head_hd = msh->p.hdoc;
			prev_hd = NULL;
			while (msh->p.hdoc && (ft_strcmp(msh->p.hdoc->name, msh->av->data) != 0 || (ft_strcmp(msh->p.hdoc->name, msh->av->data) == 0 && msh->p.hdoc->read == 1)))
			{
				prev_hd = msh->p.hdoc;
				msh->p.hdoc = msh->p.hdoc->next;
			}
			if (msh->p.hdoc)
				msh->p.hdoc->read = 1;
			msh->p.hdoc = head_hd;
		}
		msh->av = ft_lstdel_and_relink_split(msh, msh->av, NULL, head);
	}
	while (wait(&msh->status) > 0)
	;
	if (WIFEXITED(msh->status))
		msh->status = WEXITSTATUS(msh->status);
	msh->av = *head;
}

void	ft_choice_exec(t_msh *msh, t_split **head, int sub)
{
	(void) head;
	if (msh->av->token == PAR_OPEN)
	{
		// printf("TEST PAR OPEN\n");
		ft_exec_par(msh, head, sub); // OK PROTEGE A L INTERIEUR
	}
	else if (ft_search_pipe(msh) == 1)
	{
		// printf("PIPEX_MULTI\n");
		pipex_multi(msh, sub); // OK PROTEGE A L INTERIEUR
	}
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
			if (msh->status == 0)
			{
				msh->av = msh->av->next;
				ft_choice_exec(msh, head, sub); // OK PROTEGE A L INTERIEUR
				*head = msh->av;
				continue;
			}
			else
			{
				while (msh->av)
					msh->av = ft_lstdel_and_relink_split(msh, msh->av, NULL, head);
				continue;
			}
		}
		if (msh->av->token == OPERATOR && ft_strcmp(msh->av->data, "||") == 0)
		{
			if (msh->status != 0) //modifier env si cmd export avt operator
			{
				msh->previous_status = msh->status;
				msh->status = 0;
				msh->av = msh->av->next;
				ft_choice_exec(msh, head, sub); // OK PROTEGE A L INTERIEUR
				*head = msh->av;
				continue;
			}
			else
			{
				while (msh->av)
					msh->av = ft_lstdel_and_relink_split(msh, msh->av, NULL, head);
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
	// printf("FT_EXEC\n");
	if (sub == 0 && ft_heredoc(msh) == 130) // SI ERREUR MALLOC OU OPEN ON QUITTE A L INTERIEUR
		return (1); // OK CTRL + C GERE
	head = msh->av;
	ft_exec_operator(msh, &head, sub); // OK PROTEGE A L INTERIEUR
	if (sub == 0)
		ft_unlink_heredoc(msh->p.hdoc);
	mlcgic(NULL, FLUSH, NO_ENV, msh);
	// ft_magic_malloc(FLUSH, 0, NULL, NO_ENV);
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