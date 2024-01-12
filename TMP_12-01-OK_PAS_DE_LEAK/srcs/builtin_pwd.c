/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 14:42:04 by athiebau          #+#    #+#             */
/*   Updated: 2024/01/11 12:44:49 by athiebau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	builtin_pwd(t_msh *msh)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		ft_putstr_fd("pwd: error retrieving current directory: ", 2);
		ft_putstr_fd("getcwd: cannot access parent directories: ", 2);
		perror(NULL);
		return (msh->status = 1);
	}
	printf("%s\n", pwd);
	free(pwd);
	return (0);
}
