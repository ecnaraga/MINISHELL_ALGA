/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 13:15:29 by galambey          #+#    #+#             */
/*   Updated: 2024/01/10 17:07:11 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
Retourne 0 si option -n, sinon retourne 0
	- -n
	- "-n"
	- -nnnnnnnnnnnnnnnn
	- "-nnnnn"
	- '-nn'
	- -"nnnnnnnnnnn"
	- -"nn"n"nnnnnnnn"
	- "-"nn'n'nnnnnnnn
	Ces cas sont normalement gere par ft_split_minish qui a retire les ""
	ou ' ' "non imprimables" et a indique ARG dans le type de la struct
Par contre : "-""nn'n'nnnnnnnn" n est pas considiere comme l option -n mais
	dans ce cas le parsing affiche les single quote donc normalement deja gere
Elle peut avoir 1 ou plusieurs n mais 1 seul tiret
*/
int	ft_option_echo(char *data)
{
	int	i;

	i = 0;
	if (data[0] != '-')
		return (1);
	while (data[++i])
	{
		if (data[i] != 'n')
			return (1);
	}
	return (0);
}

/*
L'option -n peut etre entre quote (single OU double) ou non , ex :
	- -n
	- "-n"
	- -nnnnnnnnnnnnnnnn
	- "-nnnnn"
	- '-nn'
	- -"nnnnnnnnnnn"
	- -"nn"n"nnnnnnnn"
	- "-"nn'n'nnnnnnnn
	Ces cas sont normalement gere par ft_split_minish qui a retire les ""
	ou ' ' "non imprimables" et a indique ARG dans le type de la struct
Par contre : "-""nn'n'nnnnnnnn" n est pas considiere comme l option -n mais
	dans ce cas le parsing affiche les single quote donc normalement deja gere
Elle peut avoir 1 ou plusieurs n mais 1 seul tiret
*/
void	builtin_echo(t_msh *msh)
{
	int	i;
	int	new_line;

	i = 1;
	new_line = 0;
	while (msh->p.cmd_t[i] && ft_option_echo(msh->p.cmd_t[i]) == 0)
	{
		new_line = 1;
		i++;
	}
	while (msh->p.cmd_t[i])
	{
		ft_putstr_fd(msh->p.cmd_t[i], 1);
		i++;
		if (msh->p.cmd_t[i])
			write(1, " ", 1);
	}
	if (new_line != 1)
		write(1, "\n", 1);
}
