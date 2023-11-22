/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 14:42:04 by athiebau          #+#    #+#             */
/*   Updated: 2023/11/22 14:46:20 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	builtin_pwd(void)
{
	char	*str;

	str = ft_magic_malloc(ADD, 0, getcwd(NULL, 0));
	printf("%s\n", str);
	ft_magic_malloc(FREE, 0, str);
}