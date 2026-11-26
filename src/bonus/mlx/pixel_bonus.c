/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaratang <iaratang@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:00:00 by iaratang          #+#    #+#             */
/*   Updated: 2026/11/17 12:00:00 by iaratang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

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

/*
** Bonus (fase15): segmento horizontal de len pixels a partir de
** xy = {x, y}, recortado aos limites da imagem. Caminho rapido (escrita
** direta na linha) para 32 bits little-endian; senao usa put_pixel.
*/
void	put_hline(t_image *img, const int xy[2], int len, int color)
{
	unsigned int	*row;
	int				x;
	int				end;

	if (xy[1] < 0 || xy[1] >= img->h)
		return ;
	x = xy[0];
	if (x < 0)
		x = 0;
	end = xy[0] + len;
	if (end > img->w)
		end = img->w;
	if (img->bpp != 32 || img->endian != 0)
	{
		while (x < end)
			put_pixel(img, x++, xy[1], color);
		return ;
	}
	row = (unsigned int *)(img->addr + xy[1] * img->line_len);
	while (x < end)
		row[x++] = (unsigned int)color;
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
