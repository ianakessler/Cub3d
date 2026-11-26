/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaratang <iaratang@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:00:00 by iaratang          #+#    #+#             */
/*   Updated: 2026/11/17 12:00:00 by iaratang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** Cria o framebuffer (imagem da miniLibX do tamanho da janela). Toda a
** tela e desenhada nele e exibida com um unico mlx_put_image_to_window.
*/
static int	frame_create(t_game *g)
{
	t_image	*f;

	f = &g->frame;
	f->img = mlx_new_image(g->mlx, WIN_W, WIN_H);
	if (!f->img)
		return (error_msg("mlx_new_image failed"));
	f->addr = mlx_get_data_addr(f->img, &f->bpp, &f->line_len, &f->endian);
	if (!f->addr || f->bpp < 8 || f->line_len <= 0)
		return (error_msg("mlx_get_data_addr failed"));
	f->w = WIN_W;
	f->h = WIN_H;
	image_fill(f, 0x000000);
	return (0);
}

/*
** Pre-condicao: g zerado (ft_bzero) e g->scene ja preenchida e validada.
** As texturas sao carregadas antes da janela: textura invalida da
** Error\n sem chegar a abrir a janela. O 1o quadro ja e desenhado aqui,
** para o primeiro expose mostrar a cena (e nao um frame preto).
** Retorna 0 em sucesso; != 0 em erro ja reportado. Em falha, o que foi
** criado continua em g e game_destroy(g) libera tudo com seguranca.
*/
int	game_init(t_game *g)
{
	g->mlx = mlx_init();
	if (!g->mlx)
		return (error_msg("mlx_init failed: cannot connect to the X server"));
	if (textures_load(g) != 0)
		return (EXIT_ERR);
	g->win = mlx_new_window(g->mlx, WIN_W, WIN_H, WIN_TITLE);
	if (!g->win)
		return (error_msg("mlx_new_window failed"));
	if (frame_create(g) != 0)
		return (EXIT_ERR);
	player_init(&g->player, &g->scene.map);
	render_frame(g);
	return (0);
}

/*
** Libera em ordem inversa (B.4): texturas -> imagem -> janela ->
** display -> mlx -> cena. Cada ponteiro e checado e zerado: seguro
** apos falha parcial e idempotente.
*/
void	game_destroy(t_game *g)
{
	textures_destroy(g);
	if (g->mlx && g->frame.img)
		mlx_destroy_image(g->mlx, g->frame.img);
	g->frame.img = NULL;
	g->frame.addr = NULL;
	if (g->mlx && g->win)
		mlx_destroy_window(g->mlx, g->win);
	g->win = NULL;
	if (g->mlx)
	{
		mlx_destroy_display(g->mlx);
		free(g->mlx);
	}
	g->mlx = NULL;
	scene_free(&g->scene);
}
