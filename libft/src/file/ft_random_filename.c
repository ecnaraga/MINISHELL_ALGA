/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_random_filename.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 10:17:50 by garance           #+#    #+#             */
/*   Updated: 2023/11/30 14:15:51 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

char	*ft_random_filename(char *path, int n)
{
	char	*name;
	char	*tmp;

	if (n == 0)
		return (NULL);
	name = ft_random_string(n);
	if (!name)
		return (NULL);
	tmp = name;
	name = ft_strjoin(path, name);
	free(tmp);
	if (!name)
		write(2, "read: error\n", 12);
	return (name);
}
