/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iaratang <iaratang@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:00:00 by iaratang          #+#    #+#             */
/*   Updated: 2026/11/17 12:00:00 by iaratang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** Segundos desde o quadro anterior (gettimeofday), limitado a
** [0, DT_MAX]: depois de minimizar ou de um quadro lento o jogador nao
** "pula". O primeiro quadro (last_time zerado) devolve 0.
*/
double	frame_dt(t_input *in)
{
	struct timeval	now;
	double			dt;

	if (gettimeofday(&now, NULL) != 0)
		return (0.0);
	dt = 0.0;
	if (in->last_time.tv_sec != 0 || in->last_time.tv_usec != 0)
		dt = (double)(now.tv_sec - in->last_time.tv_sec)
			+ (now.tv_usec - in->last_time.tv_usec) / 1000000.0;
	in->last_time = now;
	if (dt < 0.0)
		dt = 0.0;
	if (dt > DT_MAX)
		dt = DT_MAX;
	return (dt);
}
