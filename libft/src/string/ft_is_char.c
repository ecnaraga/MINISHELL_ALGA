/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_char.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 14:54:43 by galambey          #+#    #+#             */
/*   Updated: 2023/06/23 14:56:21 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_is_char(char *buffer, char c)
{
	int	i;

	i = 0;
	if (buffer)
	{
		while (buffer[i] && buffer[i] != c)
			i++;
		if (buffer[i] == c)
			return (i);
	}
	return (-1);
}
