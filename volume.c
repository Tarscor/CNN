#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

// Include SSE intrinsics
#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#include <immintrin.h>
#include <x86intrin.h>
#endif

// Include OpenMP
#include <omp.h>

#include "volume.h"

inline double volume_get(volume_t *v, int x, int y, int d) {
    return v->weights[((v->width * y) + x) * v->depth + d];
}

inline void volume_set(volume_t *v, int x, int y, int d, double value) {
    v->weights[((v->width * y) + x) * v->depth + d] = value;
}

volume_t *make_volume(int width, int height, int depth, double value) {
    volume_t *new_vol = malloc(sizeof(struct volume));
    new_vol->weights = malloc(sizeof(double) * width * height * depth);

    new_vol->width = width;
    new_vol->height = height;
    new_vol->depth = depth;

    double *new_vol_weights = new_vol->weights;


    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            for (int d = 0; d < depth*4 / 4; d+=4) {
                new_vol_weights[((width * y) + x) * depth + d] = value;
                new_vol_weights[((width * y) + x) * depth + d + 1] = value;
                new_vol_weights[((width * y) + x) * depth + d + 2] = value;
                new_vol_weights[((width * y) + x) * depth + d + 3] = value;
            }
            for (int d = depth*4 / 4; d < depth; d++) {
              new_vol_weights[((width * y) + x) * depth + d] = value;
            }
        }
    }

    return new_vol;
}

void copy_volume(volume_t *dest, volume_t *src) {

    int dest_width = dest->width;
    int dest_height = dest->height;
    int dest_depth = dest->depth;
    double *dest_weights = dest->weights;
    double *src_weights = src->weights;

    assert(dest_width == src->width);
    assert(dest_height == src->height);
    assert(dest_depth == src->depth);

    for (int x = 0; x < dest_width; x++) {
        for (int y = 0; y < dest_height; y++) {
            for (int d = 0; d < dest_depth; d++) {
                dest_weights[((dest_width * y) + x) * dest_depth + d] = src_weights[((dest_width * y) + x) * dest_depth + d];
            }
        }
    }
}

void free_volume(volume_t *v) {
    free(v->weights);
    free(v);
}
