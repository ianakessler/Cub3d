/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_sprite_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaratang <iaratang@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:00:00 by iaratang          #+#    #+#             */
/*   Updated: 2026/11/17 12:00:00 by iaratang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_bonus.h"

/*
** Um frame: o token de len chars em info vira spr_path[spr_frames]
** (parse_texture confere que o arquivo existe e e legivel). O caminho
** fica em spr_path mesmo em erro: scene_free o libera.
*/
static int	add_frame(const char *info, size_t len, t_scene *s)
{
	char	*tok;
	int		ret;

	if (s->spr_frames >= SPR_MAX_FRAMES)
		return (error_msg2("SP", "too many sprite frames (at most 8)"));
	tok = ft_substr(info, 0, len);
	if (!tok)
		return (error_msg("memory allocation failed"));
	ret = parse_texture(tok, &s->spr_path[s->spr_frames], "SP");
	free(tok);
	if (s->spr_path[s->spr_frames])
		s->spr_frames++;
	return (ret);
}

/*
** Bonus (fase18): SP <frame1> [<frame2> ...] = frames XPM do sprite
** animado, na ordem da animacao (1 a SPR_MAX_FRAMES caminhos separados
** por espacos/tabs). Mesmas regras de NO/SO/WE/EA para cada caminho;
** SP repetido e erro. Opcional, mas exigido se o mapa tiver sprite
** (collect_sprites).
*/
int	parse_sprite_frames(const char *info, t_scene *s)
{
	size_t	len;
	int		ret;

	if (s->spr_frames > 0)
		return (error_msg2("duplicate identifier", "SP"));
	info = skip_sep(info);
	if (*info == '\0')
		return (error_msg2("SP", "missing texture path"));
	while (*info)
	{
		len = 0;
		while (info[len] && !is_sep(info[len]))
			len++;
		ret = add_frame(info, len, s);
		if (ret != 0)
			return (ret);
		info = skip_sep(info + len);
	}
	return (0);
}
