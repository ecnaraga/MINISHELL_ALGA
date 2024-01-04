/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 15:35:53 by athiebau          #+#    #+#             */
/*   Updated: 2024/01/04 16:54:26 by athiebau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>

char	*ft_strjoin3(t_msh *msh, char *s1, char *s2)
{
	int			len;
	char		*s;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
	{
		s1 = mcgic(mlcp(NULL, sizeof(char)), MLC, PIP, msh);
		s1[0] = '\0';
		len = ft_strlen(s2);
	}
	else if (!s2)
		len = ft_strlen(s1);
	else
		len = ft_strlen(s1) + ft_strlen(s2);
	s = mcgic(mlcp(NULL, sizeof(char) * (len + 2)), MLC, PIP, msh);
	if (!s)
		return (NULL);
	s[0] = '\0';
	if (s1)
		ft_strcat(s, s1);
	if (s2)
		ft_strcat(s, s2);
	s[len++] = '\t';
	s[len] = '\0';
	mcgic(mlcp(s1, 0), FREE, PIP, msh);
	return (s);
}

int	matchWildcard(char *str, char *pattern, int *tab) 
{
	if ((*str == '\0' && *pattern == '\0') || (*pattern == '*' && *tab == 0 && *(pattern + 1) == '\0'))
		return (1);
		
	if (*pattern == '\0' || *str == '\0')
		return (0);
		
	if (*pattern == '*' && *tab == 0)
	{
		while(*(pattern + 1) == '*' && *(tab + 1) == 0)
		{
			pattern++;
			tab++;
		}
		if (!matchWildcard(str, pattern + 1, tab + 1))
			return (matchWildcard(str + 1, pattern, tab));
		else
			return (1);
	}
		
	if (*str == *pattern)
		return matchWildcard(str + 1, pattern + 1, tab + 1);
		
	return (0);
}

int	ft_strcmp_cas(char *s1, char *s2)
{
	size_t	i;
	char	a;
	char 	b;

	i = 0;
	if (!s1[0] || !s2[0])
		return (1);
	while (s1[i] != '\0' || s2[i] != '\0')
	{
		a = ft_tolower(s1[i]);
		b = ft_tolower(s2[i]);
		if (a != b)
			return (a - b);
		i++;
	}
	return (ft_tolower(s1[i]) - ft_tolower(s2[i]));
}

char	**make_in_order(char **str)
{
	char	*tmp;
	int	i;
	
	i = 0;
	while(str[i])
	{
		if (str[i + 1] && ft_strcmp_cas(str[i], str[i + 1]) > 0)
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

char	**wildcards(char *str, t_msh *msh, char *cmd_0)
{
	DIR				*dir;
	struct dirent	*read;
	char	*str2;
	char	**str3;

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
		if (matchWildcard(read->d_name, str, msh->av->wild) == 1)
		{
			if((ft_strcmp("ls", cmd_0) != 0 && !(read->d_name[0] == '.')) || ft_strcmp("ls", cmd_0) == 0)
			{
				str2 = ft_strjoin3(msh, str2, read->d_name);
				if (msh && msh->status == 255)
					return (NULL);
			}
			// if((ft_strcmp("echo", cmd_0) == 0 && !(read->d_name[0] == '.')) || ft_strcmp("echo", cmd_0) != 0)
			// {
			// 	str2 = ft_strjoin3(msh, str2, read->d_name);
			// 	if (msh && msh->status == 255)
			// 		return (NULL);
			// }
		}
		read = readdir(dir);
	}
	closedir(dir);
	str3 = ft_split_magic_malloc(msh, 1, str2, '\t');
	if (!str3)
		return (NULL);
	mcgic(mlcp(str2, 0), FREE, PIP, msh);
	if(str3[1])
		str3 = make_in_order(str3);
	return (str3);
}
