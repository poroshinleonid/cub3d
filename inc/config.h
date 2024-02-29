/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lporoshi <lporoshi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 17:13:01 by lporoshi          #+#    #+#             */
/*   Updated: 2024/02/29 17:43:32 by lporoshi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_H
# define  CONFIG_H

# define EPS 0.000001
# define INF 10000000000.0

# define PI_	3.14159265359
# define PI2_	6.28318530718
# define PI1_2 1.57079632679
# define PI1_3 1.0471975512
# define PI1_4 0.78539816339

# define PI3_2 4.71238898038

# define WIN_WIDTH	1920
# define WIN_WIDTH_DO 1920.0
# define WIN_HEIGHT	1080

# define MAP_CHARSET "01NSWE"
# define MAP_PLAYER_CHARSET "NSWE"
# define TILE_SZ 100
# define SCALE_2D 100
# define TILE_BORDER_SZ 1

# define PL_SPEED_SCALE 0.1
# define PL_ROT_SPEED 0.1

# define PL_SPEED 0.1

# define PI 3.14159265359

# define FOV_ANGLE 1.0471975512
# define FOV_HALFANGLE 0.523599
// ANGLE_STEP = (FOV_ANGLE / WIN_WIDTH) 
// Needs recalculation when changing FOV!
# define ANGLE_STEP 0.00054541539



#endif
