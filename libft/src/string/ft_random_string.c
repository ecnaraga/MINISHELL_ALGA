/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_random_string.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 10:30:55 by garance           #+#    #+#             */
/*   Updated: 2024/01/10 18:42:43 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

int	ft_select_char(int fd)
{
	char	buf[1];

	buf[0] = '\0';
	while (ft_isalnum(buf[0]) == 0)
	{
		if (read(fd, buf, 1) == -1)
		{
			(perror("read"), close(fd));
			return (-1);
		}
	}
	return ((int)buf[0]);
}

/*Create a random alphanumeric string */
char	*ft_random_string(int n)
{
	char	*str;
	int		c;
	int		fd;
	int		i;

	str = (char *)malloc(sizeof(char) * n + 1);
	if (!str)
		return (NULL);
	fd = open("/dev/urandom", O_RDONLY, 0744);
	if (fd == -1)
	{
		(perror("/dev/urandom"), free(str));
		return (NULL);
	}
	i = -1;
	while (++i < n)
	{
		c = ft_select_char(fd);
		if (c == -1)
			return (free(str), NULL);
		str[i] = c;
	}
	close(fd);
	str[n] = '\0';
	return (str);
}
