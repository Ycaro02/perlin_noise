#include "../perlin_noise.h"
#include "../minilibx-linux/mlx.h"
#include "../minilibx-linux/mlx_int.h"

#define ESC_KEY 65307

typedef struct s_mlxData {
	void 	*ptr;
	void 	*win;
	void 	*img;
	char 	*dataAdrr;
	int	 	w, h;
	u8	 	*perlinData;
	u8	 	**perlinPtr;
	u8		colorDisplay;
} mlxContext;

/* @brief Destroy windows/display */
int destroy_windows(mlxContext *mlx)
{
	mlx_destroy_image(mlx->ptr, mlx->img);
	mlx_destroy_window(mlx->ptr, mlx->win);
	mlx_destroy_display(mlx->ptr);
	free(*mlx->perlinPtr);
	free(mlx->ptr);
	ft_printf_fd(1, "Mlx exit\n");	
	exit(0);
}



/* @brief key press handler */
int	key_hooks_press(int keycode, mlxContext *game)
{
	if (keycode == ESC_KEY)
		destroy_windows(game); /* maybe need to check sem value and lock it to detash mem */
	return (0);
}

/* @brief Draw board */
static int perlinNoiseDraw(void *data)
{
	mlxContext *mlx = data;

	int i = 0;
	for (int y = 0; y < mlx->h; ++y) {
		for (int x = 0; x < mlx->w; ++x) {
			if (!mlx->colorDisplay) { /* black and white display */
				int color = mlx->perlinData[i] << 16 | mlx->perlinData[i] << 8 | mlx->perlinData[i];
				((int *)mlx->dataAdrr)[y * mlx->w + x] = color;
			} else { /* color display RGB */
				if (mlx->perlinData[i] <= 255 && mlx->perlinData[i] >= 150) {
					((int *)mlx->dataAdrr)[y * mlx->w + x] = 0xff0000;
				} else if (mlx->perlinData[i] >= 100 && mlx->perlinData[i] <= 150) {
					((int *)mlx->dataAdrr)[y * mlx->w + x] = 0x0000ff;
				} else {
					((int *)mlx->dataAdrr)[y * mlx->w + x] = 0x00ff00;
				}
			}
			i++;
		}
	}
	mlx_put_image_to_window(mlx->ptr, mlx->win, mlx->img, 0, 0);
}


	
	/* Display image (flush) */
	// mlx_put_image_to_window(game->mlx, game->win, game->img.image, 0, 0);

/* @brief Init display */
int8_t init_mlx(int width, int height, u8 **perlinData, u8 colorDisplay) 
{
	int8_t	packet_extract = 0; 
	int		endian = 0;
	mlxContext mlx;


	ft_bzero(&mlx, sizeof(mlx));
	mlx.perlinPtr = perlinData;
	mlx.perlinData = *perlinData;
	mlx.w = width;
	mlx.h = height;
	
	mlx.colorDisplay = colorDisplay;

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
	mlx_loop_hook(mlx.ptr, perlinNoiseDraw, &mlx);
	mlx_loop(mlx.ptr);
	return (0);

}
