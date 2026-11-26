/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaratang <iaratang@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:00:00 by iaratang          #+#    #+#             */
/*   Updated: 2026/11/17 12:00:00 by iaratang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** O nome precisa terminar exatamente em ".cub" e ter ao menos um caractere
** antes da extensao (rejeita ".cub", "dir/.cub" e "mapa.cub.txt").
*/
static int	has_cub_extension(const char *path)
{
	size_t	len;

	len = ft_strlen(path);
	if (len <= 4)
		return (0);
	if (ft_strncmp(path + len - 4, ".cub", 4) != 0)
		return (0);
	if (path[len - 5] == '/')
		return (0);
	return (1);
}

/*
** Fluxo B.4. game e o dono unico da cena: parse_scene preenche
** game.scene, e game_destroy libera tudo (janela, imagem, display e
** cena) tanto em erro quanto na saida normal (ESC / X vermelho).
*/
int	main(int argc, char **argv)
{
	t_game	game;

	if (argc != 2)
		return (error_msg("usage: ./cub3D <map.cub>"));
	if (!has_cub_extension(argv[1]))
		return (error_msg("invalid map file: expected a *.cub file"));
	ft_bzero(&game, sizeof(game));
	if (parse_scene(argv[1], &game.scene) != 0)
		return (EXIT_ERR);
	if (validate_map(&game.scene.map) != 0 || game_init(&game) != 0)
	{
		game_destroy(&game);
		return (EXIT_ERR);
	}
	game_run(&game);
	game_destroy(&game);
	return (0);
}
