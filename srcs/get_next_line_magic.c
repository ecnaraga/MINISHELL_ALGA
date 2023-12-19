/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_magic.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 18:35:18 by garance           #+#    #+#             */
/*   Updated: 2023/12/19 15:38:41 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*ft_strjoin_gnl_magic(char const *s1, char const *s2, int i)
{
	int			len;
	char		*s;

	if (!s1 && i == -1)
		len = ft_strlen_gnl(s2);
	else if (!s1)
		len = i + 1;
	else if (!s2)
		len = ft_strlen_gnl(s1);
	else if (i == -1)
		len = ft_strlen_gnl(s1) + ft_strlen_gnl(s2);
	else
		len = ft_strlen_gnl(s1) + i + 1;
	s = ft_magic_malloc(MALLOC, (sizeof(char) *(len + 1)), NULL, NO_ENV);
	if (!s)
		return (NULL);
	s[0] = '\0';
	if (s1)
		ft_strlcat(s, s1, ft_strlen_gnl(s1) + 1);
	if (s2)
		ft_strlcat(s, s2, len + 1);
	return (s);
}

static char	*ft_new_line(char *line, char *buffer, int i)
{
	char	*temp;

	temp = NULL;
	if (line)
	{
		temp = line;
		line = ft_strjoin_gnl_magic(temp, buffer, i);
		free (temp);
	}
	else
		line = ft_strjoin_gnl_magic(line, buffer, i);
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

char	*get_next_line_magic(int fd)
{
	char			*line;
	static char		save[BUFFER_SIZE + 1];

	if (BUFFER_SIZE <= 0 || BUFFER_SIZE > 1000000 || fd < 0)
		return (NULL);
	line = NULL;
	if (save[0] && ft_is_char(save, '\n') >= 0)
	{
		line = ft_strjoin_gnl_magic(line, save, ft_is_char(save, '\n'));
		ft_strcpy_gnl(save, save + ft_is_char(save, '\n') + 1);
		return (line);
	}
	if (!save[0])
	{
		ft_bzero(save, BUFFER_SIZE + 1);
		return (ft_read_fd(save, line, fd));
	}
	if (save[0] && ft_is_char(save, '\n') == -1)
	{
		line = ft_strjoin_gnl_magic(line, save, -1);
		if (!line)
			return (NULL);
		return (ft_read_fd(save, line, fd));
	}
	return (NULL);
}
