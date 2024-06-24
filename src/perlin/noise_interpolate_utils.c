#include "../../include/perlin_noise.h"

f32 normalisef32Tof32(f32 value, f32 start1, f32 stop1, f32 start2, f32 stop2) {
	return (start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1)));
}

f32 perlinInterpolate(f32 a, f32 b, f32 t) {
    return (a + t * (b - a));
}

f32 bilinearInterpolation(f32 q11, f32 q12, f32 q21, f32 q22, f32 x, f32 z) {
    f32 r1 = perlinInterpolate(q11, q21, x);
    f32 r2 = perlinInterpolate(q12, q22, x);
    return (perlinInterpolate(r1, r2, z));
}

f32 getInterpolatedPerlinNoise(f32 **perlinNoise, f32 x, f32 z, f32 scale, s32 width, s32 height) {
    f32 scaledX = (fabs(x) / scale);
    f32 scaledZ = (fabs(z) / scale);

    s32 x0 = scaledX;
    s32 x1 = x0 + 1;
    s32 z0 = scaledZ;
    s32 z1 = z0 + 1;


	/* set Perlin Debug val here */
	// perlinVal->x0 = x0 % width;
	// perlinVal->z0 = z0 % height;
	// perlinVal->x1 = x1 % width;
	// perlinVal->z1 = z1 % height;
    // f32 q11 = perlinNoise[perlinVal->x0][perlinVal->z0];
    // f32 q12 = perlinNoise[perlinVal->x0][perlinVal->z1];
    // f32 q21 = perlinNoise[perlinVal->x1][perlinVal->z0];
    // f32 q22 = perlinNoise[perlinVal->x1][perlinVal->z1];

	x0 = x0 % width;
	z0 = z0 % height;
	x1 = x1 % width;
	z1 = z1 % height;

	f32 q11 = perlinNoise[x0][z0];
	f32 q12 = perlinNoise[x0][z1];
	f32 q21 = perlinNoise[x1][z0];
	f32 q22 = perlinNoise[x1][z1];


    f32 tx = scaledX - x0;
    f32 tz = scaledZ - z0;

    return (bilinearInterpolation(q11, q12, q21, q22, tx, tz));
}

/* Interpolate noise value */
f32 interpolateNoiseGet(f32 **perlinNoise, s32 x, s32 z, f32 scaleFactor, s32 width, s32 height) {
	/* set Perlin Debug val here */
	// perlinVal->givenX = x;
	// perlinVal->givenZ = z;

	return (getInterpolatedPerlinNoise(perlinNoise, x, z, scaleFactor, width, height));
}

f32 normalizeU8Tof32(u8 value, u8 start1, u8 stop1, f32 start2, f32 stop2) {
    return start2 + (stop2 - start2) * ((value - start1) / (f32)(stop1 - start1));
}

f32 **array1DTo2D(u8 *array, u32 height, u32 width) {
	f32 **perlin2D = ft_calloc(height, sizeof(f32 *));
	for (u32 i = 0; i < height; ++i) {
		perlin2D[i] = ft_calloc(width, sizeof(f32));
		for (u32 j = 0; j < width; ++j) {
			perlin2D[i][j] = normalizeU8Tof32(array[i * width + j], 0, 255, -1.0f, 1.0f);
		}
	}
	return (perlin2D);
}

f32 **perlin2DFloatGet(u8 *perlin1D) {
	f32 **perlin2D = NULL;
	/* Transform 1D array to 2D array */
	perlin2D = array1DTo2D(perlin1D, PERLIN_NOISE_HEIGHT, PERLIN_NOISE_WIDTH);
	free(perlin1D);
	return (perlin2D);
}
