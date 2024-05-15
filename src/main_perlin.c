#include "../perlin_noise.h"

s8 parseInput(int argc, char **argv, u64 *seed, int *octaves, f32 *persistence, f32 *lacurarity) {
	if (argc != 5) {
		ft_printf_fd(1, "Usage: %s <seed> <octaves[1-8](int)> <persistence[1-2](float)> <lacunarity [1-3](float)>\n", argv[0]);
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
	} else if (*lacurarity < 1.0 || *lacurarity > 3.0) {
		ft_printf_fd(1, "lacurarity must be between 1.0 and 3.0\n");
		return (1);
	}
    ft_printf_fd(1, ORANGE"seed: %u\n"RESET, *seed);
	return (0);
}

int main(int argc, char **argv) {
	u64 seed;
	int octaves;
	f32 persistence, lacurarity;

	if (parseInput(argc, argv, &seed, &octaves, &persistence, &lacurarity)) {
		return (1);
	}
    randomGenerationInit(seed);
    u8 *image = perlinImageGet(PERLIN_NOISE_WIDTH, PERLIN_NOISE_HEIGHT, octaves, persistence, lacurarity);
    if (!image) {
		ft_printf_fd(1, "Error: perlinImageGet return NULL\n");
		return (1);
	}
	init_mlx(PERLIN_NOISE_WIDTH, PERLIN_NOISE_HEIGHT, &image);
    return (0);
}