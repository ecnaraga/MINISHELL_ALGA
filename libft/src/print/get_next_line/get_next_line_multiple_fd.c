/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_multiple_fd.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 18:35:18 by garance           #+#    #+#             */
/*   Updated: 2023/06/23 16:36:25 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/libft.h"

static char	*ft_new_line(char *line, char *buffer, int i)
{
	char	*temp;

	temp = NULL;
	if (line)
	{
		temp = line;
		line = ft_strjoin_gnl(temp, buffer, i);
		free (temp);
	}
	else
		line = ft_strjoin_gnl(line, buffer, i);
	return (line);
}

static char	*ft_read_fd(char *save, char *line, int fd)
{
	char	buffer[BUFFER_SIZE + 1];
	int		count;
	int		i;

	count = -1;
	i = -1;
	ft_bzero(save, BUFFER_SIZE + 1);
	while (i < 0 && count != 0)
	{
		ft_bzero(buffer, BUFFER_SIZE + 1);
		count = read(fd, buffer, BUFFER_SIZE);
		if (count == -1)
			return (NULL);
		i = ft_is_char(buffer, '\n');
		if (!line && count == 0)
			return (NULL);
		line = ft_new_line(line, buffer, i);
		if (!line)
			return (NULL);
		if (i >= 0 && i < (int) ft_strlen_gnl(buffer))
			ft_strcpy_gnl(save, buffer + i + 1);
	}
	return (line);
}

char	*get_next_line_multiple_fd(int fd)
{
	char			*line;
	static char		save[1024][BUFFER_SIZE + 1];

	if (BUFFER_SIZE <= 0 || BUFFER_SIZE > 1000000 || fd < 0)
		return (NULL);
	line = NULL;
	if (save[fd][0] && ft_is_char(save[fd], '\n') >= 0)
	{
		line = ft_strjoin_gnl(line, save[fd], ft_is_char(save[fd], '\n'));
		ft_strcpy_gnl(save[fd], save[fd] + ft_is_char(save[fd], '\n') + 1);
		return (line);
	}
	if (!save[fd][0])
	{
		ft_bzero(save[fd], BUFFER_SIZE + 1);
		return (ft_read_fd(save[fd], line, fd));
	}
	if (save[fd][0] && ft_is_char(save[fd], '\n') == -1)
	{
		line = ft_strjoin_gnl(line, save[fd], -1);
		if (!line)
			return (NULL);
		return (ft_read_fd(save[fd], line, fd));
	}
	return (NULL);
}
