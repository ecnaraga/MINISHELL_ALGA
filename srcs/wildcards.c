/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 15:35:53 by athiebau          #+#    #+#             */
/*   Updated: 2023/12/20 15:25:51 by athiebau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "../includes/minishell.h"
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>

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

char	**wildcards(char *str)
{
	DIR				*dir;
	struct dirent	*read;

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
		//printf("file : %s | pattern : %s\n", read->d_name, str);
		if (matchWildcard(read->d_name, str) == 1)
			printf("win : %s\n", read->d_name);
		read = readdir(dir);
	}
	closedir(dir);
	return (NULL);
}

int	main(int ac, char **av)
{
	wildcards(av[1]);
	return (0);
}