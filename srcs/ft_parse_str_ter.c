/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_str_ter.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 16:06:45 by athiebau          #+#    #+#             */
/*   Updated: 2023/11/23 17:29:42 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h"

// size_t	ft_strlen(const char *str)
// {
// 	int	i;

// 	i = 0;
// 	while (str[i])
// 		i++;
// 	return (i);
// }

/*----------------------------------------------------------------------------*/

char	*ft_parse_bis_bis(char *str)
{
	char	*fstr;
	size_t	i;
	size_t	j;
	int		count;
	int	flag;

	i = 0;
	j = 0;
	count = 0;
	flag = 0;
	while (str[i])
	{
		if ((str[i] == '"' || str[i] == '\'') && flag == 0)
		{
			if(str[i] == '"')
				flag = 1;
			else if (str[i] == '\'')
				flag = 2;
			i++;
		}
		if ((str[i] == '"'  && flag == 1 ) || (str[i] == '\'' && flag == 2))
			flag = 0;
		if ((str[i] == '(' || str[i] == ')') && flag == 0)
		{
			if ((i > 0 && ft_isspace(str[i - 1]) == 1 && !(str[i - 1] == '(' || str[i - 1] == ')')))
				count++;
			if ((i + 1 < ft_strlen(str) && ft_isspace(str[i + 1]) == 1))
				count++;
		}
		i++;
	}
	fstr = ft_magic_malloc(MALLOC, sizeof(char) * (i + count + 1), NULL);
	if (!fstr)
		return (NULL);
	i = 0;
	flag = 0;
	while (str[i])
	{
		if ((str[i] == '"' || str[i] == '\'') && flag == 0)
		{
			if(str[i] == '"')
				flag = 1;
			else if (str[i] == '\'')
				flag = 2;
			fstr[j++] = str[i++];
		}
		if ((str[i] == '"'  && flag == 1 ) || (str[i] == '\'' && flag == 2))
			flag = 0;
		if ((str[i] == '(' || str[i] == ')') && flag == 0)
		{
			if (i > 0 && ft_isspace(str[i - 1]) == 1 && !(str[i - 1] == '(' || str[i - 1] == ')'))
			{
				fstr[j++] = ' ';
				fstr[j] = str[i];
				
			}	
			if (i < ft_strlen(str) && (ft_isspace(str[i + 1]) == 1))
			{
				
				fstr[j] = str[i];
				if (i + 1 < ft_strlen(str))
					fstr[++j] = ' ';
			}
		}
		else
			fstr[j] = str[i];
		i++;
		j++;
	}
	fstr[j] = '\0';
	ft_magic_malloc(FREE, 0, str);
	return (fstr);
}

// int	main(int ac, char **av)
// {
// 	char	*str;

// 	(void)ac;
// 	(void)av;
// 	while (1)
// 	{
// 		str = readline("Oui$ "); // malloc
// 		if (!str)
// 			return (1);
// 		printf("str avant : _%s_\n", str);
// 		str = ft_parse_bis_bis(str);
// 		printf("str apres : _%s_\n", str);
// 		printf("--------------------------------\n");
// 	}
// }
