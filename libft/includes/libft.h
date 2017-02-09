/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/11/07 10:30:32 by fvivaudo          #+#    #+#             */
/*   Updated: 2016/11/02 13:26:32 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <string.h>

typedef struct		s_list
{
	void			*content;
	size_t			content_size;
	struct s_list	*next;
}					t_list;

int					ft_printf(const char *format, ...);
/*
** The bzero() function writes n zeroed bytes to the string s.  If n is zero,
** bzero() does nothing.
*/
void				ft_bzero(void *s, size_t n);

/*
** count c occurences in str
*/
int					ft_count(char const *str, char c);
int					ft_ncount(char const *str, char c, int n);
void				ft_putchar(char c);
void				ft_putstr(char *str);
void				ft_putnbr(int nb);
size_t				ft_strlen(char const *str);
int					ft_strcmp(char *s1, char *s2);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
/*
** strcpy() functions copy the string src
** to dst (includingthe terminating `\0' character)
*/
char				*ft_strcpy(char *dst, const char *src);
/*
** copy size - 1 characters
*/
size_t				ft_strlcpy(char *dst, const char *src, size_t size);
char				*ft_strncpy(char *dst, const char *src, size_t n);
/*
** returns pointer to newly allocated string
*/
char				*ft_strdup(char *src);
char				*ft_strndup(char *src, size_t n);
/*
** concatenate 2 strings, s1 needs enough space
*/
char				*ft_strcat(char *s1, const char *s2);
char				*ft_strcatf(const char *s1, char *s2);
char				*ft_strncat(char *s1, const char *s2, size_t n);
char				*ft_strncatf(const char *s1, char *s2, int n);
/*
** tries to concatenate to strings, size = dst + src
*/
size_t				ft_strlcat(char *dst, const char *src, size_t size);
/*
**	ft_strchr renvoie une chaine de caractere partant du premier
**	caractere c rencontre y compris /0
*/
char				*ft_strchr(const char *s, int c);

/*
**	comme ft_strchr, mais renvoie le dernier caractere
*/

char				*ft_strrchr(const char *s, int c);
/*
**	comme ft_strchr, mais cherche une chaine de caractere
*/
char				*ft_strstr(const char *s1, const char *s2);
/*
** only the n first characters of s1 are searched
*/
char				*ft_strnstr(const char *s1, const char *s2, size_t n);
void				ft_swap(int *a, int *b);
void				ft_print_words_tables(char **tab);
/*
** Maj
*/
int					ft_toupper(int c);
char				*ft_strup(char *str);
/*
** min
*/
int					ft_tolower(int c);
/*
** char -> int
*/
int					ft_atoi(const char *str);
/*
** char -> double
*/
double				ft_datoi(const char *str);
int					ft_isalpha(int c);
int					ft_isdigit(int c);
int					ft_isalnum(int c);
int					ft_isascii(int c);
int					ft_isprint(int c);
/*
** Send back the position of the first character not ignored, or -1
*/
int					ft_ldiff(char *ignore, const char *searched);
int					ft_lndiff(char *ignore, const char *searched, int n);
/*
** The memset() function writes len bytes of value c (converted to an unsigned
** char) to the string
*/
void				*ft_memset(void *b, int c, size_t len);
/*
** The memcpy() function copies n bytes from memory area src to memory area
** dst.  If dst and src overlap, behavior is undefined.  Applications in which
** dst and src might overlap should use memmove(3) instead.
*/
void				*ft_memcpy(void *dst, const void *src, size_t n);
/*
** stops the copy when c is encountered
*/
void				*ft_memccpy(void *dst, const void *src, int c, size_t n);
/*
** move len char from src to dst
*/
void				*ft_memmove(void *dst, const void *src, size_t len);
/*
** The memchr() function locates the first occurrence of c (converted to an
** unsigned char) in string s, and returns a pointer to it;
*/
void				*ft_memchr(const void *s, int c, size_t n);
int					ft_memcmp(const void *s1, const void *s2, size_t n);
void				*ft_memalloc(size_t size);
void				ft_memdel(void **ap);
void				*ft_memcatf(const void *src, void *dst, int n, int dst_s);
char				*ft_strnew(size_t size);
void				ft_strdel(char **as);
/*
** sets every char to \0
*/
void				ft_strclr(char *s);
/*
** applies f to every char
*/
void				ft_striter(char *s, void (*f)(char *));
/*
** same, but sends char pos to f
*/
void				ft_striteri(char *s, void (*f)(unsigned int, char *));
/*
** same as striter, but allocates and sends back a modified string
*/
char				*ft_strmap(char const *s, char (*f)(char));
char				*ft_strmapi(char const *s, char (*f)(unsigned int, char));
/*
** return 1 if s1 and s2 are similar
*/
int					ft_strequ(char const *s1, char const *s2);
int					ft_strnequ(char const *s1, char const *s2, size_t n);
void				ft_putendl(char const *s);
void				ft_putchar_fd(char c, int fd);
void				ft_putstr_fd(char const *s, int fd);
void				ft_putendl_fd(char const *s, int fd);
void				ft_putnbr_fd(int n, int fd);
char				*ft_hexa(unsigned long int n);
char				*ft_octal(unsigned long int n);
char				*ft_uns(unsigned long int n);
/*
** si charactere est present, renvoie sa position (> -1)
*/
int					ft_ishere(const char *str, char c);
int					ft_isheren(const char *str, char c, int n);
/*
** int -> char (allocated)
*/
char				*ft_chitoa(int n);
char				*ft_itoa(int n);
char				*ft_litoa(long int n);
char				*ft_llitoa(long long int n);
/*
** returns an allocated copy of a part of s
*/
char				*ft_strsub(char const *s, unsigned int start, size_t len);
/*
** returns one allocated string made out of the fusion of two others
** while freeing one or two of them
*/
char				*ft_strext(char *s, char *ext, int pos);
/*
** returns one allocated string made out of the fusion of two others
*/
char				*ft_strjoin(char const *s1, char const *s2);
/*
** returns a clean chain, without ' ' '\n' 't'
*/
char				*ft_strtrim(char const *s);
/*
** returns several chains, without their separating character(s), c
*/
char				**ft_strsplit(char const *s, char c);
/*
** returns several chains, without spaces or tabs
*/
char				**ft_strsplitspace(char const *s);
/*
** returns several chains, each containing one line of s
*/
char				**ft_strsplitline(char const *s);
/*
** free a char**
*/
void				ft_doubletabfree(char	***array);
/*
** copy a char**
*/
char				**ft_doubletabcpy(char	**src);


double				ft_abs(double num);
double				ft_root(double num);
double				ft_pow(double num, int power);
double				ft_cos(double cx);
double				ft_sin(double x);
double				ft_tan(double x);

void				ft_lstrev(t_list **alst, size_t size);
t_list				*ft_lstnew (void const *content, size_t content_size);
void				ft_lstdel(t_list **alst, void (*del)(void *, size_t));
void				ft_lstadd(t_list **alst, t_list *new);
void				ft_lstnadd(t_list **alst, t_list *new, size_t n);
void				ft_lstdelone(t_list **alst, void (*del)(void*, size_t));
void				ft_lstndelone(t_list **alst, size_t n,
					void (*del)(void*, size_t));
void				ft_lstiter(t_list *lst, void (*f)(t_list *elem));
t_list				*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem));
void				ft_lstaddb(t_list **alst, t_list *new);
t_list				*ft_lstget(t_list *alst, size_t n);

#endif
