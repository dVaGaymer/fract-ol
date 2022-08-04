/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shade.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alopez-g <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 23:35:56 by alopez-g          #+#    #+#             */
/*   Updated: 2022/08/04 23:33:17 by alopez-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "fractol.h"
#include "ft_printf.h"
#include "ft_math.h"

/*
 * Little endian:
 * 0XAARRGGBB
 * color |= ((0x000000FF & a) << 24);
 * */
int	color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	int	color;

	(void)a;
	color = 0;
	color |= ((0x000000FF & r) << 16);
	color |= ((0x000000FF & g) << 8);
	color |= (0x000000FF & b);
	return (color);
}

int	diverges(t_vec2 (*func)(), int type, int it, void *init_z, void *init_c)
{
	t_vec2			z;
	t_vec2			c;
	int				i_cnt;

	i_cnt = 0;
	if (type == MANDELBROT)
	{
		z.r = 0;
		z.i = 0;
		c = *((t_vec2 *)(init_c));
	}
	else if (type == JULIA)
	{	
		z = *((t_vec2 *)(init_c));
		c = *((t_vec2 *)(init_z));
	}
	while ((i_cnt++ < it) && z.r < 2.0 && z.i < 2.0)
		z = func(z, c, 0);
	if (z.r > 2.0 || z.i > 2.0)
		return (i_cnt);
	return (0);
}

/*
 * zoom:
 * map [0, 0, w, h] -> [x0, y0, x1, y1]
 * output = output_start + ((output_end - output_start) / (input_end - input_start)) * (input - input_start)
 * xn = x0 + ((x1 - x0) / (w - 0)) * (x - 0)
 * yn = y0 + ((y1 - y0) / (h - 0)) * (y - 0)
 *
 * world[0] = f.center[0] - ((((IY1 - IY0) * aspect) / 2) / f.zoom);
 * world[1] = f.center[1] + (IY0 / f.zoom);
 * world[2] = f.center[0] + ((((IY1 - IY0) * aspect) / 2) / f.zoom);
 * world[3] = f.center[1] + (IY1 / f.zoom)};
 * */
int	shade(int x, int y, t_fract f)
{
	int		col;
	int		d;	
	t_vec2	z;
	t_vec2	c;
	
	c.r = ((double)x / (double)f.img->img_w * (f.world[2] - f.world[0])) + f.world[0];
	c.i = ((double)y / (double)f.img->img_h * (f.world[3] - f.world[1])) + f.world[1];
	z.r = 0.5;
	z.i = 0.4;
	d = diverges(f.func, f.type, f.it, &z, &c);
	if(d)
		col = color(d + 150, 0, d + 150, 0);
	else
		col = 0x00000000;
	return (col);
}
