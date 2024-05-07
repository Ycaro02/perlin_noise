#ifndef HEADER_PERLIN_NOISE_H
#define HEADER_PERLIN_NOISE_H

#define PERLIN_NOISE_HEIGHT 20
#define PERLIN_NOISE_WIDTH 20

#include "libft/libft.h"

/**
 * @brief Get a random number between -max/2 and PERLIN_NOISE_HEIGHT
 * @param number The number of random number to generate
 * @param max The maximum value of the random number
 * @return int* The array of random number
*/
FT_INLINE int *randomMultipleGenerationGet(int number, int max) {
	int *array = malloc(sizeof(int) * number);

	for (int i = 0; i < number; i++) {
		array[i] = ((rand() - (max / 2)) % PERLIN_NOISE_HEIGHT);
	}
	return (array);
}

/**
 * @brief Initialize the random generation with a seed
 * @param seed The seed to initialize the random generation
*/
FT_INLINE void randomGenerationInit(unsigned int seed) {
	srand(seed);
}

/**
 * @brief Get a random number between -max/2 and PERLIN_NOISE_HEIGHT
*/
FT_INLINE int randomGenerationGet(int max) {
	return ((rand() - (max / 2)) % PERLIN_NOISE_HEIGHT);
}

FT_INLINE void noiseGradienCompute(int x, int y, vec2_f32 dest) {
	f64 powX = x * x;
	f64 powY = y * y;

	f64 distance = sqrt(powX + powY);

	dest[0] = x / distance;
	dest[1] = y / distance;
}

/**
 * @brief Generate a 2D array of gradient noise
 * @param width The width of the array
 * @param height The height of the array
 * @return vec2_f32** The 2D array of gradient noise
*/
vec2_f32 **gradientNoiseGeneration(int width, int height);


f32 perlinNoise(vec2_f32 **gradient, f32 x, f32 y);

f32 **noiseSample2D(int width, int height);


int8_t init_mlx(int width, int height);

#endif /* HEADER_PERLIN_NOISE_H */
