#include "../perlin_noise.h"

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


void debugColorUpdate(u8 color, u8 *colorMin, u8 *colorMax) {
	if (color < *colorMin) {
		*colorMin = color;
	}
	if (color > *colorMax) {
		*colorMax = color;
	}
}

void debugTotalUpdate(f32 total, f32 *totalMin, f32 *totalMax) {
	if (total < *totalMin) {
		*totalMin = total;
	}
	if (total > *totalMax) {
		*totalMax = total;
	}
}

/**
 * @brief Allocate a 2D array of floats
 * @param rows,cols The number of rows and columns in the array
 * @return f32** The allocated 2D array
*/
f32 **floatDoubleArrayAlloc(int rows, int cols) {
    f32 **arr = ft_calloc(rows, sizeof(f32 *));
	if (!arr) {
		return (NULL);
	}
    for (int i = 0; i < rows; i++) {
        arr[i] = ft_calloc(cols, sizeof(f32));
		if (!arr[i]) {
			free_incomplete_array((void **)arr, i);
			return (NULL);
		}
    }
    return (arr);
}
