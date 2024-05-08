#include "perlin_noise.h"
#include <math.h>
#include <fcntl.h>
#include "minilibx-linux/mlx.h"
#include "minilibx-linux/mlx_int.h"

void minMaxNoiseGet(f32 **noise, f32 *min, f32 *max, int h, int w) {
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            *min = fmin(*min, noise[i][j]);
            *max = fmax(*max, noise[i][j]);
        }
    }
}

int normalize(f32 value, f32 start1, f32 stop1, int start2, int stop2) {
    return start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
}


u8 *perlinImageGet(int width, int height) {
    vec2_f32 **gradient = gradientNoiseGeneration(width, height);
    u8 *image = ft_calloc(width * height, sizeof(u8));
    f32 min = 1.0f, max = -1.0f;
    float **noise = noiseSample2D(gradient, width, height);
    minMaxNoiseGet(noise, &min, &max, height, width);
    f32 scale = 1.0 / (max - min);

	f32 noiseMIN = 1.0f, noiseMAX = -1.0f;
	u8 colorMin = 255, colorMax = 0;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            float total = noise[i][j];
            total = (total - min) * scale; // Extend the range of Perlin noise values
			if (total < noiseMIN) { noiseMIN = total;}
			if (total > noiseMAX) { noiseMAX = total;}
            image[i * width + j] = normalize(total, 0.0, 1.0, 0, 255); // Normalize to the range of colors
			if (image[i * width + j] < colorMin) { colorMin = image[i * width + j];}
			if (image[i * width + j] > colorMax) { colorMax = image[i * width + j];}
        }
    }
	ft_printf_fd(1, RED"min: %f, max: %f\n"RESET, noiseMIN, noiseMAX);
	ft_printf_fd(1, GREEN"colorMin: %d, colorMax: %d\n"RESET, colorMin, colorMax);
    free_incomplete_array((void **)noise, height);
    return image;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        ft_printf_fd(1, "Usage: %s <seed>\n", argv[0]);
        return 1;
    }
    u64 seed = array_to_uint32(argv[1]);
	ft_printf_fd(1, ORANGE"seed: %u\n"RESET, seed);
    randomGenerationInit(seed);
    u8 *image = perlinImageGet(PERLIN_NOISE_WIDTH, PERLIN_NOISE_HEIGHT);
    init_mlx(PERLIN_NOISE_WIDTH, PERLIN_NOISE_HEIGHT, image);
    return 0;
}