/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaratang <iaratang@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 17:49:31 by iaratang          #+#    #+#             */
/*   Updated: 2025/07/30 17:50:27 by iaratang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	unsigned char	*tmp;
	size_t			i;
	size_t			total;

	if (count == 0 || size == 0)
	{
		tmp = malloc(1);
		if (!tmp)
			return (NULL);
		return (tmp);
	}
	total = count * size;
	if (total / size != count)
		return (NULL);
	tmp = malloc(total);
	if (!tmp)
		return (NULL);
	i = 0;
	while (i < total)
	{
		tmp[i] = 0;
		i++;
	}
	return (tmp);
}
