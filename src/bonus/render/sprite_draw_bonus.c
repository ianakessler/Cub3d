/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_draw_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaratang <iaratang@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:00:00 by iaratang          #+#    #+#             */
/*   Updated: 2026/11/17 12:00:00 by iaratang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

/*
** Texel (x, y) do frame, ou -1 se transparente: cor "None" do XPM (a
** miniLibX grava 0xFF000000: byte alto nao nulo, caminho 32 bits
** little-endian) ou a cor-chave SPR_KEY (qualquer formato).
*/
static int	texel(const t_image *t, int x, int y)
{
	unsigned int	raw;
	int				c;

	if (t->bpp == 32 && t->endian == 0)
	{
		raw = *(const unsigned int *)(t->addr + y * t->line_len + x * 4);
		if (raw >> 24)
			return (-1);
		c = (int)raw;
	}
	else
		c = get_pixel(t, x, y);
	if (c == SPR_KEY)
		return (-1);
	return (c);
}

/* Coordenada na textura (n texels) do pixel pos de um trecho [start,
** start + size) da tela, clampada a [0, n - 1]. */
static int	tex_coord(int pos, double start, double size, int n)
{
	int	t;

	t = (int)((pos + 0.5 - start) * n / size);
	if (t < 0)
		return (0);
	if (t >= n)
		return (n - 1);
	return (t);
}

/*
** Desenha o sprite projetado (v) no framebuffer, linha a linha (escrita
** sequencial na memoria). Oclusao: a coluna x so recebe o sprite se ele
** estiver mais perto que a parede/porta fechada dali (depth <
** z_buffer[x]). Texels transparentes nao sao desenhados. So escreve em
** [x0, x1] x [y0, y1], ja clampados a janela.
*/
void	draw_sprite(t_game *g, const t_spr_view *v)
{
	int	x;
	int	y;
	int	ty;
	int	c;

	y = v->y0 - 1;
	while (++y <= v->y1)
	{
		ty = tex_coord(y, v->top, v->h, v->tex->h);
		x = v->x0 - 1;
		while (++x <= v->x1)
		{
			if (v->depth >= g->z_buffer[x])
				continue ;
			c = texel(v->tex, tex_coord(x, v->left, v->w, v->tex->w), ty);
			if (c >= 0)
				put_pixel(&g->frame, x, y, c);
		}
	}
}
