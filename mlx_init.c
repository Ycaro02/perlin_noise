#include "perlin_noise.h"
#include "minilibx-linux/mlx.h"
#include "minilibx-linux/mlx_int.h"

typedef struct s_mlxData {
	void *ptr;
	void *win;
	void *img;
	char *dataAdrr;
} mlxContext;

/* @brief Destroy windows/display */
int destroy_windows(mlxContext *mlx)
{
	mlx_destroy_image(mlx->ptr, mlx->img);
	mlx_destroy_window(mlx->ptr, mlx->win);
	mlx_destroy_display(mlx->ptr);
	ft_printf_fd(1, "Mlx exit\n");	
	free(mlx->ptr);
	// free(mlx);
	exit(0);
}

#define ESC 65307

/* @brief key press handler */
int	key_hooks_press(int keycode, mlxContext *game)
{
	if (keycode == ESC)
		destroy_windows(game); /* maybe need to check sem value and lock it to detash mem */
	return (0);
}

/* @brief Draw board */
// static void draw_board(mlxContext *game)
// {
// 	int color = 0;

// 	for (uint32_t y = 1; y < SCREEN_HEIGHT; ++y) {
// 		for (uint32_t x = 1; x < (SCREEN_WIDTH - RIGHTBAND_WIDTH); ++x) {
// 			uint32_t x_compute =  ((x / TILE_SIZE) % BOARD_W);
// 			uint32_t y_compute = ((y / TILE_SIZE) * BOARD_W);   
// 			uint32_t idx = x_compute + y_compute;
// 			uint32_t tile_state = game->ipc->ptr[idx];
// 			color = tile_state == TILE_EMPTY ? 0xFFFFFF : get_new_color(tile_state).color;
// 			if (x % TILE_SIZE != 0 && y % TILE_SIZE != 0) {
// 				((uint32_t *)game->img.data)[x + (y * (SCREEN_WIDTH - RIGHTBAND_WIDTH))] = color;
// 			}
// 		}
// 	}
// }


	
	/* Display image (flush) */
	// mlx_put_image_to_window(game->mlx, game->win, game->img.image, 0, 0);

/* @brief Init display */
int8_t init_mlx(int width, int height) 
{
	int8_t	packet_extract = 0; 
	int		endian = 0;
	mlxContext mlx = {};

	mlx.ptr = mlx_init();
	if (!mlx.ptr) {
		ft_printf_fd(2, "mlx_init failed\n");
		return (1);
	}
	mlx.win = mlx_new_window(mlx.ptr, width, height, "PerlinNoise");
	mlx.img = mlx_new_image(mlx.ptr, width, height);
	// ft_printf_fd(1, CYAN"SCREEN WIDTH %u SCREEN HEIGHT %u\n"RESET, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!mlx.win || !mlx.img) {
		ft_printf_fd(2, "mlx_new_window or img failed\n");
		return (1);
	}
	int bitPerPixel = 0;
	int imgWidth = 0;

	mlx.dataAdrr = mlx_get_data_addr(mlx.img, &bitPerPixel, &imgWidth, &endian);
	if (!mlx.dataAdrr) {
		ft_printf_fd(2, "mlx_get_data_addr failed\n");
		return (1);
	}

	mlx_hook(mlx.win, 2, 1L, key_hooks_press, &mlx);
	mlx_hook(mlx.win, DestroyNotify, StructureNotifyMask, destroy_windows, &mlx);
	// mlx_loop_hook(game->mlx, display_board_stdout, game);
	// mlx_loop_hook(game->mlx, main_display, game);
	mlx_loop(mlx.ptr);
	return (0);

}
