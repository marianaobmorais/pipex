/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariaoli <mariaoli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 14:12:27 by mariaoli          #+#    #+#             */
/*   Updated: 2024/09/09 14:27:33 by mariaoli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_putpnbr_fd(unsigned long n, int fd)
{
	int				count;
	unsigned int	base_len;
	char			*base;

	count = 0;
	base = "0123456789abcdef";
	base_len = ft_strlen(base);
	if (n >= base_len)
	{
		count += ft_putpnbr_fd(n / base_len, fd);
		count += ft_putpnbr_fd(n % base_len, fd);
	}
	else
		count += ft_putchar_fd(base[n], fd);
	return (count);
}

int	ft_putptr_fd(unsigned long p, int fd)
{
	int	count;

	count = 0;
	if (p == 0)
		count += (ft_putstr_fd("(nil)", fd));
	else
	{
		count += ft_putstr_fd("0x", fd);
		count += ft_putpnbr_fd(p, fd);
	}
	return (count);
}

int	ft_putxnbr_fd(unsigned int n, char c, int fd)
{
	int				count;
	unsigned int	base_len;
	char			*base;

	count = 0;
	base = "0123456789abcdef";
	base_len = ft_strlen(base);
	if (n >= base_len)
	{
		count += ft_putxnbr_fd(n / base_len, c, fd);
		count += ft_putxnbr_fd(n % base_len, c, fd);
	}
	else if (c == 'x')
		count += ft_putchar_fd(base[n], fd);
	else if (c == 'X')
		count += ft_putchar_fd(ft_toupper(base[n]), fd);
	return (count);
}

int	ft_putunbr_fd(unsigned int n, int fd)
{
	int	count;

	count = 0;
	if (n > 9)
	{
		count += ft_putunbr_fd(n / 10, fd);
		count += ft_putunbr_fd(n % 10, fd);
	}
	else
		count += ft_putchar_fd(n + '0', fd);
	return (count);
}

static int	ft_parse_fmt_fd(va_list ap, const char fmt, int fd)
{
	int	count;

	count = 0;
	if (fmt == 'c')
		count += ft_putchar_fd(va_arg(ap, int), fd);
	if (fmt == 's')
		count += ft_putstr_fd(va_arg(ap, char *), fd);
	if (fmt == 'd' || fmt == 'i')
		count += ft_putnbr_fd(va_arg(ap, int), fd);
	if (fmt == 'u')
		count += ft_putunbr_fd(va_arg(ap, unsigned int), fd);
	if (fmt == 'x' || fmt == 'X')
		count += ft_putxnbr_fd(va_arg(ap, unsigned int), fmt, fd);
	if (fmt == 'p')
		count += ft_putptr(va_arg(ap, unsigned long));
	if (fmt == '%')
		count += ft_putchar_fd('%', fd);
	return (count);
}

int	ft_fprintf(int fd, const char *format, ...)
{
	va_list		ap;
	const char	*fmt;
	int			res;

	va_start(ap, format);
	fmt = format;
	res = 0;
	while (*fmt != '\0')
	{
		if (*fmt == '%' && *(fmt + 1) != '\0')
		{
			fmt++;
			res += ft_parse_fmt_fd(ap, *fmt, fd);
		}
		else
			res += ft_putchar_fd(*fmt, fd);
		fmt++;
	}
	va_end(ap);
	return (res);
}
