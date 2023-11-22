/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 12:09:51 by galambey          #+#    #+#             */
/*   Updated: 2023/11/22 16:37:49 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*TO DO
- return, exit, ATTENTION AU CODE ERREURS RETOURNES*/

void	ft_free(int signal)
{
	if (signal == SIGINT)
	{
		ft_magic_malloc(FLUSH, 0, NULL);
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}
/*SIGNAUX :
	Faire une fonction pour les signaux dans les cas classiques, une fonction pour les signaux dans le here_doc et une fonction pour les signaux dans les commandes (cat, grep, ...)
	Changer la variable globale et la mettre a 130 au moindre CTRL + C !!!
	Cas impossible a traiter (on a pas trouve la solution): lorsqu'on fait un Ctrl + C dans un heredoc puis un nouveau Ctrl + C dans le prompt cela est traite comme deux signaux et il y a un saut de ligne
	Ctrl + D n'est pas vraiment un signal c'est simplement un EOF donc pas besoin de le traiter en tant que signal
	Tester les signaux avec cat, grep, sleep...
	Ctrl + \ est surtout a traiter en cas de commande bloquante et se comporte presque comme Ctrl + C dans ces cas la mais avec un code erreur et un syntaxe differente
*/

int main(int ac, char **av, char **env)
{
	t_msh minish;

	(void)av;
	(void)env;
	if (isatty(0) == 1) //A REGLER test ./minishell | ./minishell
	{
		printf("stdin : %d\n", isatty(0));
		printf("stdout : %d\n", isatty(1));
	}
	if (ac != 1)
	{
		write(2, "No arguments needed\n", 21);
		return (2); // CODE ERREUR A VERIFIER : Incorrect usage,
					// such as invalid options or missing arguments
	}
	minish.env = env;
	// signal (SIGQUIT, &ft_free);
	while (1)
	{
		if(signal (SIGINT, &ft_free) == SIG_ERR)
			printf("ctrl c marche pas\n");
		//if (signal (SIGQUIT, &ft_free) == -1);
		minish.line = readline("Minishell$ "); // malloc
		minish.line = ft_magic_malloc(ADD, 0, minish.line); // malloc
		if (!minish.line)
		{
			printf("exit\n");
			ft_magic_malloc(FLUSH, 0, NULL);
			return (1);
			// RENVOYER LE DERNIER CODE ERREUR STOCKE AVANT LE CTRL D
			// EOF
		}
		if (!minish.line[0])
		// a verifier aussi pas la meme chose NE PAS RETURNNNNN
		{
			// free(minish.line); // exit status doit etre == 0
			ft_magic_malloc(FLUSH, 0, NULL);
			// minish.line = NULL;
			continue ;	
		}
		add_history(minish.line);
		if (ft_parse_line(&minish) != 0)
		{
			ft_magic_malloc(FLUSH, 0, NULL);
			continue ;
		}
		printf("PASSAGE ALIX TO GAGA\n");
		if (ft_parse_bis(&minish) != 0)
		{
			ft_magic_malloc(FLUSH, 0, NULL);
			continue ;
		}
		minish.av = ft_split_msh(minish.line); //malloc
		if (!minish.av)
		{
			ft_magic_malloc(FLUSH, 0, NULL);
			return (128 + 6); //6 = SIGABRT =>Verifier si signal ok
		}
		minish.ac = ft_structtablen(minish.av);
		if (minish.ac == 0)
		{
			ft_magic_malloc(FLUSH, 0, NULL);
			continue ;
		}
		ft_token(&minish);
		if (ft_parse_ter(&minish) != 0)
		{
			ft_magic_malloc(FLUSH, 0, NULL);
			continue ;
		}
		
		// int i = -1;
		// while (minish.av[++i].data)
		// {
		// 	printf("minish.av[%d].data = |%s| minish.av[%d].token = %d\n", i,
		// 		minish.av[i].data, i, minish.av[i].token);
		// 	if (minish.av[i].type)
		// 	{
		// 		int d = -1;
		// 		while (++d < minish.av[i].dollar)
		// 			printf("minish.av[%d].type[%d].expnd = %d minish.av[%d].type[%d].len_variable = %d\n", i, d,
		// 				minish.av[i].type[d].expnd, i, d,
		// 				minish.av[i].type[d].len_variable);
		// 	}
		// }
		// printf("minish.av[%d].data = |%s| minish.av[%d].token = %d\n", i,
		// 	minish.av[i].data, i, minish.av[i].token);
		// if (minish.av[i].type)
		// {
		// 	int d = -1;
		// 	while (++d < minish.av[i].dollar)
		// 		printf("minish.av[%d].type[%d].expnd = %d minish.av[%d].type[%d].len_variable = %d\n", i, d,
		// 			minish.av[i].type[d].expnd, i, d,
		// 			minish.av[i].type[d].len_variable);
		// }
		
		ft_magic_malloc(PRINT, 0, NULL);
		ft_magic_malloc(FLUSH, 0, NULL);
	} 
}
