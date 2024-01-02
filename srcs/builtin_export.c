/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garance <garance@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 14:13:14 by athiebau          #+#    #+#             */
/*   Updated: 2024/01/02 11:06:19 by garance          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

size_t	ft_strlen2(const char *s)
{
	size_t	i;

	i = 0;
	if (*s == '=')
		s++;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strjoin2(char const *s1, char const *s2)
{
	int			len;
	char		*s;

	if (!s1 && !s2)
		return (NULL);
	if (*s2 == '=')
		s2++;
	if (!s1)
		len = ft_strlen2(s2);
	else if (!s2)
		len = ft_strlen(s1);
	else
		len = ft_strlen(s1) + ft_strlen2(s2);
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

static char	*ft_exstrjoin(t_msh *msh, char const *s1, char const *s2)
{
	int		len;
	char	*s;

	if (!s1 && !s2)
		return (NULL);
	if (*s2 == '=')
		s2++;
	if (!s1)
		len = ft_strlen2(s2);
	else if (!s2)
		len = ft_strlen(s1);
	else
		len = ft_strlen(s1) + ft_strlen(s2);
	s = mlcgic(mlcp(NULL, sizeof(char) * (len + 1)), MALLOC, ENV, msh);
	// s = ft_magic_malloc(MALLOC, sizeof(char) * (len + 1), NULL, ENV);
	if (!s)
		return (NULL);
	s[0] = '\0';
	if (s1)
		ft_strlcat(s, s1, ft_strlen(s1));
	if (s2)
		ft_strcat(s, s2);
	s[len - 1] = '"';
	s[len] = '\0';
	return (s);
}

static void	strlcpy_enjoyer(char *str, t_env *node, int statut, int size)
{
	if (statut == 2)
		ft_bzero(node->content, 2 + 1);
	else if (statut == 3 || statut == 5)
		ft_exstrlcpy(node->content, str, size + 2 + 1);
	else if (statut == 4 || statut == 6)
		ft_strlcpy(node->content, str, size + 1);
}

static int	node_exist(t_env **env, char *str, int size)
{
	t_env	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (ft_strncmp(str, tmp->name, size) == 0)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

static void	change_env(t_env *node, t_env **env, int info)
{
	t_env *tmp; // unused variable
	(void)tmp;
	tmp = *env;
	ft_lstadd_back_env(env, node);
	if (info == 1)
		order_export_env(env);
}

void	doublon_handler(t_msh *msh, char *str, t_env **env, t_intel i)
{
	t_env	*tmp;
	char	*tmp2;
	int		content_size;

	tmp = *env;
	content_size = ft_strlen(str + (i.name_size + 1));
	while (ft_strncmp(str, tmp->name, i.name_size))
		tmp = tmp->next;
	if (i.statut == 2)
		return ;
	if (i.statut == 3)
	{
		tmp2 = tmp->content;
		tmp->content = mlcgic(mlcp(NULL, sizeof(char) * (content_size + 2 + 1)), MALLOC, ENV, msh);
		// tmp->content = ft_magic_malloc(MALLOC, sizeof(char) * (content_size + 2 + 1), NULL, ENV);
		mlcgic(mlcp(tmp2, 0), FREE, ENV, msh);
		// ft_magic_malloc(FREE, 0, tmp2, ENV);
		ft_exstrlcpy(tmp->content, str + i.name_size, content_size + 2 + 1);
	}
	if (i.statut == 4)
	{
		tmp2 = tmp->content;
		tmp->content = mlcgic(mlcp(NULL, sizeof(char) * (content_size + 1)), MALLOC, ENV, msh);
		// tmp->content = ft_magic_malloc(MALLOC, sizeof(char) * content_size + 1, NULL, ENV);
		mlcgic(mlcp(tmp2, 0), FREE, ENV, msh);
		// ft_magic_malloc(FREE, 0, tmp2, ENV);
		ft_strlcpy(tmp->content, str + (i.name_size + 1), content_size + 1);
	}
	if (i.statut == 5)
	{
		tmp2 = tmp->content;
		tmp->content = mlcgic(mlcp(ft_exstrjoin(msh, tmp->content, str + i.name_size + 1), 1), ADD, ENV, msh);
		// tmp->content = ft_magic_malloc(ADD, 0, ft_exstrjoin(tmp->content, str + i.name_size + 1), ENV);
		mlcgic(mlcp(tmp2, 0), FREE, ENV, msh);
		// ft_magic_malloc(FREE, 0, tmp2, ENV);
	}
	if (i.statut == 6)
	{
		tmp2 = tmp->content;
		tmp->content = mlcgic(mlcp(ft_strjoin2(tmp->content, str + i.name_size + 1), 1), ADD, ENV, msh);
		// tmp->content = ft_magic_malloc(ADD, 0, ft_strjoin2(tmp->content, str + i.name_size + 1), ENV);
		mlcgic(mlcp(tmp2, 0), FREE, ENV, msh);
		// ft_magic_malloc(FREE, 0, tmp2, ENV);
	}
}

static void	error_export(t_msh *msh, char *str) // bash: export: `=': not a valid identifier
{
	char *message;
	char *tmp;

	tmp = mlcgic(mlcp(ft_strjoin("minishell: export: `", str), 1), ADD, NO_ENV, msh);
	// tmp = ft_magic_malloc(ADD, 0, ft_strjoin("minishell: export: `", str), NO_ENV);
	message = mlcgic(mlcp(ft_strjoin(tmp, "\': not a valid identifier\n"), 1), ADD, NO_ENV, msh);
	// message = ft_magic_malloc(ADD, 0, ft_strjoin(tmp, "\': not a valid identifier\n"), NO_ENV);
	mlcgic(mlcp(tmp, 0), FREE, NO_ENV, msh);
	// ft_magic_malloc(FREE, 0, tmp, NO_ENV);
	ft_putstr_fd(message, 2);
	mlcgic(mlcp(message, 0), FREE, NO_ENV, msh);
	// ft_magic_malloc(FREE, 0, message, NO_ENV);
}

int	new_env_node_env(t_msh *msh, char *str, int statut, t_env **env)
{
	t_env	*new;
	t_intel	i;
	int		content_size;

	i.name_size = get_name_size(str);
	content_size = ft_strlen2(str + i.name_size);
	if (!node_exist(env, str, i.name_size))
	{
		new = ft_lst_new_malloc(msh, i.name_size + 1, content_size + 1);
		if (!new)
		{
			mlcgic(NULL, FLUSH, ENV, msh); // ? QUIT PLUTOT NON? 
			// ft_magic_malloc(FLUSH, 0, NULL, ENV); // ? QUIT PLUTOT NON? 
			return (1);
		}
		ft_strlcpy(new->name, str, i.name_size + 1);
		strlcpy_enjoyer(str + (i.name_size + 1), new, statut + 2, content_size);
		change_env(new, env, 2);
	}
	else
	{
		i.statut = statut + 2;
		doublon_handler(msh, str, env, i);
	}
	return (0);
}

int	new_env_node_export(t_msh *msh, char *str, int statut, t_env **env)
{
	t_env	*new;
	t_intel	i;
	int		content_size;

	i.name_size = get_name_size(str);
	content_size = ft_strlen2(str + i.name_size);
	if (!node_exist(env, str, i.name_size))
	{
		new = ft_lst_new_malloc(msh, i.name_size + 1, content_size + 2 + 1);
		if (!new)
		{
			mlcgic(NULL, FLUSH, ENV, msh); // ? QUIT PLUTOT NON? 
			// ft_magic_malloc(FLUSH, 0, NULL, ENV); // ? QUIT PLUTOT NON? 
			return (1);
		}
		ft_strlcpy(new->name, str, i.name_size + 1);
		strlcpy_enjoyer(str + (i.name_size + 1), new, statut + 1, content_size);
		change_env(new, env, 1);
	}
	else
	{
		i.statut = statut + 1;
		doublon_handler(msh, str, env, i);
	}
	return (0);
}

// char	*enleve_le_plus(char *str)
// {
// 	int		i;
// 	int		j;
// 	int		flag;
// 	char	*tmp;

// 	i = 0;
// 	j = 0;
// 	flag = 0;
// 	tmp = ft_magic_malloc(MALLOC, sizeof(char) * ft_strlen(str), NULL, ENV);
// 	while (str[i])
// 	{
// 		if (flag == 0 && (str[i + 1] && (str[i] == '+' && str[i + 1] == '=')))
// 		{
// 			i++;
// 			flag = 1;
// 		}
// 		tmp[j] = str[i];
// 		i++;
// 		j++;
// 	}
// 	ft_magic_malloc(FREE, 0, str, PIP);
// 	tmp[j] = '\0';
// 	return (tmp);
// }

void	builtin_export(t_msh *msh)
{
	int	i;

	i = 1;
	if (!msh->p.cmd_t[i])
		ft_print_export(msh);
	else
	{
		while (msh->p.cmd_t[i])
		{
			if (!valide_key(msh->p.cmd_t[i]))
				(msh->status = 1, error_export(msh, msh->p.cmd_t[i]));
			else
			{
				if (get_statut(msh->p.cmd_t[i]) == 1)
					new_env_node_export(msh, msh->p.cmd_t[i], 1, msh->export_env);
				else if (get_statut(msh->p.cmd_t[i]) == 2)
				{
					new_env_node_export(msh, msh->p.cmd_t[i], 2, msh->export_env);
					new_env_node_env(msh, msh->p.cmd_t[i], 2, msh->env);
				}
				else if (get_statut(msh->p.cmd_t[i]) == 4)
				{
					new_env_node_export(msh, msh->p.cmd_t[i], 4, msh->export_env);
					new_env_node_env(msh, msh->p.cmd_t[i], 4, msh->env);
				}
			}
			i++;
		}
	}
	// if (!msh->p.cmd_t[i])
	// 	for (size_t j = 0; msh->p.cmd_t[j]; j++)
	// 	{
	// 		printf("oui : %s\n", msh->p.cmd_t[j]);
	// 	}
	// if(!msh->p.cmd_t[i])
	// 	ft_print_export(msh);
}
