/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaratang <iaratang@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:00:00 by iaratang          #+#    #+#             */
/*   Updated: 2026/11/17 12:00:00 by iaratang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** X vermelho: apenas encerra o mlx_loop (mlx_loop_end); a destruicao
** acontece uma unica vez, no main, depois que game_run retorna
** (game_destroy + return 0). ESC faz o mesmo em on_key_press
** (game/input.c). Assim nenhum hook libera memoria nem chama exit.
*/
static int	on_close(void *param)
{
	t_game	*g;

	g = param;
	mlx_loop_end(g->mlx);
	return (0);
}

/* Reapresenta o framebuffer quando a janela volta a ficar visivel (R15) */
static int	on_expose(void *param)
{
	t_game	*g;

	g = param;
	mlx_put_image_to_window(g->mlx, g->win, g->frame.img, 0, 0);
	return (0);
}

/*
** Um quadro: dt desde o quadro anterior -> movimento/rotacao pelas
** teclas pressionadas (fase10) -> render no frame (fase08/09, sem
** alocacao) -> um unico mlx_put_image_to_window.
*/
static int	game_loop(void *param)
{
	t_game	*g;

	g = param;
	handle_input(g, frame_dt(&g->input));
	render_frame(g);
	mlx_put_image_to_window(g->mlx, g->win, g->frame.img, 0, 0);
	return (0);
}

/*
** Registra os hooks e roda o loop de eventos. KeyPress/KeyRelease so
** marcam o estado das teclas em g->input; FocusOut solta todas.
** Retorna quando ESC ou o X vermelho chamam mlx_loop_end. No X11 o X
** vermelho chega como WM_DELETE_WINDOW, que a miniLibX entrega ao hook
** de DestroyNotify.
** mlx.h declara os callbacks como int (*)(): em C23 (gcc 15) isso vira
** int (*)(void). O cast passa por t_fn (void (*)(void)), que o gcc
** aceita sem -Wcast-function-type; compila igual em C17 e C23.
*/
void	game_run(t_game *g)
{
	mlx_hook(g->win, KeyPress, KeyPressMask,
		(t_mlx_cb)(t_fn)on_key_press, g);
	mlx_hook(g->win, KeyRelease, KeyReleaseMask,
		(t_mlx_cb)(t_fn)on_key_release, g);
	mlx_hook(g->win, FocusOut, FocusChangeMask,
		(t_mlx_cb)(t_fn)on_focus_out, g);
	mlx_hook(g->win, DestroyNotify, StructureNotifyMask,
		(t_mlx_cb)(t_fn)on_close, g);
	mlx_expose_hook(g->win, on_expose, g);
	mlx_loop_hook(g->mlx, (t_mlx_cb)(t_fn)game_loop, g);
	mlx_loop(g->mlx);
}
