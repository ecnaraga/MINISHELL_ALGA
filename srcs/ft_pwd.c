/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 14:42:04 by athiebau          #+#    #+#             */
/*   Updated: 2023/11/20 14:52:39 by athiebau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	builtin_pwd(void)
{
	char	*str;

	str = getcwd(NULL, 0);
	printf("%s\n", str);
	free(str);
}