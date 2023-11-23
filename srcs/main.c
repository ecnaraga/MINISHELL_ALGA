/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 12:09:51 by galambey          #+#    #+#             */
/*   Updated: 2023/11/23 14:09:44 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*TO DO
- return, exit, ATTENTION AU CODE ERREURS RETOURNES*/

int status = 0;


int ft_parsing(t_msh *msh)
{
	status = ft_parse_line(msh);
	if (status != 0)
		return (ft_magic_malloc(FLUSH, 0, NULL), status);
	printf("PASSAGE ALIX TO GAGA\n");
	status = ft_parse_bis(msh);
	if (status != 0) // NE PAS MAJ EXIT STATUS
		return (ft_magic_malloc(FLUSH, 0, NULL), status);
	msh->av = ft_split_msh(msh->line); // TO DO RECUPERER EXIT STATUS DANS SPLIT
	if (!msh->av)
		return (ft_magic_malloc(FLUSH, 0, NULL), status = 128 + 6); // verifier si status 128 + 6 bien retourne
	msh->ac = ft_structtablen(msh->av);
	if (msh->ac == 0) // NE PAS MAJ EXIT STATUS
		return (ft_magic_malloc(FLUSH, 0, NULL), status);
	ft_token(msh);
	status = ft_parse_ter(msh);
	if (status != 0)
		return (ft_magic_malloc(FLUSH, 0, NULL), status);
	return (0);
}
void	ft_handle_eof(void)
{
	printf("exit\n");
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
	msh.env = env;
	while (1)
	{
		ft_signal_handler_msh();
		msh.line = ft_magic_malloc(ADD, 0, readline("Minishell$ ")); // malloc
		if (!msh.line) // EOF
			return (ft_handle_eof(), status); // RENVOYER LE DERNIER CODE ERREUR STOCKE AVANT LE CTRL D
		if (!msh.line[0])
		{
			ft_magic_malloc(FLUSH, 0, NULL);
			continue ;	
		}
		add_history(msh.line);
		if (ft_parsing(&msh) != 0)
			continue;
		
		// int i = -1;
		// while (msh.av[++i].data)
		// {
		// 	printf("msh.av[%d].data = |%s| msh.av[%d].token = %d\n", i,
		// 		msh.av[i].data, i, msh.av[i].token);
		// 	if (msh.av[i].type)
		// 	{
		// 		int d = -1;
		// 		while (++d < msh.av[i].dollar)
		// 			printf("msh.av[%d].type[%d].expnd = %d msh.av[%d].type[%d].len_variable = %d\n", i, d,
		// 				msh.av[i].type[d].expnd, i, d,
		// 				msh.av[i].type[d].len_variable);
		// 	}
		// }
		// printf("msh.av[%d].data = |%s| msh.av[%d].token = %d\n", i,
		// 	msh.av[i].data, i, msh.av[i].token);
		// if (msh.av[i].type)
		// {
		// 	int d = -1;
		// 	while (++d < msh.av[i].dollar)
		// 		printf("msh.av[%d].type[%d].expnd = %d msh.av[%d].type[%d].len_variable = %d\n", i, d,
		// 			msh.av[i].type[d].expnd, i, d,
		// 			msh.av[i].type[d].len_variable);
		// }
		
		ft_magic_malloc(PRINT, 0, NULL);
		ft_magic_malloc(FLUSH, 0, NULL);
	} 
}
