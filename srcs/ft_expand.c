/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 10:50:24 by galambey          #+#    #+#             */
/*   Updated: 2023/12/21 17:21:51 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


// TO DO : ATTENTION A CE TEST : echo $=HOME => Pour l instant traite dans parsing comme si variable d environnement a voir...

/*
Check if the first char after the $ is a valid char that can begin an
	environment variable (alpha or _) or a ?
- If yes return 0
- If not return 1
	In that case as the subject tell us to expand only an ENVIRONMENT VARIABLE
		by its value and to expand the $?, we treat all other case like bash
		with $: or $+ for example
*/
int	ft_first_valid_char(char c)
{
	if (ft_isalpha(c) != 0 || c == '_' || c == '?')
		return (0);
	return (1);
}
/*
bash-5.1$ echo $ga8ga

bash-5.1$ echo $ga8+ga
+ga
bash-5.1$ 

*/
int	valide_expand(char *key) // va nous dire si les char sont ok // A IMPLEMENTER
{
	int	i;

	i = 1;
	if (ft_isalpha(key[0]) || key[0] == '_')
	{
		while(key[i] && key[i] != '=')
		{
			if (ft_isalnum(key[i]) || key[i] == '_' )
				i++;
			else
				return (0);	
		}
		return (1);
	}
	return (0);
}

char *get_value(t_msh *msh, t_env **env, char *str, int rule)
{
	t_env	*node;

	node = *env;
	while (node)
	{
		if (strcmp(node->name, str) == 0)
		{
			mlcgic(mlcp(str, 0), FREE, NO_ENV, msh);
			// ft_magic_malloc(FREE, 0, str, NO_ENV);
			if (rule == CMD)
				return (mlcgic(mlcp(ft_strtrim(node->content, " \b\t\n\v\f\r"), 1), ADD, NO_ENV, msh));
				// return (ft_magic_malloc(ADD, 0, ft_strtrim(node->content, " \b\t\n\v\f\r"), NO_ENV));
			if (rule == OTHER)
				return (mlcgic(mlcp(ft_strtrim_except_tips(node->content, " \b\t\n\v\f\r"), 1), ADD, NO_ENV, msh));
				// return (ft_magic_malloc(ADD, 0, ft_strtrim_except_tips(node->content, " \b\t\n\v\f\r"), NO_ENV));
			if (rule == INFILE || rule == OUTFILE_NO_TRUNC || rule == OUTFILE_TRUNC)
				return (mlcgic(mlcp(ft_strdup(node->content), 1), ADD, NO_ENV, msh));
				// return (ft_magic_malloc(ADD, 0, ft_strdup(node->content), NO_ENV));
		}
		else
			node = node->next;
	}
	mlcgic(mlcp(str, 0), FREE, NO_ENV, msh);
	// ft_magic_malloc(FREE, 0, str, NO_ENV);
	return (mlcgic(mlcp(ft_strdup(""), 1), ADD, NO_ENV, msh));
	// return (ft_magic_malloc(ADD, 0, ft_strdup(""), NO_ENV));
}

/*
Pour savoir s'il comment traiter le dollar, se referer aux instructions dans le
	tableau de structure strs.type (plus de precisions dans ft_strlcpy_msh)
1. Si (expnd = EXPAND = 1 et len variable = 1) ou si (expnd = MULTIDOLLAR = 3)
	=> afficher 1 $
2. Si (expnd = NO_EXPAND = 2) => afficher les char tel quel et ne pas expand
3. Si (expnd = EXPAND = 1 et len_variable = 0) => ne pas afficher le dollar
4. Si (expnd = EXPAND = 1 et len_variable > 1) => expand : remplacer la
	variable d environnement par sa valeur dans l 'env
	exemple : si expnd = 1 && len_variable = 5 => remplacer $ + 4 char
		(ex : $USER) par la variable d environnement dans env

REGLES A SUIVRE :
- Si plusieurs dollars d affilee : afficher 1 dollar
- Si 1 seul dollar suivi de quote : ne pas afficher le dollar
- Si un seul dollar entre des quotes single ou double : afficher 1 dollar
- Si dollar pas entre quote : expand
- Si dollar entre double_quote : expand
- Si dollar entre de single_quote : ne pas expand
*/
char	*ft_expand(t_msh *msh, char *cmd, int rule)
{
	char *tmp;
	char *tmp2;
	int i;
	int j;
	
	i = 0;
	j = 0;
	cmd = NULL;
	while (msh->av->data[i])
	{
		if (msh->av->data[i] == '$')
		{
			if (msh->av->type[j].expnd == 3)
			{
				tmp2 = cmd;
				cmd = mlcgic(mlcp(ft_strjoin_char(cmd, msh->av->data[i]), 1), ADD, NO_ENV, msh);
				// cmd = ft_magic_malloc(ADD, 0, ft_strjoin_char(cmd, msh->av->data[i]), NO_ENV);
				if (msh->status == 255) // IF MALLOC KO return NULL
					return (NULL);
					// ft_exit(-1, -1, -1);
				if (tmp2)
					mlcgic(mlcp(tmp2, 0), FREE, NO_ENV, msh);
					// ft_magic_malloc(FREE, 0, tmp2, NO_ENV);
				while (msh->av->data[i] && msh->av->data[i] == '$')
					i++;
				j++;
			}
			else if ((msh->av->type[j].expnd == 1 && (msh->av->type[j].len_variable == 1 || ft_first_valid_char(msh->av->data[i + 1]) == 1)) || msh->av->type[j].expnd == 2)
			{
				tmp2 = cmd;
				cmd = mlcgic(mlcp(ft_strjoin_char(cmd, msh->av->data[i]), 1), ADD, NO_ENV, msh);
				// cmd = ft_magic_malloc(ADD, 0, ft_strjoin_char(cmd, msh->av->data[i]), NO_ENV);
				if (msh->status == 255) // IF MALLOC KO return NULL
					return (NULL);
					// ft_exit(-1, -1, -1);
				if (tmp2)
					mlcgic(mlcp(tmp2, 0), FREE, NO_ENV, msh);
					// ft_magic_malloc(FREE, 0, tmp2, NO_ENV);
				i++;
				j++;
			}
			// else if (msh->av->type[j].expnd == 2)
			// {
			// 	tmp2 = cmd;
			// 	cmd = ft_magic_malloc(ADD, 0, ft_strjoin_char(cmd, msh->av->data[i]), NO_ENV);
			// 	if (status == 255) // a checker si en cas de add malloc qui foire status mis a 255
			// 		ft_exit(-1, -1, -1);
			// 	if (tmp2)
			// 		ft_magic_malloc(FREE, 0, tmp2, NO_ENV);
			// 	i++;
			// 	j++;
			// }
			else if (msh->av->type[j].expnd == 1 && msh->av->type[j].len_variable > 1)
			{
				if (msh->av->data[i + 1] == '?')
				{
					tmp = mlcgic(mlcp(ft_itoa(msh->previous_status), 1), ADD, NO_ENV, msh);
					// tmp = ft_magic_malloc(ADD, 0, ft_itoa(msh->previous_status), NO_ENV);
					if (msh->status == 255)// IF MALLOC KO return NULL
						return (NULL);
						// ft_exit(-1, -1, -1);
					tmp2 = cmd;
					cmd = mlcgic(mlcp(ft_strjoin(cmd, tmp), 1), ADD, NO_ENV, msh);
					// cmd = ft_magic_malloc(ADD, 0, ft_strjoin(cmd, tmp), NO_ENV);
					if (msh->status == 255)// IF MALLOC KO return NULL
						return (NULL);
						// ft_exit(-1, -1, -1);
					if (tmp2)
						mlcgic(mlcp(tmp2, 0), FREE, NO_ENV, msh);
						// ft_magic_malloc(FREE, 0, tmp2, NO_ENV);
					mlcgic(mlcp(tmp, 0), FREE, NO_ENV, msh);
					// ft_magic_malloc(FREE, 0, tmp, NO_ENV);
					i += 2;
					j++;
				}
				else
				{
					tmp = mlcgic(mlcp(ft_substr(msh->av->data, i + 1, msh->av->type[j].len_variable - 1), 1), ADD, NO_ENV, msh);
					// tmp = ft_magic_malloc(ADD, 0, ft_substr(msh->av->data, i + 1, msh->av->type[j].len_variable - 1), NO_ENV);
					if (msh->status == 255) // IF MALLOC KO return NULL
						return (NULL);
						// ft_exit(-1, -1, -1);
					tmp = get_value(msh, msh->env, tmp, rule);
					if (msh->status == 255)// IF MALLOC KO return NULL
						return (NULL);
						// ft_exit(-1, -1, -1);
					tmp2 = cmd;
					cmd = mlcgic(mlcp(ft_strjoin(cmd, tmp), 1), ADD, NO_ENV, msh);
					// cmd = ft_magic_malloc(ADD, 0, ft_strjoin(cmd, tmp), NO_ENV);
					if (msh->status == 255) // IF MALLOC KO return NULL
						return (NULL);
						// ft_exit(-1, -1, -1);
					if (tmp2)
						mlcgic(mlcp(tmp2, 0), FREE, NO_ENV, msh);
						// ft_magic_malloc(FREE, 0, tmp2, NO_ENV);
					mlcgic(mlcp(tmp, 0), FREE, NO_ENV, msh);
					// ft_magic_malloc(FREE, 0, tmp, NO_ENV);
					i += msh->av->type[j].len_variable;
					j++;
				}
			}
			
		}
		else
		{
			tmp2 = cmd;
			cmd = mlcgic(mlcp(ft_strjoin_char(cmd, msh->av->data[i]), 1), ADD, NO_ENV, msh);
			// cmd = ft_magic_malloc(ADD, 0, ft_strjoin_char(cmd, msh->av->data[i]), NO_ENV);
			if (msh->status == 255) // IF MALLOC KO return NULL
				return (NULL);
				// ft_exit(-1, -1, -1);
			if (tmp2)
				mlcgic(mlcp(tmp2, 0), FREE, NO_ENV, msh);
				// ft_magic_malloc(FREE, 0, tmp2, NO_ENV);
			i++;
		}
	}
	return (cmd);
}
