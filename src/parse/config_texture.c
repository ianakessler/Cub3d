/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_texture.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaratang <iaratang@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:00:00 by iaratang          #+#    #+#             */
/*   Updated: 2026/11/17 12:00:00 by iaratang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** Decisao A.5-8: a textura precisa existir e ser legivel ja no parsing.
** open() de um diretorio funciona; o read() de 1 byte detecta EISDIR.
*/
static int	check_texture(const char *path)
{
	int		fd;
	char	c;
	ssize_t	ret;
	int		err;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (error_msg2(path, strerror(errno)));
	ret = read(fd, &c, 1);
	err = errno;
	close(fd);
	if (ret < 0)
		return (error_msg2(path, strerror(err)));
	return (0);
}

/*
** info: o que vem depois do identificador. Formato: sep+ caminho sep*.
** O caminho e um unico token (sem espacos); token extra e erro.
*/
int	parse_texture(const char *info, char **dst, const char *id)
{
	size_t	len;

	if (*dst)
		return (error_msg2("duplicate identifier", id));
	info = skip_sep(info);
	len = 0;
	while (info[len] && !is_sep(info[len]))
		len++;
	if (len == 0)
		return (error_msg2(id, "missing texture path"));
	if (*skip_sep(info + len) != '\0')
		return (error_msg2(id, "unexpected data after texture path"));
	*dst = ft_substr(info, 0, len);
	if (!*dst)
		return (error_msg("memory allocation failed"));
	return (check_texture(*dst));
}
