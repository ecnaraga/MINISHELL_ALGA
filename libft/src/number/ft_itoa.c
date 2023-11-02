/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 12:13:25 by galambey          #+#    #+#             */
/*   Updated: 2023/06/23 12:59:41 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

static unsigned int	ft_count(int n)
{
	unsigned int	count;
	unsigned int	nb;

	count = 0;
	nb = n;
	if (n == 0)
		return (1);
	if (n < 0)
	{
		count++;
		nb = -n;
	}
	while (nb >= 1)
	{
		nb = nb / 10;
		count++;
	}
	return (count);
}

static void	ft_itoa_recursive(int n, char *s, int i)
{
	unsigned int	nb;

	nb = n;
	if (n < 0)
	{
		s[0] = '-';
		nb = -n;
	}
	if (nb <= 9)
		s[i] = nb + 48;
	else
	{
		ft_itoa_recursive(nb / 10, s, i - 1);
		s[i] = (nb % 10) + 48;
	}
}

char	*ft_itoa(int n)
{
	unsigned int	len;
	char			*s;

	len = ft_count(n);
	s = (char *)malloc(sizeof(char) * (len + 1));
	if (s == NULL)
		return (NULL);
	ft_itoa_recursive(n, s, len - 1);
	s[len] = '\0';
	return (s);
}
