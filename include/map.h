/*
* MIT License
* 
* Copyright (c) 2026 Thbop
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#ifndef MAP_H
#define MAP_H

#include "stdint.h"

#include "chelp/vector_t.h"

#include "ray.h"

enum {
    TEXTURE_BRICK,
    TEXTURE_DOOR,
    TEXTURE_COUNT,
};

typedef struct {
    ray_t line;
    vec2 normal;
    uint32_t texture_id;
} wall_t;

typedef struct {
    const char *data;
    const uint32_t width, height;
} ascii_map_t;

typedef struct {
    vector_t walls;
} map_t;



void map_generate_from_ascii( ascii_map_t *ascii_map, map_t *map );

void map_free( map_t *map );

#endif