#include "../perlin_noise.h"

#define PERLIN_WHITE_VAL 0U
#define PERLIN_BLACK_VAL 1U

void morphologicalErosion(u8 **snakeData, int w, int h, u8 valTocheck, u8 valToSet, int neighborhoodSize, int minSameNeighbors) {
	// Morphological erosion
	for (int y = neighborhoodSize; y < h - neighborhoodSize; ++y) {
		for (int x = neighborhoodSize; x < w - neighborhoodSize; ++x) {
			// Only apply the erosion to black pixels
			if (snakeData[y][x] == valTocheck) {
				int sameNeighbors = 0;
				for (int dy = -neighborhoodSize; dy <= neighborhoodSize; ++dy) {
					for (int dx = -neighborhoodSize; dx <= neighborhoodSize; ++dx) {
						if (snakeData[y + dy][x + dx] == valTocheck) {
							sameNeighbors++;
						}
					}
				}
				// If the pixel has less than the minimum number of black neighbors, erode it
				if (sameNeighbors < minSameNeighbors) {
					snakeData[y][x] = valToSet;
				}
			}
		}
	}
}

void neighborDiffEdgeDetection(f32 **perlinData, u8 **snakeData, int w, int h, f32 threshold) {
	for (int y = 1; y < h - 1; ++y) {
		for (int x = 1; x < w - 1; ++x) {
			// Calculate the difference in value between this pixel and its neighbors
			f32 diff = 0.0f;
			for (int dy = -1; dy <= 1; ++dy) {
				for (int dx = -1; dx <= 1; ++dx) {
					diff += fabsf(perlinData[y][x] - perlinData[y + dy][x + dx]);
				}
			}
			// If the difference is above the threshold, this is an edge
			if (diff > threshold) {
				snakeData[y][x] = 1;
			} else {
				snakeData[y][x] = 0;
			}
		}
	}
}

u8 **perlinToSnakeData(f32 **perlinData, int w, int h) {
    float threshold = 0.02f; // Adjust this value to control the sensitivity of the edge detection
	u8 **snakeData = ft_calloc(h, sizeof(u8*));
	if (!snakeData) {
		ft_printf_fd(2, "Error: ft_calloc failed\n");
		return (NULL);
	}
	for (int i = 0; i < h; ++i) {
		if (!(snakeData[i] = ft_calloc(w, sizeof(u8)))) {
			ft_printf_fd(2, "Error: ft_calloc failed\n");
			return (NULL);
		}
	}
	
	neighborDiffEdgeDetection(perlinData, snakeData, w, h, threshold);
	morphologicalErosion(snakeData, w, h, PERLIN_BLACK_VAL, PERLIN_WHITE_VAL, 2, 12);
	morphologicalErosion(snakeData, w, h, PERLIN_WHITE_VAL, PERLIN_BLACK_VAL, 2, 12);

	return (snakeData);
}