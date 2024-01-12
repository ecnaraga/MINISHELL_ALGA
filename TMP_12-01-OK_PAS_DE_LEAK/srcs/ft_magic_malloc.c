/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_magic_malloc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 12:18:58 by galambey          #+#    #+#             */
/*   Updated: 2024/01/11 15:04:37 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	*ft_magic_add_mlc(t_msh *msh, t_list **mlc, int rule, t_magic *p)
{
	t_list	*head;
	t_list	*tmp;

	head = *mlc;
	if (rule == MLC)
		tmp = ft_lstnew_malloc(p->size);
	else
		tmp = ft_lstnew_add(p->addr);
	if (!tmp)
		return (write(2, "minishell: Cannot allocate memory\n", 35),
			msh->status = 255, NULL);
	if (*mlc)
	{
		while ((*mlc)->next)
			(*mlc) = (*mlc)->next;
		(*mlc)->next = tmp;
		*mlc = head;
	}
	else
		*mlc = tmp;
	free(p);
	return (tmp->content);
}

static void	ft_clearlst(int lst, t_list **mlc, t_list **mlc_env,
		t_list **mlc_pip)
{
	if (lst == NO_ENV)
		ft_lstclear(mlc, del);
	else if (lst == ENV)
		ft_lstclear(mlc_env, del);
	else if (lst == PIP)
		ft_lstclear(mlc_pip, del);
}

static void	ft_clearall(t_list **mlc, t_list **mlc_env, t_list **mlc_pip)
{
	ft_lstclear(mlc, del);
	ft_lstclear(mlc_env, del);
	ft_lstclear(mlc_pip, del);
}

/*
size == 1 => signifie regle ADD select
*/
t_magic	*mlcp(void *addr, size_t size)
{
	t_magic	*param;

	if (!addr && size == 0)
		return (NULL);
	param = malloc(sizeof(t_magic));
	if (!param)
	{
		if (size == 1 && addr)
			free(addr);
		return (NULL);
	}
	param->addr = addr;
	param->size = size;
	return (param);
}

/*
Garbagge collector : Store in a linked list all the address malloc
	mlc_env : Contains malloc addresses containing the environment
	mlc : Contains the other malloc addresses in the program
5 rules:
	- Malloc : MLC, adds the address to the linked list and returns the
		address of the malloc element.
	- Add : adds a malloc element to the linked list and returns it
	- Free : free a malloc element and remove it from the linked list
	- Flush : free all except elements containing the environment
	- Quit : free all
rule : choice of the rule
size : if the rule asked is MLC, size of the element to malloc
addr : if the rule is add or free, address of the elemnt sent
lst : if the rule is add or MLC, linked list concerning
*/
void	*mcgic(t_magic *p, int rule, int lst, t_msh *msh)
{
	static t_list	*mlc;
	static t_list	*mlc_pip;
	static t_list	*mlc_env;

	if (!p && (rule == MLC || rule == ADD))
		return (msh->status = 255, NULL);
	if (!p && rule == FREE)
		return (NULL);
	if ((rule == MLC || rule == ADD) && lst == NO_ENV)
		return (ft_magic_add_mlc(msh, &mlc, rule, p));
	else if ((rule == MLC || rule == ADD) && lst == ENV)
		return (ft_magic_add_mlc(msh, &mlc_env, rule, p));
	else if ((rule == MLC || rule == ADD) && lst == PIP)
		return (ft_magic_add_mlc(msh, &mlc_pip, rule, p));
	else if (rule == FREE && lst == NO_ENV)
		ft_list_remove_if(&mlc, p, ft_check);
	else if (rule == FREE && lst == ENV)
		ft_list_remove_if(&mlc_env, p, ft_check);
	else if (rule == FREE && lst == PIP)
		ft_list_remove_if(&mlc_pip, p, ft_check);
	else if (rule == FLUSH)
		ft_clearlst(lst, &mlc, &mlc_env, &mlc_pip);
	else
		ft_clearall(&mlc, &mlc_env, &mlc_pip);
	return (NULL);
}
