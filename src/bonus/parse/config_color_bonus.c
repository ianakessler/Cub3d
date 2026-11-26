/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_color_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaratang <iaratang@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:00:00 by iaratang          #+#    #+#             */
/*   Updated: 2026/11/17 12:00:00 by iaratang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

/*
** Le um componente: sep* digitos sep*. Apenas 1 a 3 digitos e valor
** <= 255 (sem sinal, sem atoi). Avanca *s ate depois dos separadores.
*/
static int	parse_component(const char **s, int *value)
{
	int	digits;

	*s = skip_sep(*s);
	*value = 0;
	digits = 0;
	while (ft_isdigit(**s) && digits < 4)
	{
		*value = *value * 10 + (**s - '0');
		(*s)++;
		digits++;
	}
	if (digits == 0 || digits > 3 || *value > 255)
		return (1);
	*s = skip_sep(*s);
	return (0);
}

/*
** info: o que vem depois de F/C. Formato: R ',' G ',' B, espacos/tabs
** aceitos em volta das virgulas (A.5-10). Grava 0xRRGGBB em *dst, que
** vale -1 enquanto nao definido (repeticao e erro).
*/
int	parse_color(const char *info, int *dst, const char *id)
{
	int	rgb[3];
	int	i;

	if (*dst != -1)
		return (error_msg2("duplicate identifier", id));
	i = 0;
	while (i < 3)
	{
		if (parse_component(&info, &rgb[i]) != 0)
			return (error_msg2(id, "color must be R,G,B with each in [0,255]"));
		if (i < 2 && *info != ',')
			return (error_msg2(id, "color must be R,G,B with each in [0,255]"));
		if (i < 2)
			info++;
		i++;
	}
	if (*info != '\0')
		return (error_msg2(id, "color must be R,G,B with each in [0,255]"));
	*dst = (rgb[0] << 16) | (rgb[1] << 8) | rgb[2];
	return (0);
}
