/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alopez-g <alopez-g@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/28 04:12:48 by alopez-g          #+#    #+#             */
/*   Updated: 2022/08/04 14:55:43 by alopez-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include "ft_printf.h"
#include "fractol.h"
#include "utils.h"
#include "hooks.h"
#define FT_WIDTH 720
#define FT_HEIGHT 360

int	main(void)
{
	t_mlx	mlx;
	t_img	main_buffer;
	t_fract	fract;

	fract.mlx = &mlx;
	fract.img = &main_buffer;
	if (setup(&fract, FT_WIDTH, FT_HEIGHT))
		return (1);
	print_info(mlx, main_buffer);
	mlx_key_hook(mlx.win, key_pressed, &fract);
	mlx_mouse_hook(mlx.win, mouse_pressed, &fract);
	mlx_loop_hook(mlx.mlx, update_frame, &fract);
	mlx_loop(mlx.mlx);
	return (1);
}
