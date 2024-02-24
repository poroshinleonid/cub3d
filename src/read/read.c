/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lporoshi <lporoshi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 16:58:48 by lporoshi          #+#    #+#             */
/*   Updated: 2024/02/24 17:54:20 by lporoshi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdlib.h>
#include "../../inc/cub3d.h"

int	parse_map(t_data *data, char *pathname)
{
	int	fd;

	if (pathname == NULL || *pathname == '\0')
		return (EXIT_FAILURE);
	fd = open(pathname, O_RDONLY);
	if (fd < 0)
		exit(1);
		//die(NULL, fd);
	load_map(data, fd);
	load_textures(data, fd);
	close(fd);
	return (EXIT_SUCCESS);
}

