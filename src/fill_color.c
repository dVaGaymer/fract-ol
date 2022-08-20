/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_color.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alopez-g <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 23:31:36 by alopez-g          #+#    #+#             */
/*   Updated: 2022/08/20 23:56:39 by alopez-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include "fractol.h"
#include "shade.h"
#include "utils.h"
#include "ft_printf.h"
#include "libft.h"
#include <fcntl.h>

/*
 * 0 R
 * 1 G
 * 2 B
 * 3 A
 * */
int	pixel_buffer_put(t_img *img, int x, int y, int c)
{
	int	first_char;

	first_char = x * (img->bpp / 8) + y * img->line_size;
	*(img->buffer + first_char + 0) = (char)((c & 0x000000FF) >> 0);
	*(img->buffer + first_char + 1) = (char)((c & 0x0000FF00) >> 8);
	*(img->buffer + first_char + 2) = (char)((c & 0x00FF0000) >> 16);
	*(img->buffer + first_char + 3) = (char)((c & 0xFF000000) >> 24);
	return (0);
}

int	render_fractal(t_fract *f)
{
	int		col;
	int		x;
	int		y;
	int		y_limit;
	t_img	*img;

	img = f->img;
	y = -1;
	y_limit = img->img_h;
	if (f->menu_toggle)
		y_limit = f->menu->ypos;
	while (++y < y_limit)
	{
		x = -1;
		while (++x < img->img_w)
		{
			if (!(x % f->render_factor))
				col = shade(x, y, *f, 0);
			pixel_buffer_put(img, x, y, col);
		}
	}
	mlx_put_image_to_window(f->mlx->mlx, f->mlx->win, f->img->img, 0, 0);
	return (0);
}

int	draw_call(t_fract *f, int x, int y)
{
	int	cnt;
	int	side;

	side = 0.1 * f->menu->img_h;
	cnt = -1;
	while (++cnt < 5)
	{
		if (abs(x - f->menu->img_w / 2 + (cnt * side * 2) - (side * 4)) < side
				&& abs(y - f->menu->img_h / 2) < side)
			return (*(f->ran + f->ran_sel % 5)).ran[cnt];
	}
	return (0);
}

int	render_menu(t_fract *f)
{
	int		col;
	int		x;
	int		y;
	t_img	*menu;

	menu = f->menu;
	y = -1;
	while (++y < menu->img_h)
	{
		x = -1;
		while (++x < menu->img_w)
		{
			col = draw_call(f, x, y);
			pixel_buffer_put(menu, x, y, col);
		}
	}
	mlx_put_image_to_window(f->mlx->mlx, f->mlx->win, f->menu->img, f->menu->xpos, f->menu->ypos);
	//Color palette
	mlx_string_put(f->mlx->mlx, f->mlx->win, f->menu->img_w * 0.5 - 65, f->menu->ypos + f->menu->img_h * 0.25, 0x0000FFFF, "Color Palette:");
	//Re:
	mlx_string_put(f->mlx->mlx, f->mlx->win, f->menu->xpos + 10, f->menu->ypos + f->menu->img_h * 0.1, 0x00FFFFFF, "Re:");
	mlx_string_put(f->mlx->mlx, f->mlx->win, f->menu->xpos + 50, f->menu->ypos + f->menu->img_h * 0.1, 0x00FFFFFF, ft_ftoa(f->center.x, 9));
	//Im:
	mlx_string_put(f->mlx->mlx, f->mlx->win, f->menu->xpos + 10, f->menu->ypos + f->menu->img_h * 0.1 + 20, 0x00FFFFFF, "Im:");
	mlx_string_put(f->mlx->mlx, f->mlx->win, f->menu->xpos + 50, f->menu->ypos + f->menu->img_h * 0.1 + 20, 0x00FFFFFF, ft_ftoa(f->center.y, 9));
	//Zoom
	mlx_string_put(f->mlx->mlx, f->mlx->win, f->menu->xpos + 10, f->menu->ypos + f->menu->img_h * 0.1 + 40, 0x00FFFFFF, "Zoom:");
	mlx_string_put(f->mlx->mlx, f->mlx->win, f->menu->xpos + 80, f->menu->ypos + f->menu->img_h * 0.1 + 40, 0x00FFFFFF, ft_ultoa(f->zoom));
	//Iteration
	mlx_string_put(f->mlx->mlx, f->mlx->win, f->menu->xpos + 10, f->menu->ypos + f->menu->img_h * 0.1 + 60, 0x00FFFFFF, "Max Iter:");
	mlx_string_put(f->mlx->mlx, f->mlx->win, f->menu->xpos + 110, f->menu->ypos + f->menu->img_h * 0.1 + 60, 0x00FFFFFF, ft_ultoa(f->it));
	//Detail
	mlx_string_put(f->mlx->mlx, f->mlx->win, f->menu->xpos + 10, f->menu->ypos + f->menu->img_h * 0.1 + 80, 0x00FFFFFF, "Render Scale: 1/");
	mlx_string_put(f->mlx->mlx, f->mlx->win, f->menu->xpos + 170, f->menu->ypos + f->menu->img_h * 0.1 + 80, 0x00FFFFFF, ft_itoa(f->render_factor));
	//Type
	mlx_string_put(f->mlx->mlx, f->mlx->win, f->menu->img_w * 0.7,
			f->menu->ypos + f->menu->img_h * 0.1, 0x0000FFFF, "OP: ");
	if (f->type == MANDELBROT)
		mlx_string_put(f->mlx->mlx, f->mlx->win, f->menu->img_w * 0.7 + 40,
				f->menu->ypos + f->menu->img_h * 0.1, 0x00FFFFFF, "MANDELBROT");
	else if (f->type == JULIA)
	{
		mlx_string_put(f->mlx->mlx, f->mlx->win, f->menu->img_w * 0.7 + 40,
				f->menu->ypos + f->menu->img_h * 0.1, 0x00FFFFFF, "JULIA");
		//Re:
		mlx_string_put(f->mlx->mlx, f->mlx->win, f->menu->img_w * 0.7,
				f->menu->ypos + f->menu->img_h * 0.1 + 20, 0x00FFFFFF, "Re:");
		mlx_string_put(f->mlx->mlx, f->mlx->win, f->menu->img_w * 0.7 + 40,
				f->menu->ypos + f->menu->img_h * 0.1 + 20, 0x00FFFFFF, ft_ftoa(f->julia_init.x, 9));
		//Im1
		mlx_string_put(f->mlx->mlx, f->mlx->win, f->menu->img_w * 0.7,
				f->menu->ypos + f->menu->img_h * 0.1 + 40, 0x00FFFFFF, "Im:");
		mlx_string_put(f->mlx->mlx, f->mlx->win, f->menu->img_w * 0.7 + 40,
				f->menu->ypos + f->menu->img_h * 0.1 + 40, 0x00FFFFFF, ft_ftoa(f->julia_init.y, 9));
	}
	//Exp
	mlx_string_put(f->mlx->mlx, f->mlx->win, f->menu->img_w * 0.7,
			f->menu->ypos + f->menu->img_h * 0.1 + 60, 0x00FFFFFF, "Exp: ");
	mlx_string_put(f->mlx->mlx, f->mlx->win, f->menu->img_w * 0.7 + 45,
			f->menu->ypos + f->menu->img_h * 0.1 + 60, 0x00FFFFFF, ft_itoa(f->exp));
	//Func
	mlx_string_put(f->mlx->mlx, f->mlx->win, f->menu->img_w * 0.7,
			f->menu->ypos + f->menu->img_h * 0.1 + 80, 0x0000FFFF, "FUNCTION: ");
	if (f->func == znc)
		mlx_string_put(f->mlx->mlx, f->mlx->win, f->menu->img_w * 0.7,
				f->menu->ypos + f->menu->img_h * 0.1 + 100, 0x00FFFFFF, "z = z^exp + c");
	else if (f->func == ncorn)
	{
		mlx_string_put(f->mlx->mlx, f->mlx->win, f->menu->img_w * 0.7,
				f->menu->ypos + f->menu->img_h * 0.1 + 100, 0x00FFFFFF, "z.Im = -z.Im");
		mlx_string_put(f->mlx->mlx, f->mlx->win, f->menu->img_w * 0.7,
				f->menu->ypos + f->menu->img_h * 0.1 + 120, 0x00FFFFFF, "z = z^exp + c");
	}
	else if (f->func == bship)
		mlx_string_put(f->mlx->mlx, f->mlx->win, f->menu->img_w * 0.7,
				f->menu->ypos + f->menu->img_h * 0.1 + 100, 0x00FFFFFF, "z = abs(z)^exp + c");
	return (0);
}

int	render_export(t_fract *f)
{
	int		fd;
	int		col;
	int		x;
	int		y;
	int		completed;
	char	*scolor;
	t_img	*img;

	completed = 0;
	render_menu(f);
	fd = open("renders/new.ppm", O_CREAT | O_RDWR);
	img = f->img;
	y = -1;
	write(fd, "P3\n2048 1080\n255\n", 17);
	f->img->aspect = (double)f->render_w / (double)f->render_h;
	update_world(f);
	while (++y < Y2K)
	{
		x = -1;
		while (++x < X2K)
		{
			col = shade(x, y, *f, 1);
			scolor = ft_itoa((col & 0x00FF0000) >> 16);
			write(fd, scolor, ft_strlen(scolor));
			free(scolor);
			write(fd, " ", 1);
			scolor = ft_itoa((col & 0x0000FF00) >> 8);
			write(fd, scolor, ft_strlen(scolor));
			free(scolor);
			write(fd, " ", 1);
			scolor = ft_itoa((col & 0x000000FF));
			write(fd, scolor, ft_strlen(scolor));
			free(scolor);
			write(fd, "\n", 1);
		}
		mlx_string_put(f->mlx->mlx, f->mlx->win, 10, f->mlx->win_h - 40, 0x00000000, "NOW RENDERING");
		mlx_string_put(f->mlx->mlx, f->mlx->win, 10, f->mlx->win_h - 40, 0x00FFFFFF, "NOW RENDERING");
		mlx_string_put(f->mlx->mlx, f->mlx->win, f->mlx->win_w - 40, f->mlx->win_h - 40, 0x00000000, ft_itoa(completed));
		completed = y * 100 / Y2K;
		mlx_string_put(f->mlx->mlx, f->mlx->win, f->mlx->win_w - 40, f->mlx->win_h - 40, 0x0000FF00, ft_itoa(completed));
		mlx_string_put(f->mlx->mlx, f->mlx->win, (float)(completed/(float)100)
				* f->mlx->win_w, f->mlx->win_h - 20, 0x00FFFFFF, "-");
		mlx_do_sync(f->mlx->mlx);
	}
	f->img->aspect = (double)f->mlx->win_w / (double)f->mlx->win_h;
	update_world(f);
	close(fd);
	return (0);
}

int	on_loop(void **param)
{
	t_fract	*f;

	f = ((t_fract *)param);
	if (f->img)
		render_fractal(f);
	if (f->menu && f->menu_toggle)
		render_menu(f);
	return (0);
}
