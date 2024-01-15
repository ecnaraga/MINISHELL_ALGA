/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 15:35:53 by athiebau          #+#    #+#             */
/*   Updated: 2024/01/15 11:35:51 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	match_wildcard(char *str, char *pattern, int *tab)
{
	if ((*str == '\0' && *pattern == '\0') || (*pattern == '*' && *tab == 0
			&& *(pattern + 1) == '\0'))
		return (1);
	if (*pattern == '\0' || *str == '\0')
		return (0);
	if (*pattern == '*' && *tab == 0)
	{
		while (*(pattern + 1) == '*' && *(tab + 1) == 0)
		{
			pattern++;
			tab++;
		}
		if (!match_wildcard(str, pattern + 1, tab + 1))
			return (match_wildcard(str + 1, pattern, tab));
		else
			return (1);
	}
	if (*str == *pattern)
		return (match_wildcard(str + 1, pattern + 1, tab + 1));
	return (0);
}

int	ft_strcmp_cas(char *s1, char *s2)
{
	size_t	i;
	char	a;
	char	b;

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

static int	wildcards_begin(DIR **dir, struct dirent **read)
{
	*dir = opendir(".");
	if (!(*dir))
	{
		perror("minishell");
		return (1);
	}
	*read = readdir((*dir));
	if (!(*read))
	{
		perror("minishell");
		return (1);
	}
	return (0);
}

static char	*wildcard_end(struct dirent *read, t_wildcard *w, t_msh *msh,
		DIR *dir)
{
	char	*str;

	str = NULL;
	while (read != NULL)
	{
		if (match_wildcard(read->d_name, w->pattern, msh->av->wild) == 1)
		{
			if (!(read->d_name[0] == '.') || (ft_strcmp("ls", w->cmd_0) == 0
					&& is_flag_a(w->cmd_1) == 0 && read->d_type != 4)
				|| (w->pattern[0] == '.' && (w->pattern[1] == '*')))
			{
				str = ft_strjoin3(msh, str, read->d_name);
				if (msh && msh->status == 255)
					return (NULL);
			}
		}
		read = readdir(dir);
	}
	return (str);
}

char	**wildcards(char *pattern, t_msh *msh, char *cmd_0, char *cmd_1)
{
	DIR				*dir;
	struct dirent	*read;
	char			**str3;
	t_wildcard		w;
	char			*str2;

	w.cmd_0 = cmd_0;
	w.cmd_1 = cmd_1;
	w.pattern = pattern;
	if (wildcards_begin(&dir, &read) != 0)
		return (NULL);
	str2 = wildcard_end(read, &w, msh, dir);
	closedir(dir);
	str3 = ft_split_magic_malloc(msh, 1, str2, '\t');
	if (!str3)
		return (NULL);
	mcgic(mlcp(str2, 0), FREE, PIP, msh);
	if (str3[1])
		str3 = make_in_order(str3);
	return (str3);
}
