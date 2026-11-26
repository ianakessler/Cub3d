/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaratang <iaratang@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:00:00 by iaratang          #+#    #+#             */
/*   Updated: 2026/11/17 12:00:00 by iaratang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** Campo de t_input correspondente ao keysym, ou NULL se a tecla nao e
** tratada. A miniLibX entrega o keysym do nivel 0 (sempre minusculo).
*/
static int	*key_slot(t_input *in, int keysym)
{
	if (keysym == XK_w)
		return (&in->key_w);
	if (keysym == XK_a)
		return (&in->key_a);
	if (keysym == XK_s)
		return (&in->key_s);
	if (keysym == XK_d)
		return (&in->key_d);
	if (keysym == XK_Left)
		return (&in->key_left);
	if (keysym == XK_Right)
		return (&in->key_right);
	return (NULL);
}

/*
** KeyPress: ESC encerra o mlx_loop (a destruicao e feita uma unica vez
** no main); as demais teclas so marcam o estado. Nada se move aqui: o
** movimento e feito no loop, proporcional a dt (independe do auto-repeat).
*/
int	on_key_press(int keysym, void *param)
{
	t_game	*g;
	int		*slot;

	g = param;
	if (keysym == XK_Escape)
	{
		mlx_loop_end(g->mlx);
		return (0);
	}
	slot = key_slot(&g->input, keysym);
	if (slot)
		*slot = 1;
	return (0);
}

int	on_key_release(int keysym, void *param)
{
	t_game	*g;
	int		*slot;

	g = param;
	slot = key_slot(&g->input, keysym);
	if (slot)
		*slot = 0;
	return (0);
}

/*
** Ao perder o foco o KeyRelease pode nunca chegar: solta todas as teclas
** para o jogador nao continuar andando/girando sozinho.
*/
int	on_focus_out(void *param)
{
	t_game	*g;

	g = param;
	g->input.key_w = 0;
	g->input.key_a = 0;
	g->input.key_s = 0;
	g->input.key_d = 0;
	g->input.key_left = 0;
	g->input.key_right = 0;
	return (0);
}
