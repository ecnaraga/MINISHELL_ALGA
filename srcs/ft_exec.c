/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 14:20:52 by galambey          #+#    #+#             */
/*   Updated: 2023/12/11 17:42:38 by galambey         ###   ########.fr       */
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

int ft_exec_par(t_msh *msh, t_split **head, int rule);

int	ft_create_sub_msh(t_msh *sub_msh, t_msh *msh, t_split **head, int rule)
{
	pid_t	pid;
	
	(void) rule;
	sub_msh->line = ft_strtrim_msh(&msh->line);
	dprintf(2, "sub_msh->line %s\n", sub_msh->line);
	sub_msh->env = msh->env;
	pid = fork();
	if (pid == 0)
	{
		while (msh->av && msh->av->token != OPERATOR)
		{
			dprintf(2, "msh->av->data %s msh->av->token %d\n", msh->av->data, msh->av->token);
			
			if (msh->av->token == INFILE || msh->av->token == HERE_DOC)
				redef_stdin(msh, CMD_ALONE, 0);//TO DO adapter au pipe
			else if (msh->av->token == OUTFILE_TRUNC || msh->av->token == OUTFILE_NO_TRUNC)
			{
				// dprintf(2, "STDOUT\n");
				redef_stout(msh, CMD_ALONE, 0);
			}
			else
				msh->av = msh->av->next;
			// if (msh->av->token != PAR_OPEN)
			// 	ft_exec_par(msh, head, 1);
		}
		ft_minishell(sub_msh, 1);
	}
	else
	{
		while (msh->av && msh->av->token != OPERATOR)
		{
			msh->av = ft_lstdel_and_relink_split(msh->av, NULL, head);  
		}
	}
	return (0);
}

int ft_exec_par(t_msh *msh, t_split **head, int rule)
{
	t_msh	sub_msh;
	int		par;

	par = 0;
	dprintf(2, "msh->av->token %d\n", msh->av->token);
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
		dprintf(2, "msh->av->data %s msh->av->token %d par = %d\n", msh->av->data, msh->av->token, par);
		msh->av = ft_lstdel_and_relink_split(msh->av, NULL, head);
	}
	while (msh->av && msh->av->token == PAR_CLOSE) // et pourquoi pas if a la place
		msh->av = ft_lstdel_and_relink_split(msh->av, NULL, head);
	*head = msh->av;
	ft_create_sub_msh(&sub_msh, msh, head, rule);
	while (wait(&status) > 0)
	;
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	// printf("status %d\n", status);
	msh->av = *head;
	return (0);
}

void	ft_choice_exec(t_msh *msh, t_split **head)
{
	(void) head;
	if (msh->av->token == PAR_OPEN)
		ft_exec_par(msh, head, CMD_ALONE);
	else if (ft_search_pipe(msh) == 1)
		pipex_multi(msh);
	else
		ft_cmd_alone(msh);
}
	
int	ft_exec_operator(t_msh *msh, t_split **head)
{
	ft_choice_exec(msh, head);
	*head = msh->av;
	while (msh->av)
	{
		if (msh->av->token == OPERATOR && ft_strcmp(msh->av->data, "&&") == 0) //modifier env si cmd export avt operator
		{
			if (status == 0)
			{
				msh->av = msh->av->next;
				ft_choice_exec(msh, head);
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
				ft_choice_exec(msh, head);
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
	if (ft_heredoc(msh) == 130)
		return (1);
	head = msh->av;
	ft_exec_operator(msh, &head);
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