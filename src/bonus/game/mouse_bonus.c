/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaratang <iaratang@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:00:00 by iaratang          #+#    #+#             */
/*   Updated: 2026/11/17 12:00:00 by iaratang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

/*
** MotionNotify (G.5): so acumula o deslocamento horizontal em
** input.mouse_dx; a rotacao e aplicada no loop (handle_input), como as
** setas. Sem foco nada acontece. O 1o evento depois de mouse_x = -1 so
** vira referencia, e um salto maior que MOUSE_JUMP (recentralizacao,
** eventos antigos ainda na fila, ponteiro que saiu e voltou) nao gira:
** nunca ha giro em salto, mesmo se o warp nao funcionar (ex.: WSLg).
** Perto da borda o ponteiro volta ao centro (mlx_mouse_move), para o
** giro nao parar quando ele chegaria a borda da janela.
*/
int	on_mouse_move(int x, int y, void *param)
{
	t_game	*g;
	t_input	*in;
	int		d;

	g = param;
	in = &g->input;
	if (!in->focus)
		return (0);
	d = x - in->mouse_x;
	if (in->mouse_x >= 0 && d >= -MOUSE_JUMP && d <= MOUSE_JUMP)
		in->mouse_dx += d;
	in->mouse_x = x;
	if (x < MOUSE_EDGE || x >= WIN_W - MOUSE_EDGE
		|| y < MOUSE_EDGE || y >= WIN_H - MOUSE_EDGE)
	{
		mlx_mouse_move(g->mlx, g->win, WIN_W / 2, WIN_H / 2);
		in->mouse_x = -1;
	}
	return (0);
}

/*
** FocusIn: o mouse passa a girar a visao (FocusOut, em
** game/input_bonus.c, desliga). O cursor NAO e escondido: o
** mlx_mouse_hide desta miniLibX envia ao servidor X um XColor nao
** inicializado e carrega a libXcursor via dlopen (valgrind acusa erro e
** bloco "still reachable" fora do nosso codigo).
*/
int	on_focus_in(void *param)
{
	t_game	*g;

	g = param;
	g->input.focus = 1;
	g->input.mouse_x = -1;
	g->input.mouse_dx = 0;
	return (0);
}
