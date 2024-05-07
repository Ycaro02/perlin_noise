#include "perlin_noise.h"

# include <math.h>
# include <fcntl.h>
# include "minilibx-linux/mlx.h"
# include "minilibx-linux/mlx_int.h"


#define WIDTH 800
#define HEIGHT 600

u8 *perlinImageGet(int width, int height) {
    // Generate Perlin noise
    float **noise = noiseSample2D(width, height);
    // Convert Perlin noise to image
    u8 *image = malloc(width * height * sizeof(u8));
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // Map Perlin noise from [-1, 1] to [0, 255] for grayscale
            u8 color = (u8)((noise[i][j] + 1.0f) * 0.5f * 255);
            image[i * width + j] = color; // Gray
        }
    }

	for (int i = 0; i < height; i++) {
		ft_printf_fd(1, "Color: |%d| |%d| |%d|\n", image[i], image[i + 1], image[i + 2]);
	}
    return (image);

}

int main() {
    // Initialize GLFW
    ft_printf_fd(1, "Go\n");
    init_mlx(WIDTH, HEIGHT);
    return 0;
}
