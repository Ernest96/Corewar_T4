/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebitca <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/10/22 13:14:45 by ebitca            #+#    #+#             */
/*   Updated: 2016/10/22 13:14:47 by ebitca           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	signed char	operation;
	size_t		end;
	size_t		current;

	if (dest != src)
	{
		operation = (dest < src ? 1 : -1);
		current = (dest < src ? 0 : n - 1);
		end = (dest < src ? n : -1);
		while (current != end)
		{
			*(((unsigned char*)dest) + current) = *(((unsigned char*)src)
					+ current);
			current = current + operation;
		}
	}
	return (dest);
}
