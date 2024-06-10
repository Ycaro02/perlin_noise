#include "../perlin_noise.h"
#include "../minilibx-linux/mlx.h"
#include "../minilibx-linux/mlx_int.h"

#define ESC_KEY 65307

#define RGB_RED 0xff0000
#define RGB_GREEN 0x00ff00
#define RGB_BLUE 0x0000ff

#define BLACK 0x000000
#define VERRY_DARK_GRAY 0x404040
#define DARK_GRAY 0x808080
#define GRAY 0xA0A0A0
#define LIGHT_GRAY 0xC0C0C0
#define VERRY_LIGHT_GRAY 0xE0E0E0

#define BASIC_DISPLAY	0U
#define FLOAT_DISPLAY	1U
#define CAVE_DISPLAY	2U


typedef struct s_mlxData {
	void 	*ptr;
	void 	*win;
	void 	*img;
	char 	*dataAdrr;
	int	 	w, h;
	f32	 	**perlinFloatData;
	f32	 	***perlinFloatPtr;
	u8		*perlinU8Data;
	u8		**perlinU8Ptr;
	u8		colorDisplay;
} mlxContext;

/* @brief Destroy windows/display */
int destroy_windows(mlxContext *mlx)
{
	mlx_destroy_image(mlx->ptr, mlx->img);
	mlx_destroy_window(mlx->ptr, mlx->win);
	mlx_destroy_display(mlx->ptr);

	if (mlx->colorDisplay) {
		for (int i = 0; i < mlx->h; ++i) {
			free((mlx->perlinFloatData)[i]);
		}
		free(*mlx->perlinFloatPtr);
	} else {
		free(*mlx->perlinU8Ptr);
	}
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


/* @brief Draw black and white board */
static int perlinNoiseDraw(void *data) {
	mlxContext *mlx = data;

	for (int y = 0; y < mlx->h; ++y) {
		for (int x = 0; x < mlx->w; ++x) {
			int color = (int)mlx->perlinU8Data[y * mlx->w + x] << 16 | (int)mlx->perlinU8Data[y * mlx->w + x] << 8 | (int)mlx->perlinU8Data[y * mlx->w + x];
			((int *)mlx->dataAdrr)[y * mlx->w + x] = color;
		}
	}
	mlx_put_image_to_window(mlx->ptr, mlx->win, mlx->img, 0, 0);
}

#define SNAKE_VAL_WALL 0U
#define SNAKE_VAL_CAVE 1U
#define SNAKE_VAL_EXIT 2U

int perlinNoiseSnakeDraw(mlxContext *mlx) {

	static u8 **snakeData = NULL;
	
	if (!snakeData) {
		snakeData = perlinToSnakeData(mlx->perlinFloatData, mlx->w, mlx->h);
	}

	// Draw the snake data
	for (int y = 0; y < mlx->h; ++y) {
		for (int x = 0; x < mlx->w; ++x) {
			if (snakeData[y][x] == SNAKE_VAL_CAVE) {
				((int *)mlx->dataAdrr)[y * mlx->w + x] = BLACK;
			} else if (snakeData[y][x] == SNAKE_VAL_EXIT) {
				// ft_printf_fd(1, "EXIT on %d %d\n\n", y, x);
				((int *)mlx->dataAdrr)[y * mlx->w + x] = RGB_RED;
			} else {
				((int *)mlx->dataAdrr)[y * mlx->w + x] = GRAY;
			}
		}
	}
	// Free the snake data
	// for (int i = 0; i < mlx->h; ++i) {
	// 	free(snakeData[i]);
	// }
	// free(snakeData);

    mlx_put_image_to_window(mlx->ptr, mlx->win, mlx->img, 0, 0);
}

/* @brief Draw Color board */
static int perlinNoiseColorDraw(void *data)
{
	mlxContext *mlx = data;

	for (int y = 0; y < mlx->h; ++y) {
		for (int x = 0; x < mlx->w; ++x) {
			f32 val = mlx->perlinFloatData[y][x];
			if (val >= 0.5f) {
				((int *)mlx->dataAdrr)[y * mlx->w + x] = BLACK;
			} else if (val >= 0.2 && val <= 0.5f) {
				((int *)mlx->dataAdrr)[y * mlx->w + x] = VERRY_DARK_GRAY;
			} else if (val >= 0.0 && val <= 0.2f) {
				((int *)mlx->dataAdrr)[y * mlx->w + x] = DARK_GRAY;
			} else if (val >= -0.2 && val <= 0.0f) {
				((int *)mlx->dataAdrr)[y * mlx->w + x] = GRAY;
			} else if (val >= -0.5 && val <= 0.2f) {
				((int *)mlx->dataAdrr)[y * mlx->w + x] = LIGHT_GRAY;
			} else {
				((int *)mlx->dataAdrr)[y * mlx->w + x] = VERRY_LIGHT_GRAY;
			}
		}
	}
	mlx_put_image_to_window(mlx->ptr, mlx->win, mlx->img, 0, 0);
}


/* @brief Init display */
int8_t init_mlx(int width, int height, void *perlinData, u8 colorDisplay) 
{
	mlxContext mlx;
	int endian = 0;

	ft_bzero(&mlx, sizeof(mlx));
	if (colorDisplay) {
		ft_printf_fd(1, "Float display enable : float data\n");
		mlx.perlinFloatPtr = (f32 ***)perlinData;
		mlx.perlinFloatData = *(f32 ***)perlinData;
	} else {
		ft_printf_fd(1, "Unsigned int display enable : u8 data\n");
		mlx.perlinU8Ptr = (u8 **)perlinData;
		mlx.perlinU8Data = *(u8 **)perlinData;
	}
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
	if (colorDisplay == FLOAT_DISPLAY) {
		mlx_loop_hook(mlx.ptr, perlinNoiseColorDraw, &mlx);
	} else if (colorDisplay == CAVE_DISPLAY) {
		mlx_loop_hook(mlx.ptr, perlinNoiseSnakeDraw, &mlx);
	} else {
		mlx_loop_hook(mlx.ptr, perlinNoiseDraw, &mlx);
	}
	// mlx_loop_hook(mlx.ptr, perlinNoiseColorDraw, &mlx);
	mlx_loop(mlx.ptr);
	return (0);

}
