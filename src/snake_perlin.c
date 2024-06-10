#include "../perlin_noise.h"
#include <stdlib.h>
#include <math.h>

#define PERLIN_WHITE_VAL 0U
#define PERLIN_BLACK_VAL 1U
#define ENTRY_EXIT_VAL 2U // La valeur pour marquer les entrées/sorties

#define PATH_VAL PERLIN_BLACK_VAL // La valeur représentant le chemin

typedef struct {
    int x, y;
} PathPoint;

typedef struct {
    PathPoint 	*points;
	PathPoint 	entry;
    int 		length;
} Path;

void addPointToPath(Path *path, int x, int y) {
    path->points = realloc(path->points, (path->length + 1) * sizeof(PathPoint));
    path->points[path->length].x = x;
    path->points[path->length].y = y;
    path->length++;
}

void markEntryCircle(u8 **snakeData, int w, int h, int x, int y, int size) {
	for (int dy = -size; dy <= size; ++dy) {
		for (int dx = -size; dx <= size; ++dx) {
			if (dx * dx + dy * dy <= size * size) {
				snakeData[y + dy][x + dx] = ENTRY_EXIT_VAL;
			}
		}
	}
}

void markPathEntry(u8 **snakeData, Path *paths, int numPaths, int w, int h) {
    int entryDistThreshold = 30; // Définir le seuil de distance minimum acceptable

    for (int i = 0; i < numPaths; i++) {
        Path 		*path = paths + i;
		PathPoint 	point = path->points[path->length / 2];

		// Ajuster le point pour éviter les bords
		point.x = point.x < 5 ? 5 : point.x;
		point.x = point.x > w - 10 ? w - 10 : point.x;
		point.y = point.y < 5 ? 5 : point.y;
		point.y = point.y > h - 10 ? h - 10 : point.y;

		// Vérifier la distance avec toutes les entrées précédentes
		s8 tooClose = FALSE;
		for (int k = 0; k < numPaths; k++) {
			int dx = point.x - paths[k].entry.x;
			int dy = point.y - paths[k].entry.y;
			int distance = sqrt(dx * dx + dy * dy);
			if (distance < entryDistThreshold) {
				tooClose = TRUE;
				break;
			}
			if (tooClose) break;
		}

		if (!tooClose) {
			markEntryCircle(snakeData, w, h, point.x, point.y, 2);
			path->entry = point;
		}
    }
}

u8 **visitedPixelAlloc(int w, int h) {
	u8 **visited = ft_calloc(h, sizeof(u8*));
	if (!visited) {
		ft_printf_fd(2, "Error: ft_calloc failed\n");
		return (NULL);
	}
	for (int i = 0; i < h; ++i) {
		visited[i] = ft_calloc(w, sizeof(u8));
		if (!visited[i]) {
			ft_printf_fd(2, "Error: ft_calloc failed\n");
			return (NULL);
		}
	}
	return (visited);
}

Path *buildPaths(u8 **snakeData, int w, int h, int *nbPaths, int minLength) {
    Path *paths = NULL;
    int numPaths = 0;

    u8 **visited = visitedPixelAlloc(w, h);
    if (!visited) {
        ft_printf_fd(2, "Error: ft_calloc failed\n");
        return (NULL);
    }
    // Find the first black pixel
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            if (snakeData[y][x] == PATH_VAL && !visited[y][x]) {
                // Start a new path
                Path path = {NULL, (PathPoint){0,0}, 0};
                int currentX = x;
                int currentY = y;
                while (1) {
                    addPointToPath(&path, currentX, currentY);
                    visited[currentY][currentX] = 1;
                    // Check the 8 neighbors
                    int found = 0;
                    for (int dy = -1; dy <= 1; ++dy) {
                        for (int dx = -1; dx <= 1; ++dx) {
                            if (dy == 0 && dx == 0) continue; // Skip the center pixel
                            if (currentY + dy < 0 || currentY + dy >= h || currentX + dx < 0 || currentX + dx >= w) continue; // Check bounds
                            if (snakeData[currentY + dy][currentX + dx] == PATH_VAL && !visited[currentY + dy][currentX + dx]) {
                                currentX += dx;
                                currentY += dy;
                                found = 1;
                                break;
                            }
                        }
                        if (found) break;
                    }
                    if (!found) break;
                }
                // Add the path to the paths array if it meets the minimum length requirement
                if (path.length >= minLength) {
                    paths = realloc(paths, (numPaths + 1) * sizeof(Path));
                    paths[numPaths] = path;
                    numPaths++;
                } else {
                    // Free the path points if the path is too short
                    free(path.points);
                }
            }
        }
    }
    // Free the visited array
    for (int i = 0; i < h; ++i) {
        free(visited[i]);
    }
    free(visited);
    *nbPaths = numPaths;
    ft_printf_fd(1, RED"Number of paths: %d\n"RESET, numPaths);
	return (paths);
}

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

void makeBorderUniform(u8 **snakeData, int w, int h, int radius, u8 val) {
	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			if (x < radius || x >= w - radius || y < radius || y >= h - radius) {
				snakeData[y][x] = val;
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
	morphologicalErosion(snakeData, w, h, PERLIN_BLACK_VAL, PERLIN_WHITE_VAL, 2, 12);

	makeBorderUniform(snakeData, w, h, 2, PERLIN_WHITE_VAL);


	s32 nbPaths = 0;
	Path *paths = buildPaths(snakeData, w, h, &nbPaths, 100);
	markPathEntry(snakeData, paths, nbPaths, w, h);

	for (int i = 0; i < nbPaths; i++) {
		free(paths[i].points);
		// free(paths[i].entries);
	}
	free(paths);
	return (snakeData);
}