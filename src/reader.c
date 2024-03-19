/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lporoshi <lporoshi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 16:58:48 by lporoshi          #+#    #+#             */
/*   Updated: 2024/03/19 17:37:34 by lporoshi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdlib.h>
#include <math.h>
#include "cub3d.h"
#include "config.h"
#include "libft.h"

int	ft_atoi_cub(char *s)
{
	int	res;

	if (!s || *s < '0' || *s > '9')
		return (-1);
	res = 0;
	while (*s >= '0' && *s <= '9')
	{
		res *= 10;
		res += *(s++) - '0';
		if (res > 255)
			return (-1);
	}
	if (*s != '\0')
		return (-1);
	return (res);
}

static void	print_error(const char *message)
{
	write(STDERR_FILENO, "Error\n", 6);
	if (message == NULL)
	{
		write(STDERR_FILENO, "cub3d: FATAL ERROR\n", 13);
		return ;
	}
	write(STDERR_FILENO, "cub3d: ", 7);
	ft_putendl_fd((char *)message, STDERR_FILENO);
}

void	terminate(t_data *data, const char *err_message)
{
	(void)data;
	print_error(err_message);
	exit(1);
}

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

int	save_map_map_info(t_data *data, char *line, int fd)
{
	char	**split;

	split = ft_split(line, ' ');
	if (get_str_arr_sz(split) != 2)
		return (-1);
	printf(" [%s] [%s]\n", split[0], split[1]);
	if (!ft_strcmp(split[0], "NO"))
		data->textures.no_path = split[1];
	else if (!ft_strcmp(split[0], "SO"))
		data->textures.so_path = split[1];
	else if (!ft_strcmp(split[0], "EA"))
		data->textures.ea_path = split[1];
	else if (!ft_strcmp(split[0], "WE"))
		data->textures.we_path = split[1];
	else if (!ft_strcmp(split[0], "F"))
		data->map.floor_col = parse_color(data, ft_substitute_c(line, ',', ' '), fd);
	else if (!ft_strcmp(split[0], "C"))
		data->map.sky_col = parse_color(data, ft_substitute_c(line, ',', ' '), fd);
	else
		return (-1);
	return (0);
}

void pr(t_data *data)
{
	char **p = data->map.grid;
	printf("%d %d\n", data->map.h, data->map.w);
	while (*p != NULL)
	{
		printf("[%s]\n", p[0]);
		p++;
	}
}

char	*ft_str_extend(char *s, char filler, int reslen)
{
	char	*newstr;
	int		i;

	i = 0;
	newstr = ft_calloc(reslen + 1, sizeof(char));
	while (s[i])
	{
		newstr[i] = s[i];
		i++;
	}
	while (i < reslen)
	{
		newstr[i] = filler;
		i++;
	}
	free(s);
	return (newstr);
}

int	save_map_grid(t_data *data, char *line, int fd)
{
	int	temp;

	data->map.h = ft_strlen(line);
	temp = data->map.h;
	data->map.w= 0;
	data->map.grid = ft_calloc(temp + 1, sizeof(char *));
	int i = 0;
	while (line && line[0] != '\0')
	{
		if (i == temp - 1)
		{
			data->map.grid = ft_realloc(data->map.grid, \
						temp * sizeof(char *), \
						temp * sizeof(char *) * 2);
			temp *= 2;
		}
		ft_printf("Saving [%s] to map\n", line);
		if (!ft_consists_of(line, MAP_CHARSET))
		{
			free(line);
			return (-1);
		}
		data->map.grid[i++] = line;
		if (ft_strlen(line) > data->map.h)
		{
			data->map.h = ft_strlen(line);
			temp = data->map.h;
			ft_printf("Now map width is %d\n", data->map.h);
		}
		data->map.w += 1;
		line = ft_strtrim(get_next_line(fd), "\n");
		//ft_printf("I read [%s] to map\n", line);
	}
	return (1);
}
void	print_map2(t_data *data)
{
	int	i;

	i = 0;
	while (data->map.grid[i])
	{
		ft_printf("NORM: [%s]\n", data->map.grid[i]);
		i++;
	}
}
void	normalize_map(t_data *data)
{
	int	i;

	i = 0;
	while (data->map.grid[i])
	{
		if (ft_strlen(data->map.grid[i]) < data->map.h)
			data->map.grid[i] = ft_str_extend(data->map.grid[i], ' ', data->map.h);
		i++;
	}
	print_map2(data);
}

int	save_map_metainf(t_data *data, int fd)
{
	char	*line;
	line = ft_strtrim(get_next_line(fd), "\n");
	if (line == NULL)
		return (-1);
	else if ((line[0] =='N' && data->textures.no_path == NULL) || \
		(line[0] =='S' && data->textures.so_path == NULL) || \
		(line[0] =='W' && data->textures.we_path == NULL) || \
		(line[0] =='E' && data->textures.ea_path == NULL) || \
		(line[0] =='F' && data->map.floor_col == 0xFF000000) || \
		(line[0] =='C' && data->map.sky_col == 0xFF000000))
		return (save_map_map_info(data, line, fd));
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




int	save_player_pos(t_data *data, int x, int y)
{
	printf("PLAYER FOUND");
	printf(" %d, %d\n", x, y);
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

int check_players(t_data *data)
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
	printf("Players amount: %d\n", pl_amt);
	return (pl_amt == 0);
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
	print_map2(data);
}

int	load_map(t_data *data, int fd)
{
	int	tmp;

	while (1)
	{
		tmp = save_map_metainf(data, fd);
		if (tmp == -1)
			terminate(data, "Error while reading the map file");
		else if (tmp == 1)
			break;
	}
	close(fd);
	if (data->map.h == -1)
		terminate(data, "Can't read the map data");
	normalize_map(data);
	find_player_pos(data);
	if (!is_map_valid(data))
		terminate(data, "Invalid map");
	replace_spaces(data);
	return (0);
}

int	parse_map(t_data *data, char *pathname)
{
	int	fd;

	data->map.h = -1;
	data->map.floor_col = 0xFF000000;
	data->map.sky_col = 0xFF000000;
	data->textures.no_path = NULL;
	data->textures.so_path = NULL;
	data->textures.we_path = NULL;
	data->textures.ea_path = NULL;
	if (pathname == NULL || *pathname == '\0')
		return (EXIT_FAILURE);
	fd = open(pathname, O_RDONLY);
	if (fd < 0)
		terminate(data, "Can't parse the map");
	if (load_map(data, fd) != 0)
		terminate(data, "Can't load the map");
	return (EXIT_SUCCESS);
}
