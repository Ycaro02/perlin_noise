#include "../../include/perlin_noise.h"

void addPointToPath(Path *path, s32 x, s32 y) {
    path->points = realloc(path->points, (path->length + 1) * sizeof(PathPoint));
    path->points[path->length].x = x;
    path->points[path->length].y = y;
    path->length++;
}

void markEntryCircle(u8 **snakeData, s32 x, s32 y, s32 size) {
	for (s32 dy = -size; dy <= size; ++dy) {
		for (s32 dx = -size; dx <= size; ++dx) {
			if (dx * dx + dy * dy <= size * size) {
				snakeData[y + dy][x + dx] = ENTRY_EXIT_VAL;
			}
		}
	}
}

void markPathEntry(u8 **snakeData, Path *paths, s32 numPaths, s32 w, s32 h) {
    s32 entryDistThreshold = 100;

    for (s32 i = 0; i < numPaths; i++) {
        Path 		*path = paths + i;
		PathPoint 	point = path->points[path->length / 2];

		// Adjust the point to avoid the edges
		if (point.x < 5) { point.x = 5; }
		if (point.x > w - 10) { point.x = w - 10; }
		if (point.y < 5) { point.y = 5; }
		if (point.y > h - 10) { point.y = h - 10; }

		// Check the distance with all previous entries
		s8 tooClose = FALSE;
		for (s32 k = 0; k < numPaths; k++) {
			s32 dx = point.x - paths[k].entry.x;
			s32 dy = point.y - paths[k].entry.y;
			s32 distance = sqrt(dx * dx + dy * dy);
			if (distance < entryDistThreshold) {
				tooClose = TRUE;
				break;
			}
			if (tooClose) break;
		}

		if (!tooClose) {
			markEntryCircle(snakeData, point.x, point.y, 2);
			path->entry = point;
		}
    }
}

u8 **unsignedCharDoubleAlloc(s32 w, s32 h) {
	u8 **visited = ft_calloc(h, sizeof(u8*));
	
	if (!visited) {
		ft_printf_fd(2, "Error: ft_calloc failed\n");
		return (NULL);
	}
	for (s32 i = 0; i < h; ++i) {
		visited[i] = ft_calloc(w, sizeof(u8));
		if (!visited[i]) {
			ft_printf_fd(2, "Error: ft_calloc failed\n");
			return (NULL);
		}
	}
	return (visited);
}

Path buildSinglePath(u8 **visited, u8 **snakeData, s32 x, s32 y, s32 h, s32 w) {
	Path 	path = {NULL, (PathPoint){0,0}, 0};
	s32 	currentX = x;
	s32 	currentY = y;

	while (1) {
		addPointToPath(&path, currentX, currentY);
		visited[currentY][currentX] = 1;
		// Check the 8 neighbors
		s32 found = 0;
		for (s32 dy = -1; dy <= 1; ++dy) {
			for (s32 dx = -1; dx <= 1; ++dx) {
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
	return (path);
}

Path *buildPaths(u8 **snakeData, s32 w, s32 h, s32 *nbPaths, s32 minLength) {
    u8		**visited = NULL;
    Path	*totalPath = NULL;
    s32		numPaths = 0;
	
    if (!(visited = unsignedCharDoubleAlloc(w, h))) {
        ft_printf_fd(2, "Error: ft_calloc failed\n");
        return (NULL);
    }
    // Find the first black pixel
    for (s32 y = 0; y < h; ++y) {
        for (s32 x = 0; x < w; ++x) {
            if (snakeData[y][x] == PATH_VAL && !visited[y][x]) {
                // Start a new path
				Path path = buildSinglePath(visited, snakeData, x, y, h, w);
                // Add the path to the totalPath array if it meets the minimum length requirement
                if (path.length >= minLength) {
                    totalPath = realloc(totalPath, (numPaths + 1) * sizeof(Path));
                    totalPath[numPaths] = path;
                    numPaths++;
                } else {
                    // Free the path points if the path is too short
                    free(path.points);
                }
            }
        }
    }
    // Free the visited array
    for (s32 i = 0; i < h; ++i) {
        free(visited[i]);
    }
    free(visited);
    *nbPaths = numPaths;
    // ft_printf_fd(1, RED"Number of totalPath: %d\n"RESET, numPaths);
	return (totalPath);
}

void morphologicalErosion(u8 **snakeData, s32 w, s32 h, u8 valTocheck, u8 valToSet, s32 neighborhoodSize, s32 minSameNeighbors) {
	// Morphological erosion
	for (s32 y = neighborhoodSize; y < h - neighborhoodSize; ++y) {
		for (s32 x = neighborhoodSize; x < w - neighborhoodSize; ++x) {
			// Only apply the erosion to black pixels
			if (snakeData[y][x] == valTocheck) {
				s32 sameNeighbors = 0;
				for (s32 dy = -neighborhoodSize; dy <= neighborhoodSize; ++dy) {
					for (s32 dx = -neighborhoodSize; dx <= neighborhoodSize; ++dx) {
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

void neighborDiffEdgeDetection(f32 **perlinData, u8 **snakeData, s32 w, s32 h, f32 threshold) {
	for (s32 y = 1; y < h - 1; ++y) {
		for (s32 x = 1; x < w - 1; ++x) {
			// Calculate the difference in value between this pixel and its neighbors
			f32 diff = 0.0f;
			for (s32 dy = -1; dy <= 1; ++dy) {
				for (s32 dx = -1; dx <= 1; ++dx) {
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

void makeBorderUniform(u8 **snakeData, s32 w, s32 h, s32 radius, u8 val) {
	for (s32 y = 0; y < h; ++y) {
		for (s32 x = 0; x < w; ++x) {
			if (x < radius || x >= w - radius || y < radius || y >= h - radius) {
				snakeData[y][x] = val;
			}
		}
	}
}

u8 **perlinToSnakeData(f32 **perlinData, s32 w, s32 h) {
	u8 		**snakeData = NULL;
	Path 	*paths = NULL;
    f32 	threshold = 0.02f; // Adjust this value to control the sensitivity of the edge detection
	s32 	nbPaths = 0;

	if (!(snakeData = unsignedCharDoubleAlloc(w, h))) {
		ft_printf_fd(2, "Error: ft_calloc failed\n");
		return (NULL);
	}

	neighborDiffEdgeDetection(perlinData, snakeData, w, h, threshold);
	morphologicalErosion(snakeData, w, h, PERLIN_BLACK_VAL, PERLIN_WHITE_VAL, 2, 12);
	morphologicalErosion(snakeData, w, h, PERLIN_WHITE_VAL, PERLIN_BLACK_VAL, 2, 12);
	morphologicalErosion(snakeData, w, h, PERLIN_BLACK_VAL, PERLIN_WHITE_VAL, 2, 12);
	makeBorderUniform(snakeData, w, h, 2, PERLIN_WHITE_VAL);


	paths = buildPaths(snakeData, w, h, &nbPaths, 100);
	markPathEntry(snakeData, paths, nbPaths, w, h);

	for (s32 i = 0; i < nbPaths; i++) {
		free(paths[i].points);
	}
	free(paths);
	return (snakeData);
}