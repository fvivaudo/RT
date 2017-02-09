/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvivaudo <fvivaudo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/01/09 10:15:07 by fvivaudo          #+#    #+#             */
/*   Updated: 2015/01/09 10:15:08 by fvivaudo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <stdarg.h>
# include <wchar.h>
# include <unistd.h>
# include <stdint.h>
# include <libft.h>
# include <stdlib.h>

typedef struct	s_prec
{
	int			zero;
	int			prec;
	int			tmp;
	int			min;
	int			max;
	int			type;
}				t_prec;

typedef struct	s_isize
{
	int			*s;
	int			i;
	int			tmp;
	int			*g_size;
	int			*why;
}				t_isize;

typedef struct	s_pad
{
	const char	*format;
	char		c[2];
	int			i;
	int			y;
	int			opt;
	t_prec		pos;
}				t_pad;

typedef struct	s_spec
{
	int			s;
	int			x;
	int			y;
	int			ind;
}				t_spec;

typedef struct	s_op10
{
	int			total;
	char		*to;
	char		*s_n;
	int			y;
	int			ind;
}				t_op10;

int				countbit(wchar_t tmp);
int				ft_cutbit(unsigned int nb, int n);
int				ft_zero(const char *f, int y);
void			get_lnum(const char *format, int y, t_prec *pos);
t_pad			ft_getpos(const char *format, char *to_free, int i, t_pad p);
t_pad			ft_getpos2(const char *format, char *to_free, int i, t_pad p);
char			*rec_bis(va_list list, const char *format,
char *res, t_isize a);
char			*rec_ter(va_list list, const char *format, char *res,
t_isize a);
char			*ft_padorder2(t_pad p, char *res, char *add, int s);
char			*ft_padorder(t_pad p, char *res, char *add, int s);
char			*ft_suborder(t_pad p, char *r, char *add, int s);
char			*ft_suborder2(t_pad p, char *r, char *add, t_spec sp);
char			*ft_suborder3(t_pad p, char *r, char *add, t_spec l);
char			*ft_padding(char *res, t_prec pos, char *c, int opt);
char			*opt1(va_list list, const char *format, char *res, t_isize a);
char			*opt2(va_list list, const char *format, char *res, t_isize a);
t_pad			opt21(char *to, t_pad p, t_isize a);
char			*opt22(char *to, char *res, t_pad p, t_isize a);
char			*opt3(va_list list, const char *format, char *res, t_isize a);
t_pad			opt31(char *tm, t_pad p, t_isize a);
char			*opt4(va_list list, const char *format, char *res, t_isize a);
char			*opt5(va_list list, const char *format, char *res, t_isize a);
t_pad			opt51(char *to, t_pad p, t_isize a, const char *format);
char			*opt52(char *to, char *res, t_pad p, t_isize a);
char			*opt6(va_list list, const char *format, char *res, t_isize a);
char			*opt61(char *to, char *res, t_pad p, t_isize a);
char			*opt7(va_list list, const char *format, char *res, t_isize a);
char			*opt8(va_list list, const char *format, char *res, t_isize a);
char			*opt81(t_pad p, char *res, t_isize a, char *to);
char			*opt9(va_list list, const char *format, char *res, t_isize a);
char			*opt91(t_pad p, char *res, t_isize a, char *to);
char			*opt10(va_list list, const char *format, char *res, t_isize a);
t_pad			opt102(t_isize a, t_op10 o, t_pad p);
t_op10			opt103(t_op10 o, wchar_t *tmp);
char			*opt11(va_list list, const char *format, char *res, t_isize a);
char			*opt12(va_list list, const char *format, char *res, t_isize a);
int				opt12b(wchar_t	tmp, char *to, int y);
char			*opt13(va_list list, const char *format, char *res, t_isize a);
char			*opt14(va_list list, const char *format, char *res, t_isize a);
char			*opt15(va_list list, const char *format, char *res, t_isize a);
char			*flags(va_list list, const char *format, char *flg,
char *(*gen_res)());
char			*flags2(va_list list, const char *format, char *flg,
char *(*gen_res)());
char			*get_flags(const char *format, int i);
void			get_lprec(const char *form, int y, t_prec *pos);
int				ft_ldiff(char *ignore, const char *searched);
int				ft_lndiff(char *ignore, const char *searched, int n);
char			*rec_option(va_list list, const char *format,
char *res, t_isize a);
char			*rec_option2(const char *format, char *res, t_isize a);

#endif
