/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 14:20:52 by galambey          #+#    #+#             */
/*   Updated: 2023/12/04 17:03:48 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_search_pipe(t_msh *msh)
{
	t_split *head;

	head = msh->av;
	while (msh->av && msh->av->token != OPERATOR && msh->av->token != PAR_OPEN && msh->av->token != PAR_CLOSE)
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

int	ft_create_sub_msh(t_msh *sub_msh, t_msh *msh, t_split **head, int rule)
{
	pid_t	pid;
	
	(void) rule;
	sub_msh->line = ft_strtrim_msh(&msh->line);
	printf("sub_msh->line %s\n", sub_msh->line);
	sub_msh->env = msh->env;
	pid = fork();
	if (pid == 0)
	{
		while (msh->av && msh->av->token != OPERATOR)
		{
			if (msh->av->token == INFILE || msh->av->token == HERE_DOC)
				redef_stdin(msh, CMD_ALONE, 0);//TO DO adapter au pipe
			else if (msh->av->token == OUTFILE_TRUNC || msh->av->token == OUTFILE_NO_TRUNC)
			{
				printf("STDOUT\n");
				redef_stout(msh, CMD_ALONE, 0);
			}
			else
				msh->av = msh->av->next;
		}
		ft_minishell(sub_msh, 1);
		// while (wait(&status) > 0)
		// ;
		// if (WIFEXITED(status))
		// 	status = WEXITSTATUS(status);
		// return (0);
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

	while (msh->av->token != PAR_CLOSE)
		msh->av = ft_lstdel_and_relink_split(msh->av, NULL, head);
	while (msh->av && msh->av->token == PAR_CLOSE)
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
	
int	ft_exec_operator(t_msh *msh, t_split **head)
{
	if (ft_search_pipe(msh) == 1)
		pipex_multi(msh);
	else
		ft_cmd_alone(msh);
	if (msh->av)
		printf("msh->av->data %s\n", msh->av->data);
	*head = msh->av;
	while (msh->av)
	{
		if (msh->av->token == PAR_OPEN)
		{
			ft_exec_par(msh, head, CMD_ALONE);
			continue;
		}
		if (msh->av->token == OPERATOR && ft_strcmp(msh->av->data, "&&") == 0)
		{
			if (status == 0)
			{
				msh->av = msh->av->next;
				if (msh->av->token == PAR_OPEN)
				{
					ft_exec_par(msh, head, CMD_ALONE);
					continue;
				}
				else if (ft_search_pipe(msh) == 1)
					pipex_multi(msh);
				else
					ft_cmd_alone(msh);
				if (msh->av)
					printf("msh->av->data %s\n", msh->av->data);
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
			if (status != 0)
			{
				msh->av = msh->av->next;
				if (msh->av->token == PAR_OPEN)
				{
					ft_exec_par(msh, head, CMD_ALONE);
					continue;
				}
				else if (ft_search_pipe(msh) == 1)
					pipex_multi(msh);
				else
					ft_cmd_alone(msh);
				if (msh->av)
					printf("msh->av->data %s\n", msh->av->data);
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
		{
			// prev = msh->av;
			msh->av = msh->av->next;
		}
	}
	// msh->av = head;
	// printf("head %p\n", head);
	// // if (head)
	// // 	ft_cmd_alone(msh);
	// ft_unlink_heredoc(msh->p.here_doc);
	// ft_magic_malloc(FLUSH, 0, NULL, NO_ENV);
	return (0);
}



int	ft_exec(t_msh *msh, int sub)
{
	t_split *head;
	// t_msh	sub_msh;
	// t_split *tmp;
	// int pipe;
	
	(void) sub;
	printf("FT_EXEC\n");
	ft_heredoc(msh);
	head = msh->av;
	if (msh->av->token == PAR_OPEN)
		ft_exec_par(msh, &head, CMD_ALONE);
	// pipe = 0;
	// if (msh->av->token == PAR_OPEN)
	// {
	// 	while (msh->av->token != PAR_CLOSE)
	// 		msh->av = ft_lstdel_and_relink_split(msh->av, NULL, &head);
	// 		// msh->av = msh->av->next;
	// 	while (msh->av && msh->av->token == PAR_CLOSE)
	// 		msh->av = ft_lstdel_and_relink_split(msh->av, NULL, &head);
	// 		// msh->av = msh->av->next;
	// 	head = msh->av;
	// 	ft_create_sub_msh(&sub_msh, msh, &head);
	// 	printf("TESTTTTTTTTTTTTT\n");
	// 	while (wait(&status) > 0)
	// 	;
	// 	if (WIFEXITED(status))
	// 		status = WEXITSTATUS(status);
	// 	printf("TESTTTTTTTTTTTTT0\n");
	// 	printf("status %d\n", status);
	// 	msh->av = head;
	// // 	msh->av = ft_lstdel_and_relink_split(msh->av, NULL, &head);//TO DO ADAPTER SI PREV EXISTE
	// // 	while (msh->av->token != PAR_CLOSE)
	// // 		msh->av = msh->av->next;
	// // 	while (msh->av->token == PAR_CLOSE)
	// // 		msh->av = msh->av->next;
	// // 	while (msh->av && msh->av->token != OPERATOR)
	// // 	{
	// // 		if (msh->av->token == INFILE || msh->av->token == HERE_DOC)
	// // 			redef_stdin(msh, CMD_ALONE, 0);//TO DO adapter au pipe
	// // 		else if (msh->av->token == OUTFILE_TRUNC || msh->av->token == OUTFILE_NO_TRUNC)
	// // 		{
	// // 			printf("STDOUT\n");
	// // 			redef_stout(msh, CMD_ALONE, 0);
	// // 		}
	// // 		else
	// // 			msh->av = msh->av->next;
	// // 	}
	// // 	tmp = msh->av;
	// // 	msh->av = head;
	// // 	ft_exec(msh, 1);
	// // 	msh->av = tmp;
	// }
	if (msh->av)
		ft_exec_operator(msh, &head);
	else
		return (0);
	// if (ft_search_pipe(msh) == 1)
	// 	pipex_multi(msh);
	// else
	// 	ft_cmd_alone(msh);
	// if (msh->av)
	// 	printf("msh->av->data %s\n", msh->av->data);
	// head = msh->av;
	// while (msh->av)
	// {
	// 	if (msh->av->token == OPERATOR && ft_strcmp(msh->av->data, "&&") == 0)
	// 	{
	// 		if (status == 0)
	// 		{
	// 			msh->av = msh->av->next;
	// 			if (ft_search_pipe(msh) == 1)
	// 				pipex_multi(msh);
	// 			else
	// 				ft_cmd_alone(msh);
	// 			if (msh->av)
	// 				printf("msh->av->data %s\n", msh->av->data);
	// 			head = msh->av;
	// 			continue;
	// 		}
	// 		else
	// 		{
	// 			while (msh->av)
	// 				msh->av = ft_lstdel_and_relink_split(msh->av, NULL, &head);
	// 			continue;
	// 		}
	// 	}
	// 	if (msh->av->token == OPERATOR && ft_strcmp(msh->av->data, "||") == 0)
	// 	{
	// 		if (status != 0)
	// 		{
	// 			msh->av = msh->av->next;
	// 			if (ft_search_pipe(msh) == 1)
	// 				pipex_multi(msh);
	// 			else
	// 				ft_cmd_alone(msh);
	// 			if (msh->av)
	// 				printf("msh->av->data %s\n", msh->av->data);
	// 			head = msh->av;
	// 			continue;
	// 		}
	// 		else
	// 		{
	// 			while (msh->av)
	// 				msh->av = ft_lstdel_and_relink_split(msh->av, NULL, &head);
	// 		}
	// 	}
	// 	else
	// 	{
	// 		// prev = msh->av;
	// 		msh->av = msh->av->next;
	// 	}
	// }
	msh->av = head;
	if (head)
		printf("head %p\n", head);
	// if (head)
	// 	ft_cmd_alone(msh);
	ft_unlink_heredoc(msh->p.here_doc);
	ft_magic_malloc(FLUSH, 0, NULL, NO_ENV);
	return (0);
}

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