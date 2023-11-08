/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 14:40:48 by galambey          #+#    #+#             */
/*   Updated: 2023/11/08 09:39:29 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h"

/*TO DO
- return, exit, ATTENTION AU CODE ERREURS RETOURNES*/

int main(int ac, char **av, char **env)
{
	t_msh	minish;
	
	(void) av;
	(void) env;
	if (ac != 1)
	{
		write(2, "No arguments needed\n", 21);
		return (2); // CODE ERREUR A VERIFIER : Incorrect usage, such as invalid options or missing arguments
	}
	minish.env = env;
	while (1)
	{
		minish.line = readline("Minishell$ "); //malloc
		if (!minish.line)
			return (1); // CODE ERREUR A VERIFIER : One or more generic errors encountered upon exit
		if (!minish.line[0]) // a verifier aussi pas la meme chose NE PAS RETURNNNNN
		{
			free(minish.line); // exit status doit etre == 0
			continue ;	
		}
		add_history(minish.line);
		printf("minish.line %s\n", minish.line);
		/* POUR ALIX : PROBLEME DANS FT_PARSE : 
		ENVOI A MINISHELL DE LA CMD : ( bravo )
		AVANT FT_PARSE : minish.line = ( bravo )
		APRES FT_PARSE : str = ( bravo )?0
		C est comme si il n y avait plus de \0 a la fin de str >> erreur qui arrive parfois mais pas tout le temps
		*/
		// if (ft_parse_line(&minish) != 0)
		// {
		// 	free(minish.line);
		// 	continue ; //verifier si on revient au debut de la boucle
		// }
		if (ft_parse_bis(&minish) != 0)
		{
			free(minish.line);
			continue ; //verifier si on revient au debut de la boucle
		}
		minish.av = ft_split_minish(minish.line); //malloc
		free(minish.line); //free malloc main.c lg 32
		if (!minish.av)
			return (128 + 6); //6 = SIGABRT =>Verifier si signal ok
		
		int i = -1;
		while (minish.av[++i].data)
		{
			printf("minish.av[%d].data = |%s|\n", i, minish.av[i].data);
			printf("minish.av[%d].type = |%d|\n", i, minish.av[i].type);
		}
		printf("minish.av[%d].data = |%s|\n", i, minish.av[i].data);
		printf("minish.av[%d].type = |%d|\n", i, minish.av[i].type);
		
		minish.ac = ft_structtablen(minish.av); // A DAPTER A ft_split_minishell qui renvoie un tableau de struct dont la derniere data == NULL
		ft_free_split_minish(minish.av);
	} 
}