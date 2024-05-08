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

f32 **doubleArrayAlloc(int rows, int cols) {
    f32 **arr = ft_calloc(rows, sizeof(f32 *));
    for (int i = 0; i < rows; i++) {
        arr[i] = ft_calloc(cols, sizeof(f32));
    }
    return (arr);
}

f32 **perlinOctaveNoiseSample2D(vec2_f32 **gradient, int width, int height, int octaves, f32 persistence, f32 lacunarity) {
    f32 **octaveNoise = doubleArrayAlloc(height, width);
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



void colorUpdate(u8 color, u8 *colorMin, u8 *colorMax) {
	if (color < *colorMin) {
		*colorMin = color;
	}
	if (color > *colorMax) {
		*colorMax = color;
	}
}

void totalUpdate(f32 total, f32 *totalMin, f32 *totalMax) {
	if (total < *totalMin) {
		*totalMin = total;
	}
	if (total > *totalMax) {
		*totalMax = total;
	}
}

u8 *perlinImageGet(int width, int height, int octaves, f32 persistence, f32 lacurarity) {
    vec2_f32 **gradient = gradientNoiseGeneration(width, height);
    u8 *image = ft_calloc(width * height, sizeof(u8));
    f32 min = 1.0f, max = -1.0f;

    // Generate Perlin noise with octaves
    f32 **noise = perlinOctaveNoiseSample2D(gradient, width, height, octaves, persistence, lacurarity);

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
			colorUpdate(image[i * width + j], &colorMin, &colorMax);
			totalUpdate(total, &totalMin, &totalMax);
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
    return image;
}



int main(int argc, char **argv) {
    if (argc != 5) {
        ft_printf_fd(1, "Usage: %s <seed> <octaves[4-10]> <persistence[1-4]> <lacunarity [1-3]>\n", argv[0]);
        return 1;
    }
    u64 seed = array_to_uint32(argv[1]);
    int octaves = atoi(argv[2]);
    f32 persistence = atof(argv[3]);
	f32 lacurarity = atof(argv[4]);

    ft_printf_fd(1, ORANGE"seed: %u\n"RESET, seed);
    randomGenerationInit(seed);
    u8 *image = perlinImageGet(PERLIN_NOISE_WIDTH, PERLIN_NOISE_HEIGHT, octaves, persistence, lacurarity);
    init_mlx(PERLIN_NOISE_WIDTH, PERLIN_NOISE_HEIGHT, &image);
	
    return 0;
}