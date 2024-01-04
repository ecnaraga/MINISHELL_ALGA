/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   magic_malloc.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: athiebau <athiebau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 12:17:56 by galambey          #+#    #+#             */
/*   Updated: 2024/01/04 16:54:26 by athiebau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAGIC_MALLOC_H
# define MAGIC_MALLOC_H

/*
ft_magic_malloc.c
*/
// void *ft_magic_malloc(int rule, size_t size, void *addr, int nb);
void	*mcgic(t_magic *p, int rule, int lst, t_msh *msh);
// void	*mcgic(t_magic *p, size_t size, void *addr, t_msh *msh);
t_magic	*mlcp(void *addr, size_t size);
// t_magic	*mlcp(t_msh *msh, int rule, int lst);

/*
ft_magic_malloc_utils.c
*/
void del(void *content);
int ft_check(void *data, void *data_ref);
t_list	*ft_lstnew_malloc(size_t size);
t_list	*ft_lstnew_add(void *addr);

#endif