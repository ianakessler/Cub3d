NAME		= cub3D

CC			= cc
CFLAGS		= -Wall -Wextra -Werror

SRC_DIR		= src
BONUS_DIR	= $(SRC_DIR)/bonus
OBJ_DIR		= obj
INC_DIR		= includes
LIBFT_DIR	= libft
MLX_DIR		= minilibx

LIBFT		= $(LIBFT_DIR)/libft.a
MLX			= $(MLX_DIR)/libmlx_Linux.a

INCLUDES	= -I$(INC_DIR) -I$(LIBFT_DIR) -I$(MLX_DIR)
LIBS		= -L$(LIBFT_DIR) -lft -L$(MLX_DIR) -lmlx_Linux -lXext -lX11 -lm

HEADERS		= $(INC_DIR)/cub3d.h
HEADERS_BONUS	= $(INC_DIR)/cub3d_bonus.h

SRCS		= $(SRC_DIR)/main.c \
			  $(SRC_DIR)/error/error.c \
			  $(SRC_DIR)/parse/parse_scene.c \
			  $(SRC_DIR)/parse/file.c \
			  $(SRC_DIR)/parse/file_utils.c \
			  $(SRC_DIR)/parse/parse_utils.c \
			  $(SRC_DIR)/parse/config.c \
			  $(SRC_DIR)/parse/config_texture.c \
			  $(SRC_DIR)/parse/config_color.c \
			  $(SRC_DIR)/parse/map.c \
			  $(SRC_DIR)/parse/map_utils.c \
			  $(SRC_DIR)/validate/map.c \
			  $(SRC_DIR)/validate/flood_fill.c \
			  $(SRC_DIR)/cleanup/scene_free.c \
			  $(SRC_DIR)/mlx/window.c \
			  $(SRC_DIR)/mlx/hooks.c \
			  $(SRC_DIR)/mlx/pixel.c \
			  $(SRC_DIR)/game/player.c \
			  $(SRC_DIR)/game/input.c \
			  $(SRC_DIR)/game/move.c \
			  $(SRC_DIR)/game/time.c \
			  $(SRC_DIR)/render/dda.c \
			  $(SRC_DIR)/render/project.c \
			  $(SRC_DIR)/render/texture.c \
			  $(SRC_DIR)/render/frame.c

# Bonus (fase14+): fontes proprios em src/bonus/ (*_bonus.c), header
# includes/cub3d_bonus.h. O binario bonus e montado SO com SRCS_BONUS
# (nenhum objeto da parte obrigatoria entra nele).
SRCS_BONUS	= $(BONUS_DIR)/main_bonus.c \
			  $(BONUS_DIR)/error/error_bonus.c \
			  $(BONUS_DIR)/parse/parse_scene_bonus.c \
			  $(BONUS_DIR)/parse/file_bonus.c \
			  $(BONUS_DIR)/parse/file_utils_bonus.c \
			  $(BONUS_DIR)/parse/parse_utils_bonus.c \
			  $(BONUS_DIR)/parse/config_bonus.c \
			  $(BONUS_DIR)/parse/config_texture_bonus.c \
			  $(BONUS_DIR)/parse/config_color_bonus.c \
			  $(BONUS_DIR)/parse/map_bonus.c \
			  $(BONUS_DIR)/parse/map_utils_bonus.c \
			  $(BONUS_DIR)/parse/config_sprite_bonus.c \
			  $(BONUS_DIR)/parse/sprite_collect_bonus.c \
			  $(BONUS_DIR)/validate/map_bonus.c \
			  $(BONUS_DIR)/validate/flood_fill_bonus.c \
			  $(BONUS_DIR)/validate/door_check_bonus.c \
			  $(BONUS_DIR)/cleanup/scene_free_bonus.c \
			  $(BONUS_DIR)/mlx/window_bonus.c \
			  $(BONUS_DIR)/mlx/hooks_bonus.c \
			  $(BONUS_DIR)/mlx/pixel_bonus.c \
			  $(BONUS_DIR)/game/player_bonus.c \
			  $(BONUS_DIR)/game/input_bonus.c \
			  $(BONUS_DIR)/game/move_bonus.c \
			  $(BONUS_DIR)/game/collision_bonus.c \
			  $(BONUS_DIR)/game/time_bonus.c \
			  $(BONUS_DIR)/game/mouse_bonus.c \
			  $(BONUS_DIR)/game/door_bonus.c \
			  $(BONUS_DIR)/render/dda_bonus.c \
			  $(BONUS_DIR)/render/project_bonus.c \
			  $(BONUS_DIR)/render/texture_bonus.c \
			  $(BONUS_DIR)/render/frame_bonus.c \
			  $(BONUS_DIR)/render/minimap_bonus.c \
			  $(BONUS_DIR)/render/minimap_color_bonus.c \
			  $(BONUS_DIR)/render/sprite_bonus.c \
			  $(BONUS_DIR)/render/sprite_draw_bonus.c

OBJS		= $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
OBJS_BONUS	= $(SRCS_BONUS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# As duas versoes geram o mesmo executavel $(NAME) (decisao G.6). O marcador
# BONUS_MARK existe enquanto $(NAME) for a versao bonus: `make` depois de
# `make bonus` religa a obrigatoria (e apaga o marcador), `make bonus`
# depois de `make` religa o bonus; repetir a mesma regra nao religa nada.
BONUS_MARK	= $(OBJ_DIR)/.bonus_linked

all: $(NAME)

$(NAME): $(LIBFT) $(MLX) $(OBJS) $(if $(wildcard $(BONUS_MARK)),FORCE)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)
	@rm -f $(BONUS_MARK)

bonus: $(BONUS_MARK)

$(BONUS_MARK): $(LIBFT) $(MLX) $(OBJS_BONUS) $(if $(wildcard $(NAME)),,FORCE)
	$(CC) $(CFLAGS) $(OBJS_BONUS) $(LIBS) -o $(NAME)
	@touch $@

$(OBJ_DIR)/bonus/%.o: $(BONUS_DIR)/%.c $(HEADERS_BONUS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(MLX):
	$(MAKE) -C $(MLX_DIR)

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean
	$(MAKE) -C $(MLX_DIR) clean

re: fclean all

FORCE:

.PHONY: all clean fclean re bonus FORCE
