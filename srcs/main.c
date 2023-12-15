/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 12:09:51 by galambey          #+#    #+#             */
/*   Updated: 2023/12/15 15:59:10 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minishell.h"

/*TO DO
- return, exit, ATTENTION AU CODE ERREURS RETOURNES*/

int status = 0;

/*
Call all the parsing function + split the line + tokenisation
*/
int ft_parsing(t_msh *msh, int sub)
{
	if (sub == 0 && ft_parse_line(msh) != 0) // OK PROTEGE ET SI MALLOC KO ON QUITTE A L INTERIEUR
		return (ft_magic_malloc(FLUSH, 0, NULL, 0), 1);
	// printf("PASSAGE ALIX TO GAGA\n");
	if (sub == 0 && ft_parse_bis(msh) != 0) // OK PROTEGE ET SI MALLOC KO ON QUITTE A L INTERIEUR
		return (ft_magic_malloc(FLUSH, 0, NULL, 0), 1);
	msh->av = ft_split_msh(msh->line); // OK PROTEGE ET SI MALLOC KO ON QUITTE A L INTERIEUR
	if (!msh->av)
		return (ft_magic_malloc(FLUSH, 0, NULL, 0), 1);
	msh->ac = ft_lstsize_split(msh->av);
	if (msh->ac == 0)
		return (ft_magic_malloc(FLUSH, 0, NULL, 0), 1);
	ft_token(msh);
	if (sub == 0 && ft_parse_ter(msh) != 0) // OK PROTEGE ET SI MALLOC KO ON QUITTE A L INTERIEUR
		return (ft_magic_malloc(FLUSH, 0, NULL, 0), 1);
	return (0);
}

/*
In case of Ctrl-D, free all and quit the program
*/
void	ft_handle_eof(void)
{
	ft_putstr_fd("exit\n", 2);
	ft_magic_malloc(QUIT, 0, NULL, 0);
	rl_clear_history();
}

void	ft_minishell(t_msh *msh, int sub)
// int	ft_minishell(t_msh *msh, int sub)
{
	if (ft_parsing(msh, sub) != 0)  // OK PROTEGE ET SI MALLOC KO ON QUITTE A L INTERIEUR
		return ;
		// return (1);
		
	// int i = 0;
	// t_split *head;
	// head = msh->av;
	// while (msh->av)
	// {
	// 	printf("%d msh->av->data = |%s| msh->av->token = %d\n", i, msh->av->data, msh->av->token);
	// 	if (msh->av->type)
	// 	{
	// 		int d = -1;
	// 		while (++d < msh->av->dollar)
	// 			printf("msh->av->type[%d].expnd = %d msh->av->type[%d].len_variable = %d\n", d,
	// 				msh->av->type[d].expnd, d,
	// 				msh->av->type[d].len_variable);
	// 	}
	// 	msh->av = msh->av->next;
	// 	i++;
	// }
	// msh->av = head;
	
	if (ft_exec(msh, sub) != 0) // OK GERE ET PROTEGE A l interieur
		return ;
		// return (1);
	if (sub == 1)
		ft_exit(-1, -1, -1);
	// return (0);
}

int main(int ac, char **av, char **env)
{
	t_msh msh;
	
	(void)av;
	if (isatty(0) == 1) //A REGLER test ./minishell | ./minishell
	{
		int fd = open("/dev/stdin", O_RDWR);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	if (isatty(0) == 0) //A REGLER test ./minishell | ./minishell
		exit (0);
	if (ac != 1)
		return (write(2, "bash: minishell: too many arguments\n", 37), 1); // si cd avec 2 arguments meme message d erreur et exit status 1
	msh.env = get_env(env);
	msh.export_env = get_export_env(env);	
	while (1)
	{
		ft_signal_handler_msh();
		msh.line = readline("Minishell$ "); // PROTEGE OK
		msh.previous_status = status;
		status = 0;
		// /*--------------------------------------------------------*/
		// if(strcmp(msh.line, "pwd") == 0)
		// 	builtin_pwd(&msh);
		// if(strcmp(msh.line, "env") == 0)
		// 	builtin_env(&msh);
		// if(ft_strncmp(msh.line, "export", 6) == 0)
		// 	builtin_export(ft_split(msh.line, ' '), &msh);
		// if(ft_strncmp(msh.line, "cd", 2) == 0)
		// 	builtin_cd(ft_split(msh.line, ' '), &msh);
		// if(ft_strncmp(msh.line, "unset", 5) == 0)
		// 	builtin_unset(ft_split(msh.line, ' '), &msh);
		// /*--------------------------------------------------------*/
		if (!msh.line) // EOF
			return (ft_handle_eof(), status); // RENVOYER LE DERNIER CODE ERREUR STOCKE AVANT LE CTRL D
		if (!ft_magic_malloc(ADD, 0, msh.line, NO_ENV))
		{
			ft_magic_malloc(QUIT, 0, NULL, 0); // SI MALLOC KO ON QUITTE
			// ft_magic_malloc(FLUSH, 0, NULL, 0);
			continue ;
		}	
		if (!msh.line[0])
		{
			ft_magic_malloc(FLUSH, 0, NULL, 0);
			continue ;	
		}
		add_history(msh.line);
		ft_minishell(&msh, 0);
		
		// if (ft_parsing(&msh) != 0)
		// 	continue;
		// int i = 0;
		// t_split *head;
		// head = msh.av;
		// while (msh.av)
		// {
		// 	printf("%d msh.av->data = |%s| msh.av->token = %d\n", i, msh.av->data, msh.av->token);
		// 	if (msh.av->type)
		// 	{
		// 		int d = -1;
		// 		while (++d < msh.av->dollar)
		// 			printf("msh.av->type[%d].expnd = %d msh.av->type[%d].len_variable = %d\n", d,
		// 				msh.av->type[d].expnd, d,
		// 				msh.av->type[d].len_variable);
		// 	}
		// 	msh.av = msh.av->next;
		// 	i++;
		// }
		// msh.av = head;
		// ft_exec(&msh, 0);
		
		// int i = 0;
		// while (msh.av)
		// {
		// 	printf("%d msh.av->data = |%s| msh.av->token = %d\n", i, msh.av->data, msh.av->token);
		// 	if (msh.av->type)
		// 	{
		// 		int d = -1;
		// 		while (++d < msh.av->dollar)
		// 			printf("msh.av->type[%d].expnd = %d msh.av->type[%d].len_variable = %d\n", d,
		// 				msh.av->type[d].expnd, d,
		// 				msh.av->type[d].len_variable);
		// 	}
		// 	msh.av = msh.av->next;
		// 	i++;
		// }
		ft_magic_malloc(FLUSH, 0, NULL, NO_ENV);
	}
}
