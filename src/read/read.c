/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lporoshi <lporoshi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 16:58:48 by lporoshi          #+#    #+#             */
/*   Updated: 2024/02/24 18:46:56 by lporoshi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdlib.h>
#include "../../inc/cub3d.h"
#include "../../libft/libft.h"

int	load_map_line(t_data *data, char *line)
{
	data->map.grid = ft_strdup(line);
}

int	load_data_part(t_data *data,char **parts)
{
	if (ft_strcmp(parts[0], "NO") == 1)
		data->map.textures.no = parts[1];
	else if (ft_strcmp(parts[0], "SO") == 1)
		data->map.textures.so = parts[1];
	else if (ft_strcmp(parts[0], "EA") == 1)
		data->map.textures.ea = parts[1];
	else if (ft_strcmp(parts[0], "WE") == 1)
		data->map.textures.we = parts[1];
	else if (parts[0][0] == 'F')
		data->map.floor_col = parts[1];
	else if (parts[0][0] == 'C')
		data->map.floor_col = parts[1];
}

int	load_map(t_data *data, int fd)
{
	char	*line;
	char	**split_line;
	int		i;


	line = get_next_line(fd);
	split_line = ft_split(line, ' ');
	while (get_str_arr_sz(split_line) != 1)
	{
		load_data_part(data, split_line);
		line = get_next_line(fd);
		split_line = ft_split(line, ' ');
	}
	i = 0;
	data->map.grid = ft_calloc(100, sizeof(char *));
	while (get_str_arr_sz(split_line) == 1)
	{
		load_map_line(data, split_line[1]);
		line = get_next_line(fd);
		split_line = ft_split(line, ' ');
		i++;
	}
	return (0);
}

int	parse_map(t_data *data, char *pathname)
{
	int	fd;

	if (pathname == NULL || *pathname == '\0')
		return (EXIT_FAILURE);
	fd = open(pathname, O_RDONLY);
	if (fd < 0)
		exit(1);
	load_map(data, fd);
	load_textures(data, fd);
	close(fd);
	return (EXIT_SUCCESS);
}

