/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   horizontal_intersection.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lporoshi <lporoshi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 15:31:02 by lporoshi          #+#    #+#             */
/*   Updated: 2024/03/20 15:31:02 by lporoshi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.h"
#include "cub3d.h"
#include "funcs.h"
#include <math.h>

static t_point	first_horizontal_point(t_data *data, float ra)
{
	t_point	p;

	p.exist = false;
	if (ra == 0 || ra == PI)
		return (p);
	if (ra < PI)
		p.y = ceil_map(data->player.y);
	else
		p.y = floor_map(data->player.y);
	p.x = (p.y - data->player.y) * (1 / tan(ra)) + data->player.x;
	if (p.y < 0 || p.x < 0 || p.y > data->map.h * MAP_S || p.x > data->map.w
		* MAP_S)
		return (p);
	p.exist = true;
	return (p);
}

static t_point	horizontal_delta(float ra)
{
	t_point	delta;

	if (ra < PI)
		delta.y = +MAP_S;
	else
		delta.y = -MAP_S;
	delta.x = (1 / tan(ra)) * delta.y;
	return (delta);
}

static bool	is_horizontal_in_obstacle(t_data *data, float x, float y)
{
	int	map_x;
	int	map_y;

	map_x = (int)x / MAP_S;
	map_y = (int)y / MAP_S;
	if (data->map.grid[map_x][map_y] == '1')
		return (true);
	map_y = (int)y / MAP_S - 1;
	if (map_y >= 0 && data->map.grid[map_x][map_y] == '1')
		return (true);
	return (false);
}

// first actual intersection with map
t_point	horizontal_intersection(t_data *data, float ra)
{
	t_point	p;
	t_point	delta;

	while (ra > 2 * PI)
		ra -= 2 * PI;
	while (ra < 0)
		ra += 2 * PI;
	p = first_horizontal_point(data, ra);
	if (p.exist == false)
		return (p);
	delta = horizontal_delta(ra);
	while (p.y > 0 && p.x > 0 && p.y < data->map.h * MAP_S && p.x < data->map.w
		* MAP_S)
	{
		if (is_horizontal_in_obstacle(data, p.x, p.y))
			return (p);
		p.x += delta.x;
		p.y += delta.y;
	}
	p.exist = false;
	return (p);
}
