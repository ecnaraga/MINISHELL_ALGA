/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_random_string.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garance <garance@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 10:30:55 by garance           #+#    #+#             */
/*   Updated: 2023/09/19 10:32:59 by garance          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

char	*ft_random_string(int n)
{
	char	*str;
	int		fd;

	str = (char *)malloc(sizeof(char) * n + 1);
	if (!str)
		return (NULL);
	fd = open("/dev/urandom", O_RDONLY, 0744);
	if (fd == -1)
	{
		(perror("/dev/urandom"), free(str));
		return (NULL);
	}
	if (read(fd, str, n) == -1)
	{
		(perror("read"), close(fd), free(str));
		return (NULL);
	}
	close(fd);
	str[n] = '\0';
	return (str);
}
