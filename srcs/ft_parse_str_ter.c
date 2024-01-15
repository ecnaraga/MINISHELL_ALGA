/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_str_ter.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 16:06:45 by athiebau          #+#    #+#             */
/*   Updated: 2024/01/15 11:17:12 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

/* attention 
dans la boucle on teste des str [i - 1] et str[i + 1] .. 
risque de invalid read ou write <-- Non, c'est protege
*/
static void	get_final_size(size_t *i, int *count, int *flag, char *str)
{
	*i = 0;
	*count = 0;
	*flag = 0;
	while (str[*i])
	{
		flag_handler(flag, str[*i]);
		if ((str[*i] == '(' || str[*i] == ')') && *flag == 0)
		{
			if ((*i > 0 && ft_isspace(str[(*i) - 1]) == 1
					&& !(str[(*i) - 1] == '(' || str[(*i) - 1] == ')')))
				(*count)++;
			if (((*i) + 1 < ft_strlen(str) && ft_isspace(str[(*i) + 1]) == 1))
				(*count)++;
		}
		(*i)++;
	}
	*flag = 0;
}

static void	get_final_str(size_t *i, size_t *j, char *fstr, char *str)
{
	if (*i > 0 && ft_isspace(str[(*i) - 1]) == 1 && !(str[(*i) - 1] == '('
			|| str[(*i) - 1] == ')'))
	{
		fstr[*j] = ' ';
		*j += 1;
		fstr[*j] = str[*i];
	}
	else
		fstr[*j] = str[*i];
	if (*i < ft_strlen(str) && (ft_isspace(str[(*i) + 1]) == 1))
	{
		fstr[*j] = str[*i];
		if ((*i) + 1 < ft_strlen(str))
		{
			*j += 1;
			fstr[*j] = ' ';
		}
	}
}

char	*ft_parse_bis_bis(t_msh *msh, char *str)
{
	char	*fstr;
	size_t	i;
	size_t	j;
	int		count;
	int		flag;

	get_final_size(&i, &count, &flag, str);
	if (count == 0)
		return (str);
	fstr = mcgic(mlcp(NULL, sizeof(char) * (i + count + 1)), MLC, NO_ENV, msh);
	if (!fstr)
		ft_exit(-1, -1, -1, msh);
	i = -1;
	j = 0;
	while (str[++i])
	{
		flag_handler(&flag, str[i]);
		if ((str[i] == '(' || str[i] == ')') && flag == 0)
			get_final_str(&i, &j, fstr, str);
		else
			fstr[j] = str[i];
		j++;
	}
	fstr[j] = '\0';
	return (mcgic(mlcp(str, 0), FREE, NO_ENV, msh), fstr);
}
