#include "../perlin_noise.h"

/**
 * @brief Compute the gradient of a noise
 * @param x The x coordinate
 * @param y The y coordinate
 * @param dest The destination vector 
*/
FT_INLINE void noiseGradienCompute(int x, int y, vec2_f32 dest) {
	f64 powX = x * x;
	f64 powY = y * y;

	f32 distance = sqrt(powX + powY);

	dest[0] = (f32)x / distance * 0.5;
	dest[1] = (f32)y / distance * 0.5;
}


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
			noiseGradienCompute(randomGenerationGet(), randomGenerationGet(), noise[i][j]);
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

	if (iy >= PERLIN_NOISE_HEIGHT) { iy = iy % PERLIN_NOISE_HEIGHT;}
	if (ix >= PERLIN_NOISE_WIDTH) { ix = ix % PERLIN_NOISE_WIDTH;}

	/* Return  dot product */
	return ((dx * gradient[iy][ix][0]) + (dy * gradient[iy][ix][1]));
}


f32 perlinNoise(vec2_f32 **gradient, f32 x, f32 y) {
	/* Determine grid cellule point */
	s32 x0 = (s32)floor(x);
	s32 x1 = x0 + 1;

	s32 y0 = (s32)floor(y);
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


f32 **noiseSample2D(vec2_f32 **gradient, int width, int height, f32 frequency) {

    f32 **sample = floatDoubleArrayAlloc(height, width);
    if (!sample) {
        return (NULL);
    }

    /* Fill the sample with Perlin noise */ 
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
			f32 x = (f32)j / (f32)width * frequency;
			f32 y = (f32)i / (f32)height * frequency;
            sample[i][j] = perlinNoise(gradient, x,y);
        }
    }
    return (sample);
}


f32 **perlinNoiseOctaveSample2D(vec2_f32 **gradient, int width, int height, int octaves, f32 persistence, f32 lacunarity) {
    f32 **octaveNoise = floatDoubleArrayAlloc(height, width);
    f32 amplitude = 1.0f;
    f32 totalAmplitude = 0.0f;
    f32 frequency = 1.0f;

    /* Loop through octaves */
    for (int octave = 0; octave < octaves; octave++) {
        /* Sample noise at this octave with current frequency */
        f32 **noise = noiseSample2D(gradient, width, height, frequency);

        totalAmplitude += amplitude;

        /* Add noise to the final result with appropriate amplitude */
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                octaveNoise[i][j] += noise[i][j] * amplitude;
            }
        }
        free_incomplete_array((void **)noise, height);
        /* Update amplitude and frequency for next octave */
        amplitude *= persistence;
        frequency *= lacunarity;
    }

    /* Normalize the result by dividing by total amplitude */
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            octaveNoise[i][j] /= totalAmplitude;
        }
    }
    return (octaveNoise);
}
