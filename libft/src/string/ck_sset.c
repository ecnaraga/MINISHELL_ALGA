/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ck_sset.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 16:18:50 by galambey          #+#    #+#             */
/*   Updated: 2023/12/18 11:00:20 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Return -1 if there's no char of the set in the string s, otherwise return a
	other value
*/
int	ck_sset(char *s, char *set)
{
	int	i;
	int	j;

	i = -1;
	while (s && s[++i])
	{
		j = -1;
		while (set[++j])
		{
			if (s[i] == set[j])
				return (j);
		}
	}
	return (-1);
}
