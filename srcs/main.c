/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garance <garance@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 12:09:51 by galambey          #+#    #+#             */
/*   Updated: 2023/11/25 14:18:08 by garance          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minishell.h"

/*TO DO
- return, exit, ATTENTION AU CODE ERREURS RETOURNES*/

int status = 0;

int ft_parsing(t_msh *msh)
{
	if (ft_parse_line(msh) != 0) // POUR ALIX : PB INVALID READ
		return (ft_magic_malloc(FLUSH, 0, NULL), 1);
	printf("PASSAGE ALIX TO GAGA\n");
	if (ft_parse_bis(msh) != 0)
		return (ft_magic_malloc(FLUSH, 0, NULL), 1);
	msh->av = ft_split_msh(msh->line);
	if (!msh->av)
		return (ft_magic_malloc(FLUSH, 0, NULL), 1);
	msh->ac = ft_structtablen(msh->av);
	if (msh->ac == 0)
		return (ft_magic_malloc(FLUSH, 0, NULL), 1);
	ft_token(msh);
	if (ft_parse_ter(msh) != 0)
		return (ft_magic_malloc(FLUSH, 0, NULL), 1);
	return (0);
}

/*
In case of Ctrl-D, free all and quit the program
*/
void	ft_handle_eof(void)
{
	ft_putstr_fd("exit\n", 2);
	ft_magic_malloc(FLUSH, 0, NULL);
	rl_clear_history();
}

int main(int ac, char **av, char **env)
{
	t_msh msh;
	
	(void)av;
	if (isatty(0) == 1) //A REGLER test ./minishell | ./minishell
	{
		printf("stdin : %d\n", isatty(0));
		printf("stdout : %d\n", isatty(1));
	}
	if (ac != 1)
		return (write(2, "bash: minishell: too many arguments\n", 37), 1); // si cd avec 2 arguments meme message d erreur et exit status 1
	msh.env = get_env(env); // PB EST FLUSH AVEC LES AUTRES il faudrait aJOUTER UNE REGLE A FT-magic-malloc
	while (1)
	{
		ft_signal_handler_msh();
		msh.line = readline("Minishell$ ");
		msh.previous_status = status;
		status = 0;
		if (!msh.line) // EOF
			return (ft_handle_eof(), status); // RENVOYER LE DERNIER CODE ERREUR STOCKE AVANT LE CTRL D
		if (!ft_magic_malloc(ADD, 0, msh.line))
		{
			ft_magic_malloc(FLUSH, 0, NULL);
			continue ;
		}	
		if (!msh.line[0])
		{
			ft_magic_malloc(FLUSH, 0, NULL);
			continue ;	
		}
		add_history(msh.line);
		if (ft_parsing(&msh) != 0)
			continue;
		
		int i = -1;
		while (msh.av[++i].data)
		{
			printf("msh.av[%d].data = |%s| msh.av[%d].token = %d\n", i,
				msh.av[i].data, i, msh.av[i].token);
			if (msh.av[i].type)
			{
				int d = -1;
				while (++d < msh.av[i].dollar)
					printf("msh.av[%d].type[%d].expnd = %d msh.av[%d].type[%d].len_variable = %d\n", i, d,
						msh.av[i].type[d].expnd, i, d,
						msh.av[i].type[d].len_variable);
			}
		}
		printf("msh.av[%d].data = |%s| msh.av[%d].token = %d\n", i,
			msh.av[i].data, i, msh.av[i].token);
		if (msh.av[i].type)
		{
			int d = -1;
			while (++d < msh.av[i].dollar)
				printf("msh.av[%d].type[%d].expnd = %d msh.av[%d].type[%d].len_variable = %d\n", i, d,
					msh.av[i].type[d].expnd, i, d,
					msh.av[i].type[d].len_variable);
		}
		
		ft_magic_malloc(PRINT, 0, NULL);
		ft_magic_malloc(FLUSH, 0, NULL);
	} 
}
