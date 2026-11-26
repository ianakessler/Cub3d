/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaratang <iaratang@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:00:00 by iaratang          #+#    #+#             */
/*   Updated: 2026/11/17 19:24:54 by iaratang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_BONUS_H
# define CUB3D_BONUS_H

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
# define WIN_W 800
# define WIN_H 600
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
/* Bonus (fase17): textura da porta (identificador DO) e total de texturas */
# define TEX_D 4
# define TEX_COUNT 5

/* Movimento (fase10): velocidades por SEGUNDO, independentes do fps */
/* celulas por segundo (W/A/S/D) */
# define MOVE_SPEED 3.0
/* radianos por segundo (setas) */
# define ROT_SPEED 2.0
/* maior dt aceito por quadro, em segundos (evita salto apos minimizar) */
# define DT_MAX 0.1
/* o jogador fica sempre em [POS_EDGE, cols - POS_EDGE] x [.., rows - ..] */
# define POS_EDGE 0.01

/*
** Colisao (bonus, fase14): o jogador e um quadrado de meio-lado
** COLL_RADIUS (celulas); encosta na parede a COLL_EPS dela; cada
** subpasso de movimento anda no maximo COLL_STEP por eixo
** (COLL_STEP < 2 * COLL_RADIUS e < 1 - 2 * COLL_RADIUS: nunca atravessa).
*/
# define COLL_RADIUS 0.2
# define COLL_EPS 1e-6
# define COLL_STEP 0.25

/*
** Minimapa (bonus, fase15): canto superior esquerdo, origem (MM_OX,
** MM_OY), no maximo MM_MAX_W x MM_MAX_H pixels. Escala s (pixels por
** celula) = a que faz o mapa inteiro caber, limitada a [MM_MIN_S,
** MM_MAX_S]; se o mapa nao couber com s = MM_MIN_S, a vista rola
** centrada no jogador. Jogador: quadrado de meio-lado MM_PLAYER_R;
** direcao: segmento de MM_DIR_LEN pixels.
*/
# define MM_OX 10
# define MM_OY 10
# define MM_MAX_W 200
# define MM_MAX_H 200
# define MM_MIN_S 6
# define MM_MAX_S 16
# define MM_PLAYER_R 2
# define MM_DIR_LEN 12
# define MM_WALL 0xD0D0D0
# define MM_FLOOR 0x303030
# define MM_PLAYER 0xFF2020
# define MM_DIRCOL 0xFFE000

/*
** Mouse (bonus, fase16): radianos por pixel de deslocamento horizontal.
** Ponteiro a menos de MOUSE_EDGE pixels da borda da janela -> volta ao
** centro (mlx_mouse_move). Salto maior que MOUSE_JUMP pixels entre dois
** eventos (recentralizacao, ponteiro que saiu e voltou) nao gira.
*/
# define MOUSE_SENS 0.003
# define MOUSE_EDGE 100
# define MOUSE_JUMP 250

/*
** Portas (bonus, fase17). No .cub a porta e 'D' (sempre comeca fechada);
** na grade, em tempo de execucao, DOOR_CLOSED = 'D' e DOOR_OPEN = 'd'
** ('d' nunca e aceito no arquivo). A tecla E (DOOR_KEY) alterna a porta
** a frente do jogador, se a face dela estiver a no maximo DOOR_REACH
** celulas na direcao do olhar; nova alternancia so depois de
** DOOR_COOLDOWN segundos (segurar E com auto-repeat nao fica piscando).
*/
# define DOOR_CLOSED 'D'
# define DOOR_OPEN 'd'
# define DOOR_KEY XK_e
# define DOOR_REACH 1.5
# define DOOR_COOLDOWN 0.3
# define MM_DOOR 0xC06A1E
# define MM_DOOR_OPEN 0x5A8A3C

/*
** Sprites (bonus, fase18). No .cub o sprite e a celula SPRITE_CELL ('2'),
** desenhado no centro dela; a celula bloqueia o jogador (cell_blocks) e
** e transparente para os raios. Frames: SP <frame1> [<frame2> ...], ate
** SPR_MAX_FRAMES XPMs, na ordem da animacao; cada frame dura SPR_FRAME_MS
** milissegundos de relogio (gettimeofday), independente do fps. Texel
** transparente: cor "None" do XPM (a miniLibX grava 0xFF000000) ou a
** cor-chave SPR_KEY (magenta). Profundidade menor que SPR_MIN_DEPTH (rente
** ao plano da camera ou atras dele) nao e desenhada. MM_SPRITE: cor da
** celula do sprite no minimapa.
*/
# define SPRITE_CELL '2'
# define SPR_MAX_FRAMES 8
# define SPR_FRAME_MS 150
# define SPR_MIN_DEPTH 0.1
# define SPR_KEY 0xFF00FF
# define MM_SPRITE 0xC040C0

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
**   == NULL. Celulas: '1' parede, '0' chao, ' ' vazio exterior; bonus:
**   'D' porta fechada / 'd' porta aberta (o .cub so tem 'D').
** - A celula do spawn e trocada por '0' na grade; a posicao fica em
**   player_row/player_col (0-indexados; linha = y, coluna = x) e a
**   orientacao em player_dir ('N' | 'S' | 'E' | 'W').
** - Apos validate_map: nenhum '0' na borda ou vizinho (4-viz.) de ' ',
**   e todo '0' e alcancavel a partir do spawn. Bonus: porta conta como
**   caminhavel nessas checagens e fica entre duas paredes alinhadas.
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

/*
** Sprite (bonus, fase18): centro (x, y) em celulas; dist = quadrado da
** distancia ao jogador (recalculado a cada quadro, so para ordenar);
** phase = deslocamento do frame da animacao (sprites vizinhos nao ficam
** sincronizados).
*/
typedef struct s_sprite
{
	double	x;
	double	y;
	double	dist;
	int		phase;
}	t_sprite;

/*
** Bonus: door_path (DO, fase17); spr_path[0..spr_frames) (SP, fase18);
** sprites[0..spr_count): alocado UMA vez no parsing (collect_sprites),
** liberado por scene_free; reordenado (nao realocado) a cada quadro.
*/
typedef struct s_scene
{
	char		*north_path;
	char		*south_path;
	char		*west_path;
	char		*east_path;
	char		*door_path;
	char		*spr_path[SPR_MAX_FRAMES];
	int			spr_frames;
	t_sprite	*sprites;
	int			spr_count;
	int			floor_rgb;
	int			ceil_rgb;
	t_map		map;
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
** Mouse (bonus, fase16): focus = 1 entre FocusIn e FocusOut; mouse_x =
** ultimo x do ponteiro (-1 = desconhecido: o proximo evento so serve de
** referencia); mouse_dx = deslocamento horizontal acumulado desde o
** ultimo quadro (handle_input o converte em rotacao e zera).
** Portas (bonus, fase17): key_use = 1 quando E foi pressionada desde o
** ultimo quadro (door_update consome); use_cd = segundos que faltam para
** aceitar a proxima alternancia.
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
	int				focus;
	int				mouse_x;
	int				mouse_dx;
	int				key_use;
	double			use_cd;
}	t_input;

/*
** Estado do jogo. t_game e o DONO UNICO da cena: o main faz parse_scene
** direto em game.scene, e game_destroy libera tudo (inclusive a cena).
** tex[TEX_N..TEX_D]: texturas XPM (fase09; TEX_D so se ha porta),
** liberadas em game_destroy
** antes da janela/display. z_buffer[x]: perp_dist da parede na coluna x
** no ultimo frame (RAY_INF se nao houve parede) - usado pelos sprites
** (fase18). input: estado das teclas e do relogio (fase10).
** spr_tex[0..scene.spr_frames): frames do sprite (bonus, fase18),
** liberados junto com tex.
*/
typedef struct s_game
{
	void		*mlx;
	void		*win;
	t_image		frame;
	t_image		tex[TEX_COUNT];
	t_image		spr_tex[SPR_MAX_FRAMES];
	double		z_buffer[WIN_W];
	t_scene		scene;
	t_player	player;
	t_input		input;
}	t_game;

/*
** Vista do minimapa de um quadro (fase15; variavel local, sem malloc).
** s: pixels por celula; w, h: tamanho da vista em pixels; ox, oy: canto
** superior esquerdo da vista no mapa ampliado (em pixels: a celula
** (col, row) ocupa [col * s, col * s + s) x [row * s, row * s + s));
** img: o framebuffer.
*/
typedef struct s_minimap
{
	t_image	*img;
	int		s;
	int		w;
	int		h;
	int		ox;
	int		oy;
}	t_minimap;

/*
** Projecao de um sprite num quadro (fase18; variavel local). tex: frame
** atual; depth: profundidade (distancia perpendicular, comparavel ao
** z_buffer); left/top/w/h: retangulo na tela, sem clamp (pode ser
** enorme com o sprite perto); x0..x1, y0..y1: parte visivel, clampada a
** janela (inclusivos).
*/
typedef struct s_spr_view
{
	const t_image	*tex;
	double			depth;
	double			left;
	double			top;
	double			w;
	double			h;
	int				x0;
	int				x1;
	int				y0;
	int				y1;
}	t_spr_view;

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
void	put_hline(t_image *img, const int xy[2], int len, int color);

/* game/player.c */
void	player_init(t_player *p, const t_map *map);

/* game/input.c, game/move.c, game/time.c */
int		on_key_press(int keysym, void *param);
int		on_key_release(int keysym, void *param);
int		on_focus_out(void *param);
int		on_focus_in(void *param);
int		on_mouse_move(int x, int y, void *param);
void	rotate(t_player *p, double angle);
void	handle_input(t_game *g, double dt);
double	frame_dt(t_input *in);

/* game/collision_bonus.c (fase14) */
int		cell_blocks(const t_game *g, int col, int row);
int		can_move(const t_game *g, double nx, double ny);
void	move_collide(t_game *g, double mx, double my);

/* render/dda.c, render/project.c, render/texture.c, render/frame.c */
void	ray_init(t_ray *r, const t_player *p, int x);
int		ray_cast(t_ray *r, const t_map *m);
void	ray_project(t_ray *r, const t_player *p);
int		textures_load(t_game *g);
void	textures_destroy(t_game *g);
void	tex_column(const t_game *g, t_ray *r);
void	render_frame(t_game *g);

/* validate/door_check_bonus.c, game/door_bonus.c (fase17) */
int		is_door(char c);
int		check_doors(const t_map *map);
int		check_door_texture(const t_scene *scene);
int		door_toggle(t_game *g);
void	door_update(t_game *g, double dt);

/* render/minimap_bonus.c (fase15) */
void	draw_minimap(t_game *g);
int		mm_cell_color(char c);

/* sprites (fase18): parse/ e render/ sprite*_bonus.c, texture_bonus.c */
int		parse_sprite_frames(const char *info, t_scene *s);
int		collect_sprites(t_scene *s);
int		load_xpm(t_game *g, t_image *t, char *path);
void	sort_sprites(t_sprite *spr, int n, const t_player *p);
int		sprite_project(const t_player *p, const t_sprite *s, t_spr_view *v);
void	draw_sprite(t_game *g, const t_spr_view *v);
void	draw_sprites(t_game *g);

#endif
