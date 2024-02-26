/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lporoshi <lporoshi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 16:25:31 by lporoshi          #+#    #+#             */
/*   Updated: 2024/02/26 17:09:49 by lporoshi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"
#include "../../libft/libft.h"
#include "../../MLX42/include/MLX42/MLX42.h"

int32_t get_color(t_data *data, int x, int y)
{
	(void)data;
	return (x * y);
}

int32_t get_color_minimap(t_data *data)
{
	int	x;
	int	y;

	x = data->rect.mapx;
	y = data->rect.mapy;
	if (x < 0 || y < 0 || x >= data->map_w|| y >= data->map_h)
		return (0xFFFF00);
	if (data->map.grid[y][x] == '1')
		return (0xFF0000FF);
	if (data->map.grid[y][x] == '0' || ft_in(data->map.grid[y][x], MAP_PLAYER_CHARSET))
		return (0xFFFFFFFF);
	return (0xFFFF00);
}

void	draw_rect(t_data *data, int x, int y)
{
	int	tmpx;
	int32_t	cur_col;

	cur_col = get_color_minimap(data);
	tmpx = x;
	while (y < data->rect.dy)
	{
		x = tmpx;
		while (x < data->rect.dx)
		{
			mlx_put_pixel(data->mlx_img, x, y, cur_col);
			x++;
		}
		y++;
	}
}

void	draw_minimap(t_data *data)
{
	int	x;
	int	y;

	y = 0;
	while (y < 21)
	{
		x = 0;
		while (x < 21)
		{
			data->rect.mapx = x;
			data->rect.mapy = y;
			data->rect.dx = x * TILE_SZ + TILE_SZ - TILE_BORDER_SZ;
			data->rect.dy = y * TILE_SZ + TILE_SZ - TILE_BORDER_SZ;
			draw_rect(data, x  * TILE_SZ + TILE_BORDER_SZ, y * TILE_SZ + TILE_BORDER_SZ);
			x++;
		}
		y++;
	}
}

double	ft_abs(double num)
{
	if (num < 0)
		return (-num);
	return (num);
}

void	verLine(t_data *data, int x, int start, int end, uint32_t color)
{
	int i;

	//printf("%d %d, %u\n", start, end, color);
	i = start;
	while (i <= end)
	{
		mlx_put_pixel(data->mlx_img, x, i, color);
		i++;
	}
}

void	cast_rays(t_data *data)
{
	double posX = data->player.x, posY = data->player.y; 
	double plane_x = data->player.dx, plane_y = data->player.dy; //x and y start position
	ft_printf("cAST\n");
	for(int x = 0; x < WIN_WIDTH; x++)
    {
      //calculate ray position and direction
      double cameraX = 2 * x / WIN_WIDTH_DO - 1.0; //x-coordinate in camera space
      double rayDirX = data->player.dx + plane_x * cameraX;
      double rayDirY = -data->player.dy + plane_y * cameraX;
	//which box of the map we're in
      int mapX = (int)(posX);
      int mapY = (int)(posY);

      //length of ray from current position to next x or y-side
      double sideDistX;
      double sideDistY;

       //length of ray from one x or y-side to next x or y-side
      double deltaDistX = (rayDirX == 0) ? 1e30 : ft_abs(1 / rayDirX);
      double deltaDistY = (rayDirY == 0) ? 1e30 : ft_abs(1 / rayDirY);
      double perpWallDist;

      //what direction to step in x or y-direction (either +1 or -1)
      int stepX;
      int stepY;

      int hit = 0; //was there a wall hit?
      int side; //was a NS or a EW wall hit?
	
      //calculate step and initial sideDist
      if (rayDirX < 0)
      {
        stepX = -1;
        sideDistX = (posX - mapX) * deltaDistX;
      }
      else
      {
        stepX = 1;
        sideDistX = (mapX + 1.0 - posX) * deltaDistX;
      }
      if (rayDirY < 0)
      {
        stepY = -1;
        sideDistY = (posY - mapY) * deltaDistY;
      }
      else
      {
        stepY = 1;
        sideDistY = (mapY + 1.0 - posY) * deltaDistY;
      }

      //perform DDA
      while (hit == 0)
      {
        //jump to next map square, either in x-direction, or in y-direction
        if (sideDistX < sideDistY)
        {
          sideDistX += deltaDistX;
          mapX += stepX;
          side = 0;
        }
        else
        {
          sideDistY += deltaDistY;
          mapY += stepY;
          side = 1;
        }
		if (data->map.grid[mapX][mapY] == '1') hit = 1;
      } 

//Calculate distance projected on camera direction (Euclidean distance would give fisheye effect!)ß
      if(side == 0) perpWallDist = (sideDistX - deltaDistX);
      else          perpWallDist = (sideDistY - deltaDistY);

      //Calculate height of line to draw on screen
      int lineHeight = (int)(WIN_HEIGHT / perpWallDist);

      //calculate lowest and highest pixel to fill in current stripe

      int drawStart = -lineHeight / 2 + WIN_HEIGHT / 2;
      int drawEnd = lineHeight / 2 + WIN_HEIGHT / 2;
	  if(drawStart < 0)drawStart = 0;
      if(drawEnd >= WIN_HEIGHT)drawEnd = WIN_HEIGHT - 2;
	  
      //choose wall color
      uint32_t color;
      switch(data->map.grid[mapX][mapY])
      {
        case '1':  color = 0xFF5555FF;  break; //red
        default: color = 0x00FF0000; break; //yellow
      }

      if (side == 1) {color = color * 0.8;}
	  verLine(data, x, 0, drawStart, 0xFFFF1F);
      verLine(data, x, drawStart, drawEnd, color);
	  verLine(data, x, drawEnd + 1, WIN_HEIGHT - 1, 0xFFFFFF);
    }
}

void	drawscreen(void *ptr){
	t_data	*data = (t_data *)ptr;
	
	cast_rays(data);
	//draw_minimap(data);
}

void	listenkeys(mlx_key_data_t keydata, void* ptr)
{
	t_data *data;

	data = (t_data *)ptr;

	(void)keydata;
	mlx_t* mlx = data->mlx_win;

	if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE) || mlx_is_key_down(mlx, MLX_KEY_Q))
		mlx_close_window(mlx);
	if (mlx_is_key_down(mlx, MLX_KEY_LEFT))
		data->player.x -= PL_SPEED;
	if (mlx_is_key_down(mlx, MLX_KEY_RIGHT))
		data->player.x += PL_SPEED;
	if (mlx_is_key_down(mlx, MLX_KEY_UP))
		data->player.y -= PL_SPEED;
	if (mlx_is_key_down(mlx, MLX_KEY_DOWN))
		data->player.y += PL_SPEED;
	return ;
}

int	load_mlx_data(t_data *data)
{
	data->mlx_win = mlx_init(WIN_WIDTH, WIN_HEIGHT, "cub3d", 0);
	data->mlx_img = mlx_new_image(data->mlx_win, WIN_WIDTH, WIN_HEIGHT);
	mlx_image_to_window(data->mlx_win, data->mlx_img, 0, 0);
	//mlx_loop_hook(data->mlx_win, drawscreen, data);
	//mlx_key_hook(data->mlx_win, listenkeys,(void *)data);
	mlx_loop_hook(data->mlx_win, ft_hook, data);
	return (0);
}