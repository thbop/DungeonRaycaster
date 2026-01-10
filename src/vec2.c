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

#include "vec2.h"

vec2 vec2_add( vec2 *a, vec2 *b ) {
    return (vec2){
        a->x + b->x,
        a->y + b->y,
    };
}

vec2 vec2_add_value( vec2 *a, float value ) {
    return (vec2){
        a->x + value,
        a->y + value,
    };
}

vec2 vec2_sub( vec2 *a, vec2 *b ) {
    return (vec2){
        a->x - b->x,
        a->y - b->y,
    };
}

vec2 vec2_sub_value( vec2 *a, float value ) {
    return (vec2){
        a->x - value,
        a->y - value,
    };
}

vec2 vec2_mul( vec2 *a, vec2 *b ) {
    return (vec2){
        a->x * b->x,
        a->y * b->y,
    };
}

vec2 vec2_mul_value( vec2 *a, float value ) {
    return (vec2){
        a->x * value,
        a->y * value,
    };
}
vec2 vec2_div( vec2 *a, vec2 *b ) {
    return (vec2){
        a->x / b->x,
        a->y / b->y,
    };
}

vec2 vec2_div_value( vec2 *a, float value ) {
    float inv_value = 1.0f / value;
    return (vec2){
        a->x * inv_value,
        a->y * inv_value,
    };
}


float vec2_dot( vec2 *a, vec2 *b ) {
    return a->x * b->x + a->y * b->y;
}

float vec2_square_length( vec2 *a ) {
    return vec2_dot( a, a );
}

float vec2_length( vec2 *a ) {
    return SDL_sqrt( vec2_dot( a, a ) );
}


float vec2_rlength( vec2 *a ) {
    return Q_rsqrt( vec2_dot( a, a ) );
}

vec2 vec2_normalize( vec2 *a ) {
    return vec2_mul_value( a, vec2_rlength( a ) );
}

// a points to b
vec2 vec2_point_to( vec2 *start, vec2 *target ) {
    vec2 diff = vec2_sub( target, start );
    return vec2_normalize( &diff );
}

float vec2_distance_to( vec2 *a, vec2 *b ) {
    vec2 diff = vec2_sub( a, b );
    return vec2_length( &diff );
}

float vec2_squared_distance_to( vec2 *a, vec2 *b ) {
    vec2 diff = vec2_sub( a, b );
    return vec2_square_length( &diff );
}

vec2 vec2_random() {
    vec2 p = {
        (float)rand() - ( RAND_MAX >> 1 ),
        (float)rand() - ( RAND_MAX >> 1 ),
    };

    return p;
}

vec2 vec2_random_unit() {
    vec2 p = vec2_random();
    return vec2_normalize( &p );
}


vec2 vec2_lerp( vec2 *a, vec2 *b, float t ) {
    t = SDL_clamp( t, 0.0f, 1.0f );
    vec2
        a_weight = vec2_mul_value( a, ( 1.0f - t ) ),
        b_weight = vec2_mul_value( b, t );
    return vec2_add( &a_weight, &b_weight );
}


vec2 vec2_reflect( vec2 *incoming, vec2 *normal ) {
    float a = vec2_dot( incoming, normal ) * 2;
    vec2 b = vec2_mul_value( normal, a );

    return vec2_sub( incoming, &b );
}