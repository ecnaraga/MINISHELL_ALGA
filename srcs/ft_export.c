/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garance <garance@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 14:13:14 by athiebau          #+#    #+#             */
/*   Updated: 2023/12/10 11:57:03 by garance          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_exstrjoin(char const *s1, char const *s2)
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
	while(tmp)
	{
		if (ft_strncmp(str, tmp->name, size) == 0)
			return (0);
		tmp = tmp->next;
	}
	return (1);
}

static void	change_env(t_env *node, t_env **env, int info)
{
	t_env *tmp; // unused variable

	(void) tmp;
	tmp = *env;
	ft_lstadd_back_env(env, node);
	if (info == 1)
		order_export_env(env);
}

void	doublon_handler(char *str, t_env **env, int name_size, int statut)
{
	t_env	*tmp;
	char	*tmp2;
	int	content_size;
	
	tmp = *env;
	content_size = ft_strlen(str + name_size);
	while(ft_strncmp(str, tmp->name, name_size))
		tmp = tmp->next;
	if (statut == 2)
		return ;
	if (statut == 3)
	{
		tmp2 = tmp->content;
		tmp->content = ft_magic_malloc(MALLOC, sizeof(char) * content_size + 2 + 1, NULL, ENV);
		ft_magic_malloc(FREE, 0, tmp2, ENV);
		ft_exstrlcpy(tmp->content, str + name_size, content_size + 2 + 1);
	}
	if (statut == 4)
	{
		tmp2 = tmp->content;
		tmp->content = ft_magic_malloc(MALLOC, sizeof(char) * content_size + 1, NULL, ENV);
		ft_magic_malloc(FREE, 0, tmp2, ENV);
		ft_strlcpy(tmp->content, str + name_size, content_size + 1);
	}
	if (statut == 5)
	{
		tmp2 = tmp->content;
		tmp->content = ft_magic_malloc(ADD, 0, ft_exstrjoin(tmp->content, str + name_size + 1), ENV);
		ft_magic_malloc(FREE, 0, tmp2, ENV);
	}
	if (statut == 6)
	{
		tmp2 = tmp->content;
		tmp->content = ft_magic_malloc(ADD, 0, ft_strjoin(tmp->content, str + name_size + 1), ENV);
		ft_magic_malloc(FREE, 0, tmp2, ENV);
	}
}

void	new_env_node(char *str, int statut, t_env **env, int info)
{
	t_env	*new;
	int	name_size;
	int	content_size;

	printf("valide key : %d\n", valide_key(str));
	if(!valide_key(str))
		return ; //GERER SI L'INDENTIFIER N'EST PAS Ok : bash: export: `=': not a valid identifier
	name_size = get_name_size(str);
	content_size = ft_strlen(str + name_size);
	if(node_exist(env, str, name_size))
	{
		if (info == 2)
			new = ft_lst_new_malloc(name_size + 1, content_size + 1);
		else 
			new = ft_lst_new_malloc(name_size + 1, content_size + 2 + 1);
		if (!new)
		{
			ft_magic_malloc(FLUSH, 0, NULL, ENV);
			return ;
		}
		ft_strlcpy(new->name, str, name_size + 1);
		strlcpy_enjoyer(str + name_size, new, statut + info, content_size);
		change_env(new, env, info);
	}
	else
		doublon_handler(str, env, name_size, statut + info);
	return ;
}

char	*enleve_le_plus(char *str)
{
	int	i;
	int	j;
	int	flag;
	char	*tmp;

	i = 0;
	j = 0;
	flag = 0;
	tmp = ft_magic_malloc(MALLOC, sizeof(char) * ft_strlen(str), NULL, ENV);
	while(str[i])
	{
		if(flag == 0 && (str[i + 1] && (str[i] == '+' && str[i + 1] == '=')))
		{
			i++;
			flag = 1;
		}
		tmp[j] = str[i];
		i++;
		j++;
	}
	//free(str);
	tmp[j] = '\0';
	return (tmp);
}

void	builtin_export(t_msh *minish)
{
	int	i;

	i = 1;
	for (size_t j = 0; minish->p.cmd_opt[j]; j++)
	{
		printf("oui : %s\n", minish->p.cmd_opt[j]);
	}
	
	if(!minish->p.cmd_opt[i])
		ft_print_export(minish);
	else
	{
		while(minish->p.cmd_opt[i])
		{
			if(get_statut(minish->p.cmd_opt[i]) == 1)
				new_env_node(minish->p.cmd_opt[i], 1, minish->export_env, 1);
			else if (get_statut(minish->p.cmd_opt[i]) == 2)
			{
				new_env_node(minish->p.cmd_opt[i], 2, minish->export_env, 1);
				new_env_node(minish->p.cmd_opt[i], 2, minish->env, 2);
			}
			else if (get_statut(minish->p.cmd_opt[i]) == 4)
			{
				new_env_node(enleve_le_plus(minish->p.cmd_opt[i]), 4, minish->export_env, 1);
				new_env_node(enleve_le_plus(minish->p.cmd_opt[i]), 4, minish->env, 2);
			}
			i++;
		}
		ft_print_export(minish);
	}
}
