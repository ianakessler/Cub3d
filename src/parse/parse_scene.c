/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scene.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaratang <iaratang@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:00:00 by iaratang          #+#    #+#             */
/*   Updated: 2026/11/17 12:00:00 by iaratang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** Orquestra o parsing (B.4): le as linhas (fase02), a configuracao
** (fase03) e o mapa (fase04). scene chega zerado (main faz ft_bzero).
** Retorna 0 em sucesso; != 0 em erro ja reportado, com scene liberado.
** As linhas sao sempre liberadas aqui (E.7). A validacao do fechamento
** do mapa (fase05) e feita depois, por validate_map, no main (B.4).
*/
int	parse_scene(const char *path, t_scene *scene)
{
	t_lines	lines;
	size_t	map_start;
	int		ret;

	scene->floor_rgb = -1;
	scene->ceil_rgb = -1;
	if (read_lines(path, &lines) != 0)
		return (EXIT_ERR);
	map_start = 0;
	ret = parse_config(&lines, scene, &map_start);
	if (ret == 0 && map_start >= lines.count)
		ret = error_msg("missing map: no map after the configuration");
	if (ret == 0)
		ret = parse_map(&lines, map_start, &scene->map);
	lines_free(&lines);
	if (ret != 0)
		scene_free(scene);
	return (ret);
}
