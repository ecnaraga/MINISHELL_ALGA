/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_replace_char.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: garance <garance@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 09:41:17 by garance           #+#    #+#             */
/*   Updated: 2023/09/21 09:49:28 by garance          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	ft_replace_char(char *str, char to_replace, char new)
{
	int	i;

	i = -1;
	if (!str)
		return ;
	while (str[++i])
	{
		if (str[i] == to_replace)
			str[i] = new;
	}
}
