/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 14:17:15 by athiebau          #+#    #+#             */
/*   Updated: 2023/11/22 12:00:37 by athiebau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_strcat(char *dst, const char *src)
{
	size_t	i;
	size_t	len;

	i = -1;
	len = ft_strlen(dst);
	while (src[++i])
		dst[len + i] = src[i];
	dst[len + i] = '\0';
	return (dst);
}

size_t	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	int			len;
	char		*s;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		len = ft_strlen(s2);
	else if (!s2)
		len = ft_strlen(s1);
	else
		len = ft_strlen(s1) + ft_strlen(s2);
	s = (char *)malloc(sizeof(char) *(len + 1));
	if (!s)
		return (NULL);
	s[0] = '\0';
	if (s1)
		ft_strcat(s, s1);
	if (s2)
		ft_strcat(s, s2);
	return (s);
}

/*----------------------------------------------------------------------------*/

char	*get_old_pwd()
{
	char	*tmp;
	char	*old_pwd;

	tmp = getcwd(NULL, 0); //malloc
	if (!tmp)
	{
		perror("minishell:");
		return (NULL);
	}
	else
	{
		old_pwd = ft_strjoin("OLDPWD=", tmp); //malloc
		if (!old_pwd)
		{
			//free(tmp);
			perror("minishell:");
			return (NULL);
		}
		free(tmp);
		return (old_pwd);
	}
}

char	*get_path(char	**str)
{
	char	*path;

	if (str[2])
		path = str[2];
	else 
	{
		path = getenv("HOME");
		if(!path)
			return (NULL);
	}
	return (path);
}

void	builtin_cd(char **str, char **env)
{
	char	*path;
	char	*old_pwd;
	(void)env;

	old_pwd = get_old_pwd(); //malloc
	printf("old_pwd : %s\n", old_pwd);
	if (!old_pwd)
		return;
	path = get_path(str);
	printf("path : %s\n", path);
	if (!path)
	{
		free(old_pwd);
		perror("minishell:");
		return;
	}
	if (chdir(path) == 0)
		change_env(old_pwd);
	else
		//free ?
	free(old_pwd);
}

int	main(int ac, char **av, char **env)
{
	(void)ac;
		builtin_cd(av, env);
		printf("--------------------------------\n");
}