/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frame_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaratang <iaratang@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:00:00 by iaratang          #+#    #+#             */
/*   Updated: 2026/11/17 12:00:00 by iaratang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

/*
** Pinta a linha y inteira com color. Escrita sequencial na memoria (linha
** a linha) e muito mais rapida que coluna a coluna, por causa do cache.
** Caminho rapido para 32 bits little-endian; senao usa put_pixel.
*/
static void	fill_row(t_image *img, int y, int color)
{
	unsigned int	*row;
	int				x;

	x = -1;
	if (img->bpp != 32 || img->endian != 0)
	{
		while (++x < img->w)
			put_pixel(img, x, y, color);
		return ;
	}
	row = (unsigned int *)(img->addr + y * img->line_len);
	while (++x < img->w)
		row[x] = (unsigned int)color;
}

/*
** Fundo do quadro (D.11): metade de cima com o teto (C), metade de baixo
** com o chao (F), linha a linha. As paredes sao desenhadas por cima.
** Coluna sem parede (so com mapa aberto) fica metade teto/metade chao.
*/
static void	draw_background(t_game *g)
{
	int	y;

	y = -1;
	while (++y < WIN_H)
	{
		if (y < WIN_H / 2)
			fill_row(&g->frame, y, g->scene.ceil_rgb);
		else
			fill_row(&g->frame, y, g->scene.floor_rgb);
	}
}

/*
** Parede texturizada da coluna x, entre draw_start e draw_end
** (inclusivos), por cima do fundo. tex_y avanca em passos de
** tex_h / line_h a partir da posicao do topo REAL da parede (line_h sem
** clamp), sempre clampado em [0, tex_h - 1]. O teto acima e o chao abaixo
** da parede ja foram pintados por draw_background.
*/
static void	draw_wall(t_game *g, const t_ray *r, int x)
{
	const t_image	*tex;
	double			step;
	double			tex_pos;
	int				tex_y;
	int				y;

	tex = &g->tex[r->tex_id];
	step = (double)tex->h / r->line_h;
	tex_pos = (r->draw_start - WIN_H / 2 + r->line_h / 2) * step;
	y = r->draw_start;
	while (y <= r->draw_end)
	{
		tex_y = (int)tex_pos;
		if (tex_y >= tex->h)
			tex_y = tex->h - 1;
		put_pixel(&g->frame, x, y++, get_pixel(tex, r->tex_x, tex_y));
		tex_pos += step;
	}
}

/*
** Desenha um quadro inteiro no framebuffer: fundo (teto/chao) e depois
** um raio por coluna. Nao aloca memoria e nao chama a miniLibX: quem
** chama faz o mlx_put_image_to_window. Preenche z_buffer para os
** sprites. Bonus: depois das paredes, os sprites (fase18, ocluidos pelo
** z_buffer) e, por cima de tudo, o minimapa (fase15).
*/
void	render_frame(t_game *g)
{
	t_ray	r;
	int		x;

	draw_background(g);
	x = -1;
	while (++x < WIN_W)
	{
		ray_init(&r, &g->player, x);
		g->z_buffer[x] = RAY_INF;
		if (ray_cast(&r, &g->scene.map))
		{
			ray_project(&r, &g->player);
			tex_column(g, &r);
			g->z_buffer[x] = r.perp_dist;
			draw_wall(g, &r, x);
		}
	}
	draw_sprites(g);
	draw_minimap(g);
}
