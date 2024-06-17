#include "../../include/perlin_noise.h"

u8 *perlinImageInit(int width, int height, int octaves, f32 persistence, f32 lacurarity) {
    vec2_f32 **gradient = gradientNoiseGeneration(width, height);
    u8 *image = ft_calloc(width * height, sizeof(u8));
    f32 min = 1.0f, max = -1.0f;

    // Generate Perlin noise with octaves
    f32 **noise = perlinNoiseOctaveSample2D(gradient, width, height, octaves, persistence, lacurarity);

    // Find minimum and maximum values in the noise
    minMaxNoiseGet(noise, &min, &max, height, width);
    f32 scale = 1.0 / (max - min);
	#ifdef PERLIN_NOISE_DEBUG
	u8 colorMax = 0, colorMin = 255;
	f32 totalMax = -1.0f, totalMin = 1.0f;
	ft_printf_fd(1, RED"Perlin Noise Debug Mode Enable\n"RESET""ORANGE"Perlin Val: min: %f, max: %f\n"RESET, min, max);
	#endif

    // Normalize noise values and convert to color values
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            float total = noise[i][j];
            total = (total - min) * scale; // Extend the range of Perlin noise values
            image[i * width + j] = normalize(total, 0.0, 1.0, 0, 255); // Normalize to the range of colors
			#ifdef PERLIN_NOISE_DEBUG
			debugColorUpdate(image[i * width + j], &colorMin, &colorMax);
			debugTotalUpdate(total, &totalMin, &totalMax);
			#endif
        }
    }

	#ifdef PERLIN_NOISE_DEBUG
	ft_printf_fd(1, PINK"Color Val: min: %d, max: %d\n"RESET, colorMin, colorMax);
	ft_printf_fd(1, GREEN"Total Val: min: %f, max: %f\n"RESET, totalMin, totalMax);
	#endif

    // Free allocated memory for noise
    free_incomplete_array((void **)noise, height);
	free_incomplete_array((void **)gradient, height);
    return (image);
}

u8 *perlinImageGet(unsigned int seed, int width, int height, int octaves, f32 persistence, f32 lacurarity) {
    u8 *perlinMap = NULL;

    randomGenerationInit(seed);
    perlinMap = perlinImageInit(width, height, octaves, persistence, lacurarity);
    return (perlinMap);
}

u8 *perlinImageGetWithoutSeed(int width, int height, int octaves, f32 persistence, f32 lacurarity) {
    u8 *perlinMap = NULL;

    perlinMap = perlinImageInit(width, height, octaves, persistence, lacurarity);
    return (perlinMap);
}