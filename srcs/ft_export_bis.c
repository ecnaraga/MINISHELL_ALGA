/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_bis.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 17:47:35 by athiebau          #+#    #+#             */
/*   Updated: 2023/12/05 14:08:24 by athiebau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	ft_print_export(t_msh *minish)
{
	t_env	*tmp;

	tmp = *(minish->export_env);
	while (tmp)
	{
		printf("%s%s\n", (char *)tmp->name, (char *)tmp->content);
		tmp = tmp->next;
	}
}

int	get_statut(char *cmd)
{
	int	i;

	i = -1;
	while(cmd[++i])
	{
		if (cmd[i] == '=')
			if(cmd[i - 1] == '+')
				return (4);
			else
				return (2);
	}
	return (1);	
}

int	valide_key(char *key)
{
	int	i;

	i = 0;
	if (key[0] >= 48 && key[0] <= 57)
		return (0);
	if (ft_isalpha(key[0]) || key[0] == '_')
	{
		while(key[++i] != '=')
		{
			if (ft_isalnum(key[i]) || key[i] == '_')
				i++;
			else
				return (0);		
		}
		return (1);
	}
	else
		return (0);
}