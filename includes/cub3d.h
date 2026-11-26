/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaratang <iaratang@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:00:00 by iaratang          #+#    #+#             */
/*   Updated: 2026/11/17 12:00:00 by iaratang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "libft.h"
# include <fcntl.h>
# include <errno.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <math.h>
# include <sys/time.h>
# include <X11/X.h>
# include <X11/keysym.h>
# include "mlx.h"

# define EXIT_ERR 1
# define READ_CHUNK 4096
/* Limite de linhas/colunas do mapa (evita estouro de int/memoria) */
# define MAP_MAX_DIM 10000

/* Janela/framebuffer (fase06) */
# define WIN_W 1024
# define WIN_H 768
# define WIN_TITLE "cub3D"
/* |plane| = tan(FOV / 2); FOV ~66 graus (fase07, plano mestre D.2) */
# define PLANE_LEN 0.66

/* Raycasting (fase08/09) */
/* "Infinito" do DDA para componente de raio nula (plano mestre D.6) */
# define RAY_INF 1e30
/* Menor distancia perpendicular aceita (evita line_h absurdo / overflow) */
# define MIN_PERP 1e-4
/* Indices de t_game.tex: textura mostrada para raios indo para N/S/E/W */
# define TEX_N 0
# define TEX_S 1
# define TEX_E 2
# define TEX_W 3

/* Movimento (fase10): velocidades por SEGUNDO, independentes do fps */
/* celulas por segundo (W/A/S/D) */
# define MOVE_SPEED 3.0
/* radianos por segundo (setas) */
# define ROT_SPEED 2.0
/* maior dt aceito por quadro, em segundos (evita salto apos minimizar) */
# define DT_MAX 0.1
/* o jogador fica sempre em [POS_EDGE, cols - POS_EDGE] x [.., rows - ..] */
# define POS_EDGE 0.01

/* Linhas cruas do .cub, sem o '\n' final, na ordem (fase02) */
typedef struct s_lines
{
	char	**lines;
	size_t	count;
}	t_lines;

/*
** Contrato parsing -> engine (plano mestre B.3), preenchido por parse_map
** (fase04) e validado por validate_map (fase05):
** - grid: rows strings de exatamente cols chars ('\0' no fim), grid[rows]
**   == NULL. Celulas: '1' parede, '0' chao, ' ' vazio exterior.
** - A celula do spawn e trocada por '0' na grade; a posicao fica em
**   player_row/player_col (0-indexados; linha = y, coluna = x) e a
**   orientacao em player_dir ('N' | 'S' | 'E' | 'W').
** - Apos validate_map: nenhum '0' na borda ou vizinho (4-viz.) de ' ',
**   e todo '0' e alcancavel a partir do spawn.
*/
typedef struct s_map
{
	char	**grid;
	int		rows;
	int		cols;
	int		player_row;
	int		player_col;
	char	player_dir;
}	t_map;

typedef struct s_scene
{
	char	*north_path;
	char	*south_path;
	char	*west_path;
	char	*east_path;
	int		floor_rgb;
	int		ceil_rgb;
	t_map	map;
}	t_scene;

/* Tipo dos callbacks de mlx_hook / mlx_loop_hook (ver mlx/hooks.c) */
typedef int		(*t_mlx_cb)();
typedef void	(*t_fn)(void);

/*
** Imagem da miniLibX (fase06). addr aponta para o buffer da imagem;
** bpp/line_len/endian vem de mlx_get_data_addr (nao assumir 32 bits).
*/
typedef struct s_image
{
	void	*img;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
	int		w;
	int		h;
}	t_image;

/*
** Jogador (fase07), em unidades de CELULA (nunca pixels). Convencao
** A.5-11: x -> direita (coluna), y -> baixo (linha), N = y decrescente.
** |dir| = 1; plane perpendicular a dir, |plane| = PLANE_LEN.
** plane = (-dir_y * k, dir_x * k).
*/
typedef struct s_player
{
	double	x;
	double	y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
}	t_player;

/*
** Um raio por coluna da tela (fase08/09). Variavel local, sem malloc.
** side: 0 = cruzou linha vertical (x inteiro), 1 = horizontal (y inteiro).
** line_h nao e clampado (o mapeamento da textura usa o valor real);
** draw_start/draw_end sao clampados em [0, WIN_H - 1] (inclusivos).
** wall_x: fracao [0, 1) do ponto de impacto ao longo da parede.
*/
typedef struct s_ray
{
	double	camera_x;
	double	dir_x;
	double	dir_y;
	int		map_x;
	int		map_y;
	double	side_dist_x;
	double	side_dist_y;
	double	delta_dist_x;
	double	delta_dist_y;
	int		step_x;
	int		step_y;
	int		side;
	int		hit;
	double	perp_dist;
	int		line_h;
	int		draw_start;
	int		draw_end;
	double	wall_x;
	int		tex_id;
	int		tex_x;
}	t_ray;

/*
** Estado das teclas (fase10): 1 = pressionada, 0 = solta. Os hooks de
** KeyPress/KeyRelease so alteram estes campos; o movimento e aplicado no
** loop (handle_input), proporcional a dt. last_time: instante do ultimo
** quadro (tv_sec == 0 e tv_usec == 0 = ainda nao medido).
*/
typedef struct s_input
{
	int				key_w;
	int				key_a;
	int				key_s;
	int				key_d;
	int				key_left;
	int				key_right;
	struct timeval	last_time;
}	t_input;

/*
** Estado do jogo. t_game e o DONO UNICO da cena: o main faz parse_scene
** direto em game.scene, e game_destroy libera tudo (inclusive a cena).
** tex[TEX_N..TEX_W]: texturas XPM (fase09), liberadas em game_destroy
** antes da janela/display. z_buffer[x]: perp_dist da parede na coluna x
** no ultimo frame (RAY_INF se nao houve parede) - usado pelos sprites
** (fase18). input: estado das teclas e do relogio (fase10).
*/
typedef struct s_game
{
	void		*mlx;
	void		*win;
	t_image		frame;
	t_image		tex[4];
	double		z_buffer[WIN_W];
	t_scene		scene;
	t_player	player;
	t_input		input;
}	t_game;

/* Estado do flood fill de validate/flood_fill.c (fase05) */
typedef struct s_fill
{
	const t_map	*map;
	char		*visited;
	size_t		*stack;
	size_t		top;
}	t_fill;

/* error/error.c */
int		error_msg(const char *msg);
int		error_msg2(const char *what, const char *detail);
int		error_at(const char *what, int row, int col);

/* parse/file.c, parse/file_utils.c */
int		read_lines(const char *path, t_lines *out);
int		split_lines(const char *buf, size_t len, t_lines *out);
void	lines_free(t_lines *l);

/* parse/parse_utils.c */
int		is_sep(char c);
char	*skip_sep(const char *s);
int		is_blank_line(const char *s);
int		is_map_line(const char *s);

/* parse/config.c, parse/config_color.c */
int		parse_config(const t_lines *l, t_scene *s, size_t *map_start);
int		parse_texture(const char *info, char **dst, const char *id);
int		parse_color(const char *info, int *dst, const char *id);

/* parse/map.c, parse/map_utils.c */
int		parse_map(const t_lines *l, size_t start, t_map *map);
int		map_check_chars(const t_lines *l, size_t start, size_t end);
int		map_find_spawn(t_map *m);

/* parse/parse_scene.c */
int		parse_scene(const char *path, t_scene *scene);

/* validate/map.c, validate/flood_fill.c */
int		validate_map(const t_map *map);
int		check_connected(const t_map *map);

/* cleanup/scene_free.c */
void	scene_free(t_scene *scene);

/* mlx/window.c */
int		game_init(t_game *g);
void	game_destroy(t_game *g);

/* mlx/hooks.c */
void	game_run(t_game *g);

/* mlx/pixel.c */
void	put_pixel(t_image *img, int x, int y, int color);
void	image_fill(t_image *img, int color);
int		get_pixel(const t_image *img, int x, int y);

/* game/player.c */
void	player_init(t_player *p, const t_map *map);

/* game/input.c, game/move.c, game/time.c */
int		on_key_press(int keysym, void *param);
int		on_key_release(int keysym, void *param);
int		on_focus_out(void *param);
void	handle_input(t_game *g, double dt);
double	frame_dt(t_input *in);

/* render/dda.c, render/project.c, render/texture.c, render/frame.c */
void	ray_init(t_ray *r, const t_player *p, int x);
int		ray_cast(t_ray *r, const t_map *m);
void	ray_project(t_ray *r, const t_player *p);
int		textures_load(t_game *g);
void	textures_destroy(t_game *g);
void	tex_column(const t_game *g, t_ray *r);
void	render_frame(t_game *g);

#endif
