/*
* Copyright © 2026 Thbop
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy of
* this software and associated documentation files (the “Software”), to deal in
* the Software without restriction, including without limitation the rights to
* use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
* of the Software, and to permit persons to whom the Software is furnished to do
* so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#ifndef VEC2_H
#define VEC2_H

#include "SDL3/SDL.h"
#include "stdlib.h"
#include "math.h"
#include "tmath.h"

typedef struct {
    float x, y;
} vec2;

#define VEC2_ZERO ( (vec2){ 0.0f, 0.0f } )

vec2 vec2_add( vec2 *a, vec2 *b );
vec2 vec2_add_value( vec2 *a, float value );
vec2 vec2_sub( vec2 *a, vec2 *b );
vec2 vec2_sub_value( vec2 *a, float value );
vec2 vec2_mul( vec2 *a, vec2 *b );
vec2 vec2_mul_value( vec2 *a, float value );
vec2 vec2_div( vec2 *a, vec2 *b );
vec2 vec2_div_value( vec2 *a, float value );

float vec2_dot( vec2 *a, vec2 *b );

float vec2_square_length( vec2 *a );
float vec2_length( vec2 *a );
float vec2_rlength( vec2 *a );

vec2 vec2_normalize( vec2 *a );

// start points to target
vec2 vec2_point_to( vec2 *start, vec2 *target );

float vec2_distance_to( vec2 *a, vec2 *b );
float vec2_squared_distance_to( vec2 *a, vec2 *b );

vec2 vec2_random();
vec2 vec2_random_unit();

vec2 vec2_lerp( vec2 *a, vec2 *b, float t );

vec2 vec2_reflect( vec2 *incoming, vec2 *normal );

#endif