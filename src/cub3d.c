/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lporoshi <lporoshi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/24 17:53:15 by lporoshi          #+#    #+#             */
/*   Updated: 2024/02/24 18:47:53 by lporoshi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"
#include "../../libft/libft.h"

int	main(int argc, char **argv)
{
	t_data	data;
	
	parse_map(&data, "testmap.cub");
	print_map(&data);
	
}
