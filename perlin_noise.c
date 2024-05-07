#include "cglm/include/cglm/cglm.h"
#include "perlin_noise.h"

/**
 * @brief Compute the gradient vector, SEED must be initialized before
 * by calling randomGenerationInit()
 * @param width: Width of the gradient vector
 * @param height: Height of the gradient vector
 * @return Gradient vector
*/
vec2_f32 **gradientNoiseGeneration(int width, int height) {
	vec2_f32 **noise = malloc(sizeof(vec2_f32 *) * height);

	if (!noise) {
		return (NULL);
	}

	for (int i = 0; i < height; i++) {
		noise[i] = malloc(sizeof(vec2_f32 *) * width);
		if (!noise[i]) {
			free_incomplete_array((void **)noise, i);
			return (NULL);
		}
		for (int j = 0; j < width; j++) {
			noiseGradienCompute(randomGenerationGet(INT_MAX), randomGenerationGet(INT_MAX), noise[i][j]);
		}
	}
	return (noise);
}


/** 
 * @brief Smooth transition between 0 and 1
 * @param w: Value to smooth  
 * @return Smoothed value
*/
f32 smoothStep(f32 w) {
     if (w <= 0.0) {
		// ft_printf_fd(1, "NEG w: %f\n", w);
		return (0.0);
	 } else if (w >= 1.0) {
		return (1.0);
	 }
     return (w * w * (3.0 - 2.0 * w));
}

/**
 * @brief Interpolate between two values
 * @param a0: First value
 * @param a1: Second value
 * @param w: Weight must be between 0 and 1
*/
f32 interpolateValues(f32 a0, f32 a1, f32 w) {
	// ft_printf_fd(1, "a0: %f, a1: %f, w: %f smoothW: %f\n", a0, a1, w, smoothStep(w));
	// ft_printf_fd(1, "smoothStep(w): %f\n", smoothStep(w));
	// ft_printf_fd(1, "a0 + (a1 - a0) * smoothStep(w): %f\n", a0 + (a1 - a0) * smoothStep(w));
	return (a0 + (a1 - a0) * smoothStep(w));
}

/**
 * @brief Compute the dot product between the gradient and the distance vector
 * @param gradient: Gradient vector
 * @param ix, iy: index
 * @param x, y: float coordinate
*/
f32 dotGridGradient(vec2_f32 **gradient, int ix, int iy, f32 x, f32 y) {

	/* Compute the distance vector */
	f32 dx = x - (f32)ix;
	f32 dy = y - (f32)iy;

	/* Return  dot product */
	return ((dx * gradient[iy][ix][0]) + (dy * gradient[iy][ix][1]));
}

/**
 * @brief Compute the perlin noise value for x, y coordinates
 * @param gradient: Gradient vector
 * @param x, y: float coordinate
 * @return Perlin noise value
*/
f32 perlinNoise(vec2_f32 **gradient, f32 x, f32 y) {
	/* Determine grid cellule point */
	s32 x0 = floor(x);
	s32 x1 = x0 + 1;

	s32 y0 = floor(y);
	s32 y1 = y0 + 1;

	/* Determine interpolation weights */
	f32 sx = x - (f32)x0;
	f32 sy = y - (f32)y0;

	/* Interpolate between grid point gradients */
	f32 n0, n1, ix0, ix1, value;
	n0 = dotGridGradient(gradient, x0, y0, x, y);
	n1 = dotGridGradient(gradient, x1, y0, x, y);
	
	ix0 = interpolateValues(n0, n1, sx);

	n0 = dotGridGradient(gradient, x0, y1, x, y);
	n1 = dotGridGradient(gradient, x1, y1, x, y);
	ix1 = interpolateValues(n0, n1, sx);

	value = interpolateValues(ix0, ix1, sy);
	return (value);
}

/**
 * @brief Generate a 2D sample of Perlin noise
 * @param width: Width of the sample
 * @param height: Height of the sample
 * @return 2D sample of Perlin noise
*/
f32 **noiseSample2D(int width, int height) {
    // Generate the gradient noise
    vec2_f32 **gradient = gradientNoiseGeneration(width, height);

    // Allocate memory for the sample
    f32 **sample = malloc(sizeof(f32 *) * height);
    if (!sample) {
        return (NULL);
    }

    for (int i = 0; i < height; i++) {
        sample[i] = malloc(sizeof(f32) * width);
        if (!sample[i]) {
            free_incomplete_array((void **)sample, i);
            return (NULL);
        }
    }

    // Fill the sample with Perlin noise
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            sample[i][j] = perlinNoise(gradient, (f32)j / width, (f32)i / height);
			// f32 fj = (f32)j + 0.500000;
			// f32 fi = (f32)i + 0.500000;
            // sample[i][j] = perlinNoise(gradient, fj, fi);
        }
    }

    // Free the gradient noise
    for (int i = 0; i < height; i++) {
        free(gradient[i]);
    }
    free(gradient);

    return (sample);
}
