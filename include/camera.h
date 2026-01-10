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

#ifndef CAMERA_H
#define CAMERA_H


#include "SDL3/SDL.h"
#include "vec2.h"
#include "matrix.h"
#include "ray.h"

typedef struct {
    vec2 position;
    float
        rotation_delta, // In degrees
        fov,            // In degrees
        move_speed;
} camera_settings_t;

typedef struct {
    ray_t view;

    float half_sensor_width;
    
    float _move_speed;
    // Pre-computed rotation matrices
    // TODO: Possibly improve memory efficiency
    float
        _rotate_right[4],
        _rotate_left[4];
} camera_t;


void camera_new( camera_t *camera, camera_settings_t *settings );

void camera_rotate_right( camera_t *camera );
void camera_rotate_left( camera_t *camera );
void camera_move_forward( camera_t *camera );
void camera_move_backward( camera_t *camera );

#endif
