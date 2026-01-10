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

#include "ray.h"


vec2 ray_at( ray_t *ray, float t ) {
    return (vec2){
        ray->direction.x * t + ray->origin.x,
        ray->direction.y * t + ray->origin.y,
    };
}

vec2 ray_intersect( ray_t *a, ray_t *b ) {
    float mat[4] = {
        a->direction.x, -b->direction.x,
        a->direction.y, -b->direction.y,
    };
    if ( !mat2_invert( mat, mat ) ) return VEC2_ZERO;

    vec2 p = vec2_sub( &b->origin, &a->origin );
    
    return mat2_transform( mat, &p );
}