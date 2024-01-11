/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: galambey <galambey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 10:34:22 by garance           #+#    #+#             */
/*   Updated: 2024/01/11 11:11:50 by galambey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <fcntl.h>
# include <stdarg.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 50
# endif

/*ALLOC*/
char					*ft_strdup(const char *s);
void					*ft_calloc(size_t nmemb, size_t size);

/*LINKED-_LIST*/
typedef struct s_list
{
	void				*content;
	struct s_list		*next;
}						t_list;

void					ft_lstadd_front(t_list **lst, t_list *new);
t_list					*ft_lstnew(void *content);
void					ft_lstadd_back(t_list **lst, t_list *new);
t_list					*ft_lstlast(t_list *lst);
void					ft_lstdelone(t_list *lst, void (*del)(void *));
void					ft_lstclear(t_list **lst, void (*del)(void *));
int						ft_lstsize(t_list *lst);
void					ft_lstiter(t_list *lst, void (*f)(void *));
t_list					*ft_lstmap(t_list *lst, void *(*f)(void *),
							void (*del)(void *));

/*DOUBLE_LINKED_LIST*/
typedef struct s_lst_two
{
	struct s_lst_two	*prev;
	void				*data;
	struct s_lst_two	*next;
}						t_lst_two;

typedef struct s_cont
{
	t_lst_two			*first;
	t_lst_two			*last;
	int					size;
}						t_cont;

t_lst_two				*lst_two_addnew(void *data);
void					lst_two_addback(t_cont *cont, t_lst_two *new);
int						lst_two_size(t_cont *cont);
void					lst_two_clear(t_lst_two **lst, void (*del)(void *));
void					lst_two_delone(t_lst_two *lst, void (*del)(void *));
void					lst_two_iter(t_cont *cont, void (*f)(void *));
t_cont					lst_two_map(t_cont cont, void *(*f)(void *),
							void (*del)(void *));

/*NUMBER*/
int						ft_atoi(const char *nptr);
char					*ft_itoa(int n);
void					digit_len(unsigned int *n, int *count,
							unsigned int base);
void					ft_swap_int(int *a, int *b);
int						ft_abs(int n);
double					ft_dabs(double n);
float					ft_fabs(float n);
int						ft_sign(int n);

/*MEMMORY*/
void					*ft_memset(void *s, int c, size_t n);
void					*ft_memchr(const void *s, int c, size_t n);
void					*ft_memcpy(void *dest, const void *src, size_t n);
int						ft_memcmp(const void *s1, const void *s2, size_t n);
void					ft_bzero(void *s, size_t n);
void					*ft_memmove(void *dest, const void *src, size_t n);

/*PRINT*/
void					ft_putchar_fd(char c, int fd);
void					ft_putstr_fd(char *s, int fd);
void					ft_putendl_fd(char *s, int fd);
void					ft_putnbr_fd(int n, int fd);

/*		-GET_NEXT_LINE*/
char					*get_next_line(int fd);
char					*get_next_line_multiple_fd(int fd);
size_t					ft_strlen_gnl(const char *s);
char					*ft_strjoin_gnl(char const *s1, char const *s2, int i);
void					ft_strcpy_gnl(char *dst, const char *src);

/*		-FT_PRINTF*/
typedef struct a_list
{
	void				*content;
	int					len;
	char				*flag;
	long int			nb;
	long int			prec;
}						t_aglist;

typedef int				(*t_print)(t_aglist *ag);

int						ft_printf(const char *s, ...);
int						ft_arg(va_list ap, char *s, int *count);
int						ft_make_arg(t_aglist *ag, char *s, int *i);
int						ft_free_arg(t_aglist *ag, int result);
long int				ft_atoi_printf(char *s);
int						ck_front_flag(t_aglist *ag, unsigned int j, int id);
int						ck_back_flag(t_aglist *ag, int j);
int						print_char(t_aglist *ag);
int						ft_print_char(void *c);
int						print_str(t_aglist *ag);
int						print_ptr(t_aglist *ag);
int						print_int(t_aglist *ag);
int						print_unsignedint(t_aglist *ag);
int						print_lowhexa(t_aglist *ag);
int						print_upphexa(t_aglist *ag);

/*STRING*/
size_t					ft_strlen(const char *s);
size_t					ft_strlen_bfr_char(const char *str, char c);
size_t					ft_strlen_afr_char(const char *str, char c);
int						ft_arraylen(char **array);
char					*ft_strcpy(char *dst, const char *src);
size_t					ft_strlcpy(char *dst, const char *src, size_t size);
char					*ft_strcat(char *dst, const char *src);
size_t					ft_strlcat(char *dst, const char *src, size_t size);
int						ft_strcmp(const char *s1, const char *s2);
int						ft_strncmp(const char *s1, const char *s2, size_t n);
char					**ft_split(char const *s, char c);
char					**ft_split_isspace(char const *s);
void					*ft_free_split(char **strs);
char					*ft_strjoin(char const *s1, char const *s2);
char					*ft_strjoin_bfr_char(char const *s1, char const *s2,
							char c);
char					*ft_strjoin_aftr_char(char const *s1, char const *s2,
							char c);
char					*ft_strjoin_char(char const *s1, char c);
char					*ft_strtrim(char const *s1, char const *set);
char					*ft_strtrim_exept_set(char const *s1, char const *set);
char					*ft_strtrim_except_tips(char const *s1,
							char const *set, int rule);
char					*ft_substr(char const *s, unsigned int start,
							size_t len);
void					ft_striteri(char *s, void (*f)(unsigned int, char *));
char					*ft_strmapi(char const *s, char (*ft)(unsigned int,
								char));
char					*ft_strchr(const char *s, int c);
char					*ft_strrchr(const char *s, int c);
char					*ft_strnstr(const char *big, const char *little,
							size_t len);
int						ft_is_char(char *buffer, char c);
int						ck_char(char *s, char c);
int						ck_sset(char *s, char *set);
void					ft_replace_char(char *str, char to_replace, char new);
char					*ft_random_string(int n);

/*FILE*/
char					*ft_random_filename(char *path, int n);

/*TEST*/
int						ft_isalpha(int c);
int						ft_isdigit(int c);
int						ft_isalnum(int c);
int						ft_isascii(int c);
int						ft_isprint(int c);
int						ft_toupper(int c);
int						ft_tolower(int c);
int						ft_islower(int c);
int						ft_isupp(int c);

#endif
