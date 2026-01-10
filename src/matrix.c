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

#include "matrix.h"

float mat2_det( float *mat ) {
    return mat[0] * mat[3] - mat[1] * mat[2];
}

// Source: https://www.chilimath.com/lessons/advanced-algebra/inverse-of-a-2x2-matrix/
bool mat2_invert( float *mat, float *result ) {
    float det = mat2_det( mat );
    if ( SDL_fabsf( det ) < 0.0001f ) return false;

    float inv_det = 1.0f / det;
    float out[4] = {
         mat[3] * inv_det, -mat[1] * inv_det,
        -mat[2] * inv_det,  mat[0] * inv_det,
    };
    memcpy( result, out, sizeof( float ) * 4 );
    return true;
}

vec2 mat2_transform( float *mat, vec2 *p ) {
    return (vec2){
        mat[0] * p->x + mat[1] * p->y,
        mat[2] * p->x + mat[3] * p->y,
    };
}