/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 14:42:04 by athiebau          #+#    #+#             */
/*   Updated: 2023/11/22 16:45:15 by athiebau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	builtin_pwd(char **env)
{
	int	i;

	i = -1;
	while (env[++i])
	{
		if (!ft_strncmp(env[i], "PWD=", 4))
		{
			printf("%s\n", env[i] + 4);
			break ;
		}
	}
}
/*cc srcs/ft_pwd.c libft/src/string/ft_strncmp.c*/

// int main(int ac, char **av, char **env)
// {
// 	(void)ac;
// 	(void)av;
// 	builtin_pwd(env);
// 	return 0;
// }
