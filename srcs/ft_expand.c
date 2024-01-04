/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 10:50:24 by galambey          #+#    #+#             */
/*   Updated: 2024/01/04 13:25:28 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


// TO DO : ATTENTION A CE TEST : echo $=HOME => Pour l instant traite dans parsing comme si variable d environnement a voir...

/*
Return 0 if all char in key is valid
Otherwise return the index of the first non valid char
*/
int	valide_expand(char *key) // va nous dire si les char sont ok // A IMPLEMENTER
{
	int	i;

	i = 0;
	if (ft_isalpha(key[0]) || key[0] == '_')
	{
		while(key[i])
		{
			if (ft_isalnum(key[i]) || key[i] == '_' )
				i++;
			else
				return (i);	
		}
		return (0);
	}
	return (-1);
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
			if (rule == CMD)
				return (mlcgic(mlcp(ft_strtrim(node->content, " \b\t\n\v\f\r"), 1), ADD, NO_ENV, msh));
			if (rule == OTHER)
				return (mlcgic(mlcp(ft_strtrim_except_tips(node->content, " \b\t\n\v\f\r"), 1), ADD, NO_ENV, msh));
			if (rule == INFILE || rule == OUTFILE_NO_TRUNC || rule == OUTFILE_TRUNC || rule == HDOC)
				return (mlcgic(mlcp(ft_strdup(node->content), 1), ADD, NO_ENV, msh));
		}
		else
			node = node->next;
	}
	mlcgic(mlcp(str, 0), FREE, NO_ENV, msh);
	return (mlcgic(mlcp(ft_strdup(""), 1), ADD, NO_ENV, msh));
}

char	*ft_multi_dollar(t_msh *msh, char *cmd, t_expand *e)
{
	e->tmp = cmd;
	cmd = mlcgic(mlcp(ft_strjoin_char(cmd, msh->av->data[e->i]), 1), ADD, NO_ENV, msh);
	if (msh->status == 255) // IF MALLOC KO return NULL
		return (NULL);
	if (e->tmp)
		mlcgic(mlcp(e->tmp, 0), FREE, NO_ENV, msh);
	(e->i)++;
	(e->j)++;
	return (cmd);
}

char	*ft_add_char(t_msh *msh, char *cmd, t_expand *e, int *j)
{
	e->tmp = cmd;
	cmd = mlcgic(mlcp(ft_strjoin_char(cmd, msh->av->data[e->i]), 1), ADD, NO_ENV, msh);
	if (msh->status == 255) // IF MALLOC KO return NULL
		return (NULL);
	if (e->tmp)
		mlcgic(mlcp(e->tmp, 0), FREE, NO_ENV, msh);
	(e->i)++;
	if (j)
		(*j)++;
	return (cmd);
}

char	*ft_do_expand(t_msh *msh, char *tmp, char *cmd, int rule)
{
	char *tmp2;
	
	tmp = get_value(msh, msh->env, tmp, rule);
	if (msh->status == 255)// IF MALLOC KO return NULL
		return (NULL);
	tmp2 = cmd;
	cmd = mlcgic(mlcp(ft_strjoin(cmd, tmp), 1), ADD, NO_ENV, msh);
	printf("cmd = %s\n", cmd);
	if (msh->status == 255) // IF MALLOC KO return NULL
		return (NULL);
	if (tmp2)
		mlcgic(mlcp(tmp2, 0), FREE, NO_ENV, msh);
	mlcgic(mlcp(tmp, 0), FREE, NO_ENV, msh);
	return (cmd);
}

char	*ft_expand_exitcode(t_msh *msh, char *cmd, t_expand *e)
{
	e->tmp = mlcgic(mlcp(ft_itoa(msh->previous_status), 1), ADD, NO_ENV, msh);
	if (msh->status == 255)// IF MALLOC KO return NULL
		return (NULL);
	e->tmp2 = cmd;
	cmd = mlcgic(mlcp(ft_strjoin(cmd, e->tmp), 1), ADD, NO_ENV, msh);
	if (msh->status == 255)// IF MALLOC KO return NULL
		return (NULL);
	if (e->tmp2)
		mlcgic(mlcp(e->tmp2, 0), FREE, NO_ENV, msh);
	mlcgic(mlcp(e->tmp, 0), FREE, NO_ENV, msh);
	(e->i) += 2;
	(e->j)++;
	return (cmd);
}

char	*ft_expand_valid_var(t_msh *msh, char *cmd, int rule, t_expand *e)
{
	e->tmp2 = e->tmp;
	e->tmp = mlcgic(mlcp(ft_substr(e->tmp, 0, e->len), 1), ADD, NO_ENV, msh);
	if (msh->status == 255) // IF MALLOC KO return NULL
		return (NULL);
	mlcgic(mlcp(e->tmp2, 0), FREE, NO_ENV, msh);
	cmd = ft_do_expand(msh, e->tmp, cmd, rule);
	if (msh->status == 255)// IF MALLOC KO return NULL
		return (NULL);
	return (cmd);
}

char	*ft_expand_env(t_msh *msh, char *cmd, int rule, t_expand *e)
{
	e->tmp = mlcgic(mlcp(ft_substr(msh->av->data, e->i + 1, msh->av->type[e->j].len_variable - 1), 1), ADD, NO_ENV, msh);
	if (msh->status == 255) // IF MALLOC KO return NULL
		return (NULL);
	e->len = valide_expand(e->tmp);
	if (e->len == 0)
	{
		cmd = ft_do_expand(msh, e->tmp, cmd, rule);
		if (msh->status == 255)// IF MALLOC KO return NULL
			return (NULL);
		e->i += msh->av->type[e->j].len_variable;
	}
	else if (e->len == -1)
		e->i += 2;
	else
	{
		cmd = ft_expand_valid_var(msh, cmd, rule, e);
		if (msh->status == 255)
			return (NULL);
		e->i += (e->len + 1);
	}
	e->j++;
	return (cmd);
}

char	*ft_expand_var(t_msh *msh, char *cmd, int rule, t_expand *e)
{
	if (msh->av->data[e->i + 1] == '?')
		cmd = ft_expand_exitcode(msh, cmd, e); // PROTEGER APRES LES IF ELSE IF
	else
		cmd = ft_expand_env(msh, cmd, rule, e);
	return (cmd);
}

char	*ft_handle_dollar(t_msh *msh, char *cmd, int rule, t_expand *e)
{
	if (msh->av->type[e->j].expnd == MULTI_DOLLAR)
		cmd = ft_add_char(msh, cmd, e, &e->j); // PROTEGER DANS FT_EXPAND
	else if ((msh->av->type[e->j].expnd == 1 && msh->av->type[e->j].len_variable == 1) || msh->av->type[e->j].expnd == 2)
		cmd = ft_add_char(msh, cmd, e, &e->j); // PROTEGER DANS FT_EXPAND
	else if(msh->av->type[e->j].expnd == 1 && msh->av->type[e->j].len_variable == 0)
	{
		e->i += 1;
		e->j += 1;
	}
	else if (msh->av->type[e->j].expnd == 1 && msh->av->type[e->j].len_variable > 1)
		cmd = ft_expand_var(msh, cmd, rule, e); // PROTEGER DANS FT_EXPAND
	return (cmd);
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
	t_expand	e;
	
	e.i = 0;
	e.j = 0;
	cmd = NULL;
	while (msh->av->data[e.i])
	{
		if (msh->av->data[e.i] == '$')
			cmd = ft_handle_dollar(msh, cmd, rule, &e); // PROTEGER APRES LES IF ELSE IF
		else
			cmd = ft_add_char(msh, cmd, &e, NULL); // PROTEGER APRES LES IF ELSE IF
		if (msh->status == 255) // IF MALLOC KO return NULL
				return (NULL);
	}
	return (cmd);
}
