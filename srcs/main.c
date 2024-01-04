/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 12:09:51 by galambey          #+#    #+#             */
/*   Updated: 2024/01/04 11:29:04 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minishell.h"

/*TO DO
- return, exit, ATTENTION AU CODE ERREURS RETOURNES*/

// int status = 0;
int sign = 0;

/*
Call all the parsing function + split the line + tokenisation
*/
static int ft_parsing(t_msh *msh, int sub)
{
	if (sub == 0 && ft_parse_line(msh) != 0) // OK PROTEGE ET SI MALLOC KO ON QUITTE A L INTERIEUR
		return (mlcgic(NULL, FLUSH, NO_ENV, msh), 1);
	if (sub == 0 && ft_parse_bis(msh) != 0) // OK PROTEGE ET SI MALLOC KO ON QUITTE A L INTERIEUR
		return (mlcgic(NULL, FLUSH, NO_ENV, msh), 1);
	msh->av = ft_split_msh(msh->line, msh); // OK PROTEGE ET SI MALLOC KO ON QUITTE A L INTERIEUR
	if (!msh->av)
		return (mlcgic(NULL, FLUSH, NO_ENV, msh), 1);

	// int i = 0;
	// t_split *head;
	// head = msh->av;
	// while (msh->av)
	// {
	// 	printf("%d msh->av->data = |%s|\n", i, msh->av->data);
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
			
	msh->ac = ft_lstsize_split(msh->av);
	if (msh->ac == 0)
		return (mlcgic(NULL, FLUSH, NO_ENV, msh), 1);
	ft_token(msh);
	if (sub == 0 && ft_parse_ter(msh) != 0) // OK PROTEGE ET SI MALLOC KO ON QUITTE A L INTERIEUR
		return (mlcgic(NULL, FLUSH, NO_ENV, msh), 1);
	return (0);
}

void	ft_minishell(t_msh *msh, int sub, t_fdpar *fd)
{
	printf("FT_MINISHELL *************** msh->line |%s|\n", msh->line);
	//dprintf(2, "DFT_MINISHELL *************** msh->line |%s|\n", msh->line);
	if (ft_parsing(msh, sub) != 0)  // OK PROTEGE ET SI MALLOC KO ON QUITTE A L INTERIEUR
		return ;
	if (ft_exec(msh, sub, fd) != 0) // OK GERE ET PROTEGE A l interieur
		return ;
	if (sub == 1)
		ft_exit(-1, -1, -1, msh);
}

/*
In case of Ctrl-D, free all and quit the program
*/
static void	ft_handle_eof(t_msh *msh)
{
	ft_putstr_fd("exit\n", 2);
	rl_clear_history();
	mlcgic(NULL, QUIT, 0, msh);
}

static int	ft_readline(t_msh *msh)
{
	msh->line = readline("Minishell$ "); // PROTEGE OK
	if (sign == 2)
	{
		msh->status = 130;
		mlcgic(NULL, FLUSH, NO_ENV, msh);
		mlcgic(NULL, FLUSH, PIP, msh);
		sign = 0;
	}
	if (sign == 1)
	{
		msh->status = 130;
		sign = 3;
	}
	msh->previous_status = msh->status;
	msh->status = 0;
	if (!msh->line) // EOF
		(ft_handle_eof(msh), exit(msh->previous_status));
	if (!mlcgic(mlcp(msh->line, 1), ADD, NO_ENV, msh))
		(mlcgic(NULL, QUIT, 0, msh), exit(msh->status)); // SI MALLOC KO ON QUITTE
	if (!msh->line[0])
		return (mlcgic(NULL, FLUSH, NO_ENV, msh), -1);	
	return (0);
}
/*
if isatty(0) == 1, in case of the STDOUT_FILENO has already been redirected,
	we open /dev/stdin tu dup2 , to be sure the STDOUT_FILENO of our
	minishell will be the terminal
*/
int main(int ac, char **av, char **env)
{
	t_msh msh;
	int fd;
	
	(void)av;
	if (isatty(0) == 1)
	{
		fd = open("/dev/stdin", O_RDWR);
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	if (isatty(0) == 0)
		exit (0);
	if (ac != 1)
		return (write(2, "bash: minishell: too many arguments\n", 37), 1); // si cd avec 2 arguments meme message d erreur et exit status 1
	msh.status = 0;
	msh.env = get_env(env, &msh);
	msh.export_env = get_export_env(&msh, env);
	msh.status = 0;
	msh.previous_status = 0;
	while (1)
	{
		ft_signal_handler_msh();
		if (ft_readline(&msh) == -1)
			continue;
		add_history(msh.line);
		ft_minishell(&msh, 0, NULL);
		mlcgic(NULL, FLUSH, NO_ENV, &msh);
	}
}
