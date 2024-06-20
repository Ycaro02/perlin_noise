#ifndef HEADER_PERLIN_NOISE_H
#define HEADER_PERLIN_NOISE_H

/* Height and width used for perlin noise generation */

//#define PERLIN_NOISE_HEIGHT 2048
//#define PERLIN_NOISE_WIDTH 2048

#define PERLIN_NOISE_HEIGHT 1024
#define PERLIN_NOISE_WIDTH 1024

#define PERLIN_ARRAY_SIZE (PERLIN_NOISE_HEIGHT * PERLIN_NOISE_WIDTH)

#include "../libft/libft.h"

typedef struct s_perlin_data {
	s32 z0, x0, z1, x1;
	f32 val;
	f32 add;
	s32 normalise;
	s32 givenX, givenZ;
} PerlinData;

int8_t init_mlx(int width, int height, void *perlinData, u8 colorDisplay);

/**
 * @brief Initialize the random generation with a seed
 * must be called one time before any random generation
 * @param seed The seed to initialize the random generation
*/
FT_INLINE void randomGenerationInit(unsigned int seed) {
	srand(seed);
}

/**
 * @brief Get a random number between -max/2 and PERLIN_NOISE_HEIGHT
*/
FT_INLINE int randomGenerationGet() {
    f32 range = 1.0;
    f32 res = (f32)rand() / (f32)RAND_MAX * 2.0 * range - range;
	res *= 10000;
    return (res);
}

/**
 * noise_image.c *
*/
u8 *perlinImageGet(unsigned int seed, int width, int height, int octaves, f32 persistence, f32 lacurarity);

/* To call after seed generated */
u8 *perlinImageGetWithoutSeed(int width, int height, int octaves, f32 persistence, f32 lacurarity);

/**
 * perlin_noise.c *
*/

/**
 * @brief Generate a 2D array of gradient noise
 * @param width The width of the array
 * @param height The height of the array
 * @return vec2_f32** The 2D array of gradient noise
*/
vec2_f32 **gradientNoiseGeneration(int width, int height);


/**
 * @brief Compute the perlin noise value for x, y coordinates
 * @param gradient: Gradient vector
 * @param x, y: float coordinate
 * @return Perlin noise value
*/
f32 perlinNoise(vec2_f32 **gradient, f32 x, f32 y);

/**
 * @brief Generate a 2D sample of Perlin noise (Used in OctaveSample version)
 * @param width: Width of the sample
 * @param height: Height of the sample
 * @return 2D sample of Perlin noise
*/
f32 **noiseSample2D(vec2_f32 **gradient, int width, int height, f32 frequency);

/**
 * @brief Sample noise at a given octaves and frequency
 * @param gradient The 2D array of gradient noise
 * @param width The width of the array
 * @param height The height of the array
 * @param octaves The number of octaves to sample
 * @param frequency The frequency of the noise
 * @return f32** The 2D array of noise
 * @note The noise is sampled at the given frequency by scaling the coordinates
*/
f32 **perlinNoiseOctaveSample2D(vec2_f32 **gradient, int width, int height, int octaves, f32 persistence, f32 lacunarity);


/**
 * noise_utils.c *
*/

/**
 * @brief Get the minimum and maximum values in a 2D array
 * @param noise The 2D array of noise
 * @param min,max The minimum and maximum values in the array (input/output)
 * @param h,w The height and width of the array
*/
void minMaxNoiseGet(f32 **noise, f32 *min, f32 *max, int h, int w);

/**
 * @brief Normalize a value from one range to another
 * @param value The value to normalize
 * @param start1,stop1 The start and stop of the input range
 * @param start2,stop2 The start and stop of the output range
 * @return int The normalized value
*/
int normalize(f32 value, f32 start1, f32 stop1, int start2, int stop2);

/**
 * @brief Update the minimum and maximum color values
 * @param color The color to update
 * @param colorMin,colorMax The minimum and maximum color values (input/output)
*/
void debugColorUpdate(u8 color, u8 *colorMin, u8 *colorMax);

/**
 * @brief Update the minimum and maximum total values
 * @param total The total to update
 * @param totalMin,totalMax The minimum and maximum total values (input/output)
*/
void debugTotalUpdate(f32 total, f32 *totalMin, f32 *totalMax);

/**
 * @brief Allocate a 2D array of floats
 * @param rows,cols The number of rows and columns in the array
 * @return f32** The allocated 2D array
*/
f32 **floatDoubleArrayAlloc(int rows, int cols);


/* SNAKE define */
#define PERLIN_WHITE_VAL	0U
#define PERLIN_BLACK_VAL	1U
#define ENTRY_EXIT_VAL		2U

#define PATH_VAL			PERLIN_BLACK_VAL

typedef struct {
    int x, y;
} PathPoint;

typedef struct {
    PathPoint 	*points;
	PathPoint 	entry;
    int 		length;
} Path;

/**
 * @brief Perlin data to snake cave data
 * @param perlinData The perlin data to convert
 * @param w,h The width and height of the perlin data
 * @return u8** The snake cave data
*/
u8 **perlinToSnakeData(f32 **perlinData, int w, int h);

f32 normalisef32Tof32(f32 value, f32 start1, f32 stop1, f32 start2, f32 stop2);
f32 perlinInterpolate(f32 a, f32 b, f32 t);
f32 bilinearInterpolation(f32 q11, f32 q12, f32 q21, f32 q22, f32 x, f32 z);
f32 getInterpolatedPerlinNoise(f32 **perlinNoise, f32 x, f32 z, f32 scale, s32 width, s32 height, PerlinData *perlinVal);
f32 interpolateNoiseGet(f32 **perlinNoise, s32 x, s32 z, PerlinData *perlinVal);

f32 **array1DTo2D(u8 *array, u32 height, u32 width);
f32 normalizeU8Tof32(u8 value, u8 start1, u8 stop1, f32 start2, f32 stop2);
f32 **perlin2DFloatGet(u8 *perlin1D);
/* mlx init for display */
//int8_t init_mlx(int width, int height, u8 **perlinData);

#endif /* HEADER_PERLIN_NOISE_H */
