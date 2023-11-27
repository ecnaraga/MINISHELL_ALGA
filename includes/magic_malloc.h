/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   magic_malloc.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 12:17:56 by galambey          #+#    #+#             */
/*   Updated: 2023/11/27 09:59:33 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAGIC_MALLOC_H
# define MAGIC_MALLOC_H

/*
ft_magic_malloc.c
*/
void *ft_magic_malloc(int rule, size_t size, void *addr, int nb);

/*
ft_magic_malloc_utils.c
*/
void del(void *content);
int ft_check(void *data, void *data_ref);
t_list	*ft_lstnew_malloc(size_t size);
t_list	*ft_lstnew_add(void *addr);

#endif