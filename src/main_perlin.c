#include "../perlin_noise.h"

s8 parseInput(int argc, char **argv, u64 *seed, int *octaves, f32 *persistence, f32 *lacurarity, u8 *colorDisplay) {
	if (argc < 5 || argc > 6) {
		ft_printf_fd(1, "Usage: %s <seed> <octaves[1-8](int)> <persistence[1-2](float)> <lacunarity [1-3](float)> [ColorDisplay(u8 basic set to false, otherwise to enable)]\n", argv[0]);
		return (1);
	}
	*seed = array_to_uint32(argv[1]);
	*octaves = ft_atoi(argv[2]);
	*persistence = ft_atof(argv[3]);
	*lacurarity = ft_atof(argv[4]);
	if (*octaves < 1 || *octaves > 8) {
		ft_printf_fd(1, "octaves must be between 1 and 8\n");
		return (1);
	} else if (*persistence < 0.3 || *persistence > 2.0) {
		ft_printf_fd(1, "persistence must be between 1.0 and 2.0\n");
		return (1);
	} else if (*lacurarity < 1.0 || *lacurarity > 2.5) {
		ft_printf_fd(1, "lacurarity must be between 1.0 and 2.5\n");
		return (1);
	}
	if (argc == 6) {
		*colorDisplay = 1U;
	}
    ft_printf_fd(1, ORANGE"seed: %u\n"RESET, *seed);
	return (0);
}

int main(int argc, char **argv) {
	u64 seed;
	int octaves;
	f32 persistence, lacurarity;
	f32 **perlinNoise = NULL;
	u8 colorDisplay = 0;

	if (parseInput(argc, argv, &seed, &octaves, &persistence, &lacurarity, &colorDisplay)) {
		return (1);
	}
    // randomGenerationInit(seed);
    u8 *image = perlinImageGet(seed, PERLIN_NOISE_WIDTH, PERLIN_NOISE_HEIGHT, octaves, persistence, lacurarity);
    if (!image) {
		ft_printf_fd(1, "Error: perlinImageGet return NULL\n");
		return (1);
	}
	if (colorDisplay) {
		perlinNoise = perlin2DFloatGet(image);
		init_mlx(PERLIN_NOISE_WIDTH, PERLIN_NOISE_HEIGHT, (void *)&perlinNoise, colorDisplay);
	} else {
		init_mlx(PERLIN_NOISE_WIDTH, PERLIN_NOISE_HEIGHT, (void *)&image, colorDisplay);
	}

    return (0);
}