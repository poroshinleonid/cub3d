/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lporoshi <lporoshi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 14:34:36 by lporoshi          #+#    #+#             */
/*   Updated: 2024/03/20 16:55:09 by lporoshi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdlib.h>
#include <math.h>
#include "cub3d.h"
#include "funcs.h"
#include "config.h"
#include "libft.h"

int	save_map_grid(t_data *data, char *line, int fd)
{
	int	temp;

	data->map.h = ft_strlen(line);
	temp = data->map.h;
	data->map.w = 0;
	data->map.grid = ft_calloc(temp + 1, sizeof(char *));
	while (line && line[0] != '\0')
	{
		if (data->map.w == temp - 1)
		{
			data->map.grid = ft_realloc(data->map.grid, \
			temp * sizeof(char *), (temp * 2 + 1) * sizeof(char *));
			temp *= 2;
		}
		if (!ft_consists_of(line, MAP_CHARSET))
			return (-1);
		data->map.grid[data->map.w++] = line;
		if (ft_strlen(line) > data->map.h)
			data->map.h = ft_strlen(line);
		line = ft_strtrim(get_next_line(fd), "\n");
	}
	if (line)
		return (-1);
	return (1);
}

void	normalize_map(t_data *data)
{
	int	i;

	i = 0;
	while (data->map.grid[i])
	{
		if (ft_strlen(data->map.grid[i]) < data->map.h)
			data->map.grid[i] = \
			ft_str_extend(data->map.grid[i], ' ', data->map.h);
		i++;
	}
}

void	replace_spaces(t_data *data)
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
			if (data->map.grid[i][j] == ' ')
				data->map.grid[i][j] = '1';
			j++;
		}
		i++;
	}
}

int	save_map_metainf(t_data *data, int fd)
{
	char	*line;

	line = ft_strtrim(get_next_line(fd), "\n");
	if (line == NULL)
		return (-1);
	else if ((line[0] == 'N' && data->textures.no_path == NULL) || \
		(line[0] == 'S' && data->textures.so_path == NULL) || \
		(line[0] == 'W' && data->textures.we_path == NULL) || \
		(line[0] == 'E' && data->textures.ea_path == NULL) || \
		(line[0] == 'F' && data->map.floor_col == 0x00000000) || \
		(line[0] == 'C' && data->map.sky_col == 0x00000000))
		return (save_map_info(data, line, fd));
	else if (line[0] == '\0' || ft_consists_of(line, " \t\v\f\r"))
		return (0);
	else if (ft_isdigit(line[0]) || line[0] == ' ')
		return (save_map_grid(data, line, fd));
	else
	{
		free(line);
		close(fd);
		terminate(data, "Can't read the map!!!");
	}
	return (0);
}

int	check_players(t_data *data)
{
	int	pl_amt;
	int	i;
	int	j;

	i = 0;
	j = 0;
	pl_amt = 0;
	while (data->map.grid[i] != NULL)
	{
		j = 0;
		while (data->map.grid[i][j] != '\0')
		{
			if (ft_in(data->map.grid[i][j], MAP_PLAYER_CHARSET))
				pl_amt++;
			j++;
		}
		i++;
	}
	return (pl_amt == 0);
}
