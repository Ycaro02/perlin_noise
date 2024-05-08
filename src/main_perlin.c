#include "../perlin_noise.h"

int main(int argc, char **argv) {
    if (argc != 5) {
        ft_printf_fd(1, "Usage: %s <seed> <octaves[4-8](int)> <persistence[1-2](float)> <lacunarity [1-3](float)>\n", argv[0]);
        return (1);
    }
    u64 seed = array_to_uint32(argv[1]);
    int octaves = ft_atoi(argv[2]);
    f32 persistence = ft_atof(argv[3]);
	f32 lacurarity = ft_atof(argv[4]);

    ft_printf_fd(1, ORANGE"seed: %u\n"RESET, seed);
    randomGenerationInit(seed);
    u8 *image = perlinImageGet(PERLIN_NOISE_WIDTH, PERLIN_NOISE_HEIGHT, octaves, persistence, lacurarity);
    init_mlx(PERLIN_NOISE_WIDTH, PERLIN_NOISE_HEIGHT, &image);
	
    return (0);
}