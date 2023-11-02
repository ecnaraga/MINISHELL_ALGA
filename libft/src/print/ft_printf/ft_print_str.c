/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_str.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 11:08:52 by galambey          #+#    #+#             */
/*   Updated: 2023/06/23 16:33:07 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/libft.h"

static void	init_count(char *str, t_aglist *ag, int *count)
{
	if (!str)
		*count = 6;
	else if (ag->prec != -1 && (size_t)ag->prec < ft_strlen(str))
		*count = ag->prec;
	else
		*count = ft_strlen(str);
}

int	print_str(t_aglist *ag)
{
	char	*str;
	int		i;
	int		count;
	int		id;

	str = (char *)ag->content;
	i = 0;
	if (!str && ag->prec > -1 && ag->prec < 6)
	{
		ag->content = "";
		return (print_str(ag));
	}
	else
	{
		init_count(str, ag, &count);
		id = 7;
		count = ck_front_flag(ag, count, id);
		if (!str)
			write(1, "(null)", 6);
		while (str && str[i] && (ag->prec == -1 || ag->prec-- > 0))
			write(1, &str[i++], 1);
		count = ck_back_flag(ag, count);
	}
	return (count);
}
