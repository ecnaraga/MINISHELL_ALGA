/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 15:35:53 by athiebau          #+#    #+#             */
/*   Updated: 2023/12/21 17:14:52 by athiebau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>

char	*ft_strjoin3(char *s1, char *s2)
{
	int			len;
	char		*s;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
	{
		s1 = ft_calloc(1, 1);
		len = ft_strlen(s2);
	}
	else if (!s2)
		len = ft_strlen(s1);
	else
		len = ft_strlen(s1) + ft_strlen(s2);
	s = (char *)malloc(sizeof(char) * (len + 2));
	if (!s)
		return (NULL);
	s[0] = '\0';
	if (s1)
		ft_strcat(s, s1);
	if (s2)
		ft_strcat(s, s2);
	s[len++] = ' ';
	s[len] = '\0';
	free(s1);
	return (s);
}

int	matchWildcard(char *str, char *pattern) 
{
	if ((*str == '\0' && *pattern == '\0') || (*pattern == '*' && *(pattern + 1) == '\0'))
		return (1);
		
	if (*pattern == '\0' || *str == '\0')
		return (0);
		
	if (*pattern == '*')
	{
		while(*(pattern + 1) == '*')
			pattern++;
		if (!matchWildcard(str, pattern + 1))
			return (matchWildcard(str + 1, pattern));
		else
			return (1);
	}
		
	if (*str == *pattern)
		return matchWildcard(str + 1, pattern + 1);
		
	return (0);
}

char	**make_in_order(char **str)
{
	char	*tmp;
	int	i;
	
	i = 0;
	while(str[i])
	{
		if (str[i + 1] && ft_strcmp(str[i], str[i + 1]) > 0)
		{
			tmp = str[i];
			str[i] = str[i + 1];
			str[i + 1] = tmp;
			i = 0;
		}
		else
			i++;
	}
	return (str);
}

char	**wildcards(char *str, t_msh *minish)
{
	DIR				*dir;
	struct dirent	*read;
	char	*str2;
	char	**str3;
	(void)minish;

	str2 = NULL;
	dir = opendir(".");
	if (!dir)
	{
		perror("minishell");
		return (NULL);
	}
	read = readdir(dir);
	if (!read)
	{
		perror("minishell");
		return (NULL);
	}
	while (read != NULL)
	{
		if (matchWildcard(read->d_name, str) == 1)
			str2 = ft_strjoin3(str2, read->d_name);
		read = readdir(dir);
	}
	closedir(dir);
	str3 = ft_split(str2, ' ');
	free(str2);
	str3 = make_in_order(str3);	
	return (str3);
}

// int	main(int ac, char **av)
// {
// 	wildcards(av[1], NULL);
// 	return (0);
// }