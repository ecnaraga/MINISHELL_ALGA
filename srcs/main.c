/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 12:09:51 by galambey          #+#    #+#             */
/*   Updated: 2023/11/30 10:53:08 by athiebau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minishell.h"

/*TO DO
- return, exit, ATTENTION AU CODE ERREURS RETOURNES*/

int status = 0;

/*
Call all the parsing function + split the line + tokenisation
*/
int ft_parsing(t_msh *msh)
{
	if (ft_parse_line(msh) != 0) // POUR ALIX : PB INVALID READ
		return (ft_magic_malloc(FLUSH, 0, NULL, 0), 1);
	printf("PASSAGE ALIX TO GAGA\n");
	if (ft_parse_bis(msh) != 0)
		return (ft_magic_malloc(FLUSH, 0, NULL, 0), 1);
	msh->av = ft_split_msh(msh->line);
	if (!msh->av)
		return (ft_magic_malloc(FLUSH, 0, NULL, 0), 1);
	msh->ac = ft_lstsize_split(msh->av);
	if (msh->ac == 0)
		return (ft_magic_malloc(FLUSH, 0, NULL, 0), 1);
	ft_token(msh);
	if (ft_parse_ter(msh) != 0)
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

int main(int ac, char **av, char **env)
{
	t_msh msh;
	
	(void)av;
	/*if (isatty(0) == 1) //A REGLER test ./minishell | ./minishell
	{
		printf("stdin : %d\n", isatty(0));
		printf("stdout : %d\n", isatty(1));
	}*/
	if (ac != 1)
		return (write(2, "bash: minishell: too many arguments\n", 37), 1); // si cd avec 2 arguments meme message d erreur et exit status 1
	for (size_t i = 0; env[i]; i++)
	{
		printf("%s\n", env[i]);
	}
	printf("-----------------------------------------------------------------\n");
	msh.env = get_env(env);
	msh.export_env = get_export_env(env);
	
	t_env *new = (*msh.export_env);
	while(new)
	{
		printf("%s\n", new->name);
		new = new->next;
	}
	
	while (1)
	{
		ft_signal_handler_msh();
		msh.line = readline("Minishell$ ");
		msh.previous_status = status;
		status = 0;
		/*if(strcmp(msh.line, "pwd") == 0)
			builtin_pwd(&msh);
		if(strcmp(msh.line, "cd") == 0)
			builtin_cd(&msh);*/
		if (!msh.line) // EOF
			return (ft_handle_eof(), status); // RENVOYER LE DERNIER CODE ERREUR STOCKE AVANT LE CTRL D
		if (!ft_magic_malloc(ADD, 0, msh.line, NO_ENV))
		{
			ft_magic_malloc(FLUSH, 0, NULL, 0);
			continue ;
		}	
		if (!msh.line[0])
		{
			ft_magic_malloc(FLUSH, 0, NULL, 0);
			continue ;	
		}
		add_history(msh.line);
		if (ft_parsing(&msh) != 0)
			continue;
		
		int i = 0;
		while (msh.av)
		{
			printf("%d msh.av->data = |%s| msh.av->token = %d\n", i, msh.av->data, msh.av->token);
			if (msh.av->type)
			{
				int d = -1;
				while (++d < msh.av->dollar)
					printf("msh.av->type[%d].expnd = %d msh.av->type[%d].len_variable = %d\n", d,
						msh.av->type[d].expnd, d,
						msh.av->type[d].len_variable);
			}
			msh.av = msh.av->next;
			i++;
		}
		ft_magic_malloc(FLUSH, 0, NULL, 0);
	}
}
