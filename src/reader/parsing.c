/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lporoshi <lporoshi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 14:33:00 by lporoshi          #+#    #+#             */
/*   Updated: 2024/03/20 15:11:24 by lporoshi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdlib.h>
#include <math.h>
#include "cub3d.h"
#include "funcs.h"
#include "config.h"
#include "libft.h"

int32_t	parse_color(t_data *data, char *line, int fd)
{
	int	r;
	int	g;
	int	b;
	char	**colors;
	colors = ft_split(line, ' ');
	if (colors == NULL)
	{
		free(line);
		close(fd);
		terminate(data, "Can't parse colors!");
	}
	r = ft_atoi_cub(colors[1]);
	g = ft_atoi_cub(colors[2]);
	b = ft_atoi_cub(colors[3]);
	if (r == -1 || g == -1 || b == -1)
	{
		free(line);
		close(fd);
		terminate(data, "Can't parse colors!!!");
	}
	return ((r<<16) + (g<<8) + (b));
}


int	save_player_pos(t_data *data, int x, int y)
{
	data->player.x = (y + 0.5) * MAP_S;
	data->player.y = (x + 0.5) * MAP_S;
	if (data->map.grid[y][x] == 'N')
		data->player.a = M_PI/2.0;
	else if (data->map.grid[y][x] == 'S')
		data->player.a = M_PI + M_PI/2.0;
	else if (data->map.grid[y][x] == 'W')
		data->player.a = M_PI;
	else if (data->map.grid[y][x] == 'E')
		data->player.a = 0.0;
	else
		return (1);
	data->map.grid[y][x] = '0';
	return (0);
}

int	find_player_pos(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	while (data->map.grid[i] != NULL)
	{
		j = 0;
		while (data->map.grid[i][j] != '\0')
		{
			if (ft_in(data->map.grid[i][j], MAP_PLAYER_CHARSET))
			{
				save_player_pos(data, j, i);
			}
			j++;
		}
		i++;
	}
	return (0);
}


int	is_map_valid(t_data *data)
{
	int	i;
	int	j;
	i = 0;
	j = 0;
	while (i < data->map.w)
	{
		j = 0;
		while (j < data->map.h)
		{
			if (!ft_in(data->map.grid[i][j], MAP_CHARSET) || \
					(ft_in(data->map.grid[i][j], MAP_INSIDE_CHARSET) && \
					(	(!i || !j || i == data->map.w - 1 || j == data->map.h - 1) || \
						(ft_in(data->map.grid[i - 1][j], MAP_OUT_CHARSET)) || \
						(ft_in(data->map.grid[i + 1][j], MAP_OUT_CHARSET)) || \
						(ft_in(data->map.grid[i][j + 1], MAP_OUT_CHARSET)) || \
						(ft_in(data->map.grid[i][j - 1], MAP_OUT_CHARSET)) \
					)))
			{
				ft_printf("map invalid: i=%d, j=%d, char=%c\n", i, j, data->map.grid[i][j]);
				return (0);
			}
			j++;
		}
		i++;
	}
	return (check_players(data));
}


int	save_map_info(t_data *data, char *line, int fd)
{
	char	**split;
	int		ret_val;

	split = ft_split(line, ' ');
	ret_val = 0;
	if (get_str_arr_sz(split) != 2)
		ret_val = 1;
	else if (!ft_strcmp(split[0], "NO"))
		data->textures.no_path = ft_strdup(split[1]);
	else if (!ft_strcmp(split[0], "SO"))
		data->textures.so_path = ft_strdup(split[1]);
	else if (!ft_strcmp(split[0], "EA"))
		data->textures.ea_path = ft_strdup(split[1]);
	else if (!ft_strcmp(split[0], "WE"))
		data->textures.we_path = ft_strdup(split[1]);
	else if (!ft_strcmp(split[0], "F"))
		data->map.floor_col = parse_color(data, ft_substitute_c(line, ',', ' '), fd);
	else if (!ft_strcmp(split[0], "C"))
		data->map.sky_col = parse_color(data, ft_substitute_c(line, ',', ' '), fd);
	else
		ret_val = 1;
	free_str_arr(&split);
	return (ret_val);
}
