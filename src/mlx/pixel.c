/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaratang <iaratang@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:00:00 by iaratang          #+#    #+#             */
/*   Updated: 2026/11/17 12:00:00 by iaratang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** Escreve um pixel no buffer da imagem (nunca na janela). Fora de
** [0, w) x [0, h) e ignorado. Respeita bpp e endian informados pela
** miniLibX; caminho rapido para o caso comum (32 bits, little-endian).
*/
void	put_pixel(t_image *img, int x, int y, int color)
{
	char	*dst;
	int		bytes;
	int		i;

	if (x < 0 || y < 0 || x >= img->w || y >= img->h)
		return ;
	bytes = img->bpp / 8;
	dst = img->addr + (y * img->line_len + x * bytes);
	if (bytes == 4 && img->endian == 0)
	{
		*(unsigned int *)dst = (unsigned int)color;
		return ;
	}
	i = -1;
	while (++i < bytes)
	{
		if (img->endian == 0)
			dst[i] = (char)(((unsigned int)color >> (8 * i)) & 0xFF);
		else
			dst[i] = (char)(((unsigned int)color
						>> (8 * (bytes - 1 - i))) & 0xFF);
	}
}

/* Pinta a imagem inteira com uma cor (0xRRGGBB). */
void	image_fill(t_image *img, int color)
{
	int	x;
	int	y;

	y = -1;
	while (++y < img->h)
	{
		x = -1;
		while (++x < img->w)
			put_pixel(img, x, y, color);
	}
}

/*
** Le um pixel (0xRRGGBB) de uma imagem (textura). Fora dos limites
** devolve 0 (nunca le fora do buffer). Mesmo tratamento de bpp/endian
** de put_pixel.
*/
int	get_pixel(const t_image *img, int x, int y)
{
	const unsigned char	*src;
	unsigned int		c;
	int					bytes;
	int					i;

	if (x < 0 || y < 0 || x >= img->w || y >= img->h)
		return (0);
	bytes = img->bpp / 8;
	src = (const unsigned char *)img->addr + (y * img->line_len + x * bytes);
	if (bytes == 4 && img->endian == 0)
		return ((int)(*(const unsigned int *)src & 0xFFFFFF));
	c = 0;
	i = -1;
	while (++i < bytes)
	{
		if (img->endian == 0)
			c |= (unsigned int)src[i] << (8 * i);
		else
			c = (c << 8) | src[i];
	}
	return ((int)(c & 0xFFFFFF));
}
