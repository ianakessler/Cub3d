/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaratang <iaratang@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:00:00 by iaratang          #+#    #+#             */
/*   Updated: 2026/11/17 12:00:00 by iaratang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

/*
** Tamanho da vista (w ou h) e deslocamento (ox ou oy), em pixels, num
** eixo (axis 0 = x, 1 = y). Se o mapa cabe, a vista mostra o eixo
** inteiro a partir de 0; senao rola, centrada no jogador e presa aos
** limites do mapa (nunca mostra alem da borda). Tudo em inteiros: as
** fronteiras das celulas caem sempre em pixels exatos.
*/
static void	mm_axis(t_minimap *mm, int cells, double pos, int axis)
{
	int	len;
	int	off;

	len = MM_MAX_W;
	if (axis)
		len = MM_MAX_H;
	off = 0;
	if (cells * mm->s <= len)
		len = cells * mm->s;
	else
	{
		off = (int)floor(pos * mm->s) - len / 2;
		if (off > cells * mm->s - len)
			off = cells * mm->s - len;
		if (off < 0)
			off = 0;
	}
	if (axis)
		mm->h = len;
	else
		mm->w = len;
	if (axis)
		mm->oy = off;
	else
		mm->ox = off;
}

/* Pixel do minimapa: so dentro da vista (clipping), nunca fora do frame */
static void	mm_put(const t_minimap *mm, int x, int y, int color)
{
	if (x < MM_OX || y < MM_OY || x >= MM_OX + mm->w || y >= MM_OY + mm->h)
		return ;
	put_pixel(mm->img, x, y, color);
}

/*
** Linha j da vista (line = linha da grade correspondente), em trechos
** de uma celula (put_hline), na cor de mm_cell_color (' ' = vazio
** exterior de mapa nao retangular = transparente).
*/
static void	mm_row(const t_minimap *mm, const char *line, int j)
{
	int	x;
	int	col;
	int	len;
	int	color;

	col = mm->ox / mm->s;
	x = 0;
	while (x < mm->w)
	{
		len = (col + 1) * mm->s - mm->ox - x;
		if (len > mm->w - x)
			len = mm->w - x;
		color = mm_cell_color(line[col]);
		if (color >= 0)
			put_hline(mm->img, (const int [2]){MM_OX + x, MM_OY + j},
				len, color);
		x += len;
		col++;
	}
}

/*
** Jogador: segmento de MM_DIR_LEN pixels ao longo de dir e, por cima,
** um quadrado de meio-lado MM_PLAYER_R na posicao (x, y) do jogador.
*/
static void	mm_player(const t_minimap *mm, const t_player *p)
{
	double	cx;
	double	cy;
	int		i;
	int		j;

	cx = MM_OX + p->x * mm->s - mm->ox;
	cy = MM_OY + p->y * mm->s - mm->oy;
	i = -1;
	while (++i <= MM_DIR_LEN)
		mm_put(mm, (int)floor(cx + p->dir_x * i),
			(int)floor(cy + p->dir_y * i), MM_DIRCOL);
	j = -MM_PLAYER_R - 1;
	while (++j <= MM_PLAYER_R)
	{
		i = -MM_PLAYER_R - 1;
		while (++i <= MM_PLAYER_R)
			mm_put(mm, (int)floor(cx) + i, (int)floor(cy) + j, MM_PLAYER);
	}
}

/*
** Minimapa (G.2) no canto superior esquerdo do frame, por cima da cena.
** Escala: a maior que faz o mapa inteiro caber em MM_MAX_W x MM_MAX_H,
** limitada a [MM_MIN_S, MM_MAX_S] pixels por celula. Sem alocacao e sem
** chamadas a miniLibX: so escreve no framebuffer (com clipping).
** Pre-condicao (parser): cada linha da grade tem exatamente cols chars.
*/
void	draw_minimap(t_game *g)
{
	t_minimap	mm;
	const t_map	*m;
	int			j;

	m = &g->scene.map;
	if (m->rows <= 0 || m->cols <= 0)
		return ;
	mm.img = &g->frame;
	mm.s = MM_MAX_W / m->cols;
	if (MM_MAX_H / m->rows < mm.s)
		mm.s = MM_MAX_H / m->rows;
	if (mm.s < MM_MIN_S)
		mm.s = MM_MIN_S;
	if (mm.s > MM_MAX_S)
		mm.s = MM_MAX_S;
	mm_axis(&mm, m->cols, g->player.x, 0);
	mm_axis(&mm, m->rows, g->player.y, 1);
	j = -1;
	while (++j < mm.h)
		mm_row(&mm, m->grid[(mm.oy + j) / mm.s], j);
	mm_player(&mm, &g->player);
}
