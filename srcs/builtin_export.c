/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 14:13:14 by athiebau          #+#    #+#             */
/*   Updated: 2024/01/04 16:54:26 by athiebau         ###   ########.fr       */
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
		len = ft_strlen(s1) + ft_strlen2(s2);
	s = (char *)malloc(sizeof(char) * (len + 1));
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
	s = mcgic(mlcp(NULL, sizeof(char) * (len + 1)), MLC, ENV, msh);
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

static void	change_env(t_env *node, t_env **env, int info)
{
	ft_lstadd_back_env(env, node);
	if (info == 1)
		order_export_env(env);
}

void	doublon_handler_bis(t_msh *msh, char *str, t_env *tmp, t_intel i)
{
	char	*tmp2;
	int		content_size;

	content_size = ft_strlen(str + (i.name_size + 1));
	if (i.statut == 4)
	{
		tmp2 = tmp->content;
		tmp->content = mcgic(mlcp(NULL, sizeof(char) * (content_size + 1)),
				MLC, ENV, msh);
		mcgic(mlcp(tmp2, 0), FREE, ENV, msh);
		ft_strlcpy(tmp->content, str + (i.name_size + 1), content_size + 1);
	}
	if (i.statut == 5)
	{
		tmp2 = tmp->content;
		tmp->content = mcgic(mlcp(ft_exstrjoin(msh, tmp->content, str
						+ i.name_size + 1), 1), ADD, ENV, msh);
		mcgic(mlcp(tmp2, 0), FREE, ENV, msh);
	}
}

void	doublon_handler(t_msh *m, char *str, t_env **env, t_intel i)
{
	t_env	*t;
	char	*tmp2;
	int		c;

	t = *env;
	c = ft_strlen(str + (i.name_size + 1));
	while (ft_strncmp(str, t->name, i.name_size))
		t = t->next;
	if (i.statut == 2)
		return ;
	if (i.statut == 3)
	{
		tmp2 = t->content;
		t->content = mcgic(mlcp(NULL, sizeof(char) * (c + 2 + 1)), MLC, ENV, m);
		mcgic(mlcp(tmp2, 0), FREE, ENV, m);
		ft_exstrlcpy(t->content, str + i.name_size, c + 2 + 1);
	}
	if (i.statut == 6)
	{
		tmp2 = t->content;
		t->content = mcgic(mlcp(ft_strjoin2(t->content, str + i.name_size
						+ 1), 1), ADD, ENV, m);
		mcgic(mlcp(tmp2, 0), FREE, ENV, m);
	}
	doublon_handler_bis(m, str, t, i);
}

static void	error_export(t_msh *msh, char *str)
{
	char *message;
	char *tmp;

	tmp = mcgic(mlcp(ft_strjoin("minishell: export: `", str), 1), ADD, NO_ENV,
		msh);
	message = mcgic(mlcp(ft_strjoin(tmp, "\': not a valid identifier\n"), 1),
				ADD, NO_ENV, msh);
	mcgic(mlcp(tmp, 0), FREE, NO_ENV, msh);
	ft_putstr_fd(message, 2);
	mcgic(mlcp(message, 0), FREE, NO_ENV, msh);
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
			return (255);
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
			return (255);
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

static int	builtin_export_exec(int i, t_msh *msh)
{
	if (get_statut(msh->p.cmd_t[i]) == 1)
	{
		if (new_env_node_export(msh, msh->p.cmd_t[i], 1, msh->export_env) != 0)
			return (1);
	}
	else if (get_statut(msh->p.cmd_t[i]) == 2)
	{
		if (new_env_node_export(msh, msh->p.cmd_t[i], 2, msh->export_env) != 0)
			return (1);
		if (new_env_node_env(msh, msh->p.cmd_t[i], 2, msh->env) != 0)
			return (1);
	}
	else if (get_statut(msh->p.cmd_t[i]) == 4)
	{
		if (new_env_node_export(msh, msh->p.cmd_t[i], 4, msh->export_env) != 0)
			return (1);
		if (new_env_node_env(msh, msh->p.cmd_t[i], 4, msh->env) != 0)
			return (1);
	}
	return (0);
}

void	builtin_export(t_msh *msh)
{
	int	i;

	i = 0;
	if (!msh->p.cmd_t[1])
		ft_print_export(msh);
	else
	{
		while (msh->p.cmd_t[++i])
		{
			if (!valide_key(msh->p.cmd_t[i]))
				(msh->status = 1, error_export(msh, msh->p.cmd_t[i]));
			else
			{
				if (builtin_export_exec(i, msh) == 1)
					return ;
			}
		}
	}
}
