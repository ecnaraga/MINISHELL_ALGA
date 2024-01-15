/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_str.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 15:44:01 by athiebau          #+#    #+#             */
/*   Updated: 2024/01/15 10:59:50 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
Ajouter des espaces avant et apres des operateurs
	- espaces apres 🗸
	- espaces avant 🗸
	- double operateurs 🗸
	- calcul du malloc 🗸
	- guillemets X
*/
static void	flag_handler(int *flag, char c)
{
	if ((c == '"' || c == '\'') && *flag == 0)
	{
		if (c == '"')
			*flag = 1;
		else if (c == '\'')
			*flag = 2;
	}
	else if ((c == '"' && *flag == 1) || (c == '\'' && *flag == 2))
		*flag = 0;
}

static void	get_final_size(int *count, int *flag, char *str, size_t *i)
{
	*i = 0;
	*count = 0;
	*flag = 0;
	while (str[*i])
	{
		flag_handler(flag, str[*i]);
		if (is_operator(str[*i]) == 1 && *flag == 0)
		{
			if ((*i > 0 && ft_isspace(str[(*i) - 1]) == 1
					&& is_operator(str[(*i) - 1]) == 0))
				*count += 1;
			if (*i < ft_strlen(str) && (str[(*i) + 1] == str[*i]))
				*i += 1;
			if (((*i) + 1 < ft_strlen(str) && ft_isspace(str[(*i) + 1]) == 1)
				&& !((str[*i] == '>' && str[(*i) + 1] == '<') || (str[*i] == '<'
						&& str[(*i) + 1] == '>')) && !((str[*i] == '|'
						&& str[(*i) + 1] == '&') || (str[*i] == '&' && str[(*i)
							+ 1] == '|')))
				*count += 1;
		}
		*i += 1;
	}
	*flag = 0;
}

static void	get_final_str2(size_t *i, size_t *j, char *str, char *fstr)
{
	if (is_operator(str[(*i) + 1]) == 0 || (is_operator(str[(*i) + 1]) == 1
			&& str[*i] != str[(*i) + 1]))
	{
		fstr[(*j)] = str[(*i)];
		if ((*i) + 1 < ft_strlen(str) && !((str[*i] == '|'
					&& str[(*i) + 1] == '&') || (str[*i] == '&'
					&& str[(*i) + 1] == '|')))
		{
			*j += 1;
			fstr[*j] = ' ';
		}
	}
}

static void	get_final_str(size_t *i, size_t *j, char *str, char *fstr)
{
	if (*i > 0 && ft_isspace(str[(*i) - 1]) == 1
		&& is_operator(str[(*i) - 1]) == 0)
	{
		if (is_operator(str[*i - 1]) == 0)
		{
			fstr[*j] = ' ';
			*j += 1;
		}
		fstr[*j] = str[*i];
	}
	else if (*i < ft_strlen(str) && (str[*i] == str[(*i) + 1]))
		fstr[*j] = str[*i];
	else if (*i < ft_strlen(str) && *i > 0 && (str[*i] == str[(*i) - 1]))
		fstr[*j] = str[*i];
	if (*i < ft_strlen(str) && (ft_isspace(str[(*i) + 1]) == 1)
		&& !((str[*i] == '>' && str[(*i) + 1] == '<')
			|| (str[*i] == '<' && str[(*i) + 1] == '>'))
		&& !((str[*i] == '>' && str[(*i) + 1] == '(')
			|| (str[*i] == '<' && str[(*i) + 1] == '(')))
		get_final_str2(i, j, str, fstr);
	else if (*i < ft_strlen(str))
		fstr[*j] = str[*i];
}

char	*add_spaces(t_msh *msh, char *str)
{
	char	*fstr;
	size_t	i;
	size_t	j;
	int		count;
	int		flag;

	get_final_size(&count, &flag, str, &i);
	fstr = mcgic(mlcp(NULL, sizeof(char) * (i + count + 1)), MLC, NO_ENV, msh);
	if (!fstr)
		ft_exit(-1, -1, -1, msh);
	i = -1;
	j = 0;
	while (str[++i])
	{
		flag_handler(&flag, str[i]);
		if (is_operator(str[i]) == 1 && flag == 0)
			get_final_str(&i, &j, str, fstr);
		else
			fstr[j] = str[i];
		j++;
	}
	fstr[j] = '\0';
	mcgic(mlcp(str, 0), FREE, NO_ENV, msh);
	return (fstr);
}
